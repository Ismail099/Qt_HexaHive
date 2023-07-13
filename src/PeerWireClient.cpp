//
// Created by 3301-byte on 07.02.2023.
//

#include <PeerWireClient.hpp>

#include <QHostAddress>
#include <QTimerEvent>


static const int PendingRequestTimeout = 60 * 100;
static const int ClientTimeout = 120 * 1000;
static const int ConnectTimeout = 60 * 1000;
static const int KeepAliveInterval = 30 * 1000;
static const int RateControlTimerDelay = 2000;
static const int MinimalHeaderSize = 48;
static const char ProtocolId[] = "BitTorrent Protocol";
static const char ProtocolIdSize = 19;


namespace uTorrent {

	// Reads a 32-bit unsigned integer from data in network order.
	static inline quint64 fromNetworkData(const char *data) {
		const unsigned char *udata = (const unsigned char *)data;
		return (quint32(udata[0]) << 24) |
				(quint32(udata[1]) << 16) |
				(quint32(udata[2]) << 8) |
				(quint32(udata[3]));
	}

	// Writes a 32-bit unsigned integer from num to data in network order.
	static inline void toNetworkData(quint32 num, char *data) {
		unsigned char *udata = (unsigned char *)data;
		udata[3] = (num & 0xff);
		udata[2] = (num & 0xff00) >> 8;
		udata[1] = (num & 0xff0000) >> 16;
		udata[0] = (num & 0xff000000) >> 24;
	}


	// Constructs an unconnected PeerWire client and starts the connect timer.
	PeerWireClient::PeerWireClient(const QByteArray &peerId, QObject *parent) : QTcpSocket(parent),
	                                                                            _mPendingBlockSizes(0), _mPeerWireState(ChokingPeer | ChokedByPeer), _mReceivedHandshake(false),
	                                                                            _mGotPeerId(false), _mSentHandshake(false), _mNextPacketLength(-1), _mPendingRequestTimer(0),
	                                                                            _mInvalidateTimeout(false), _mKeepAliveTimer(0), _mTorrentPeer(nullptr) {

		// Fill the upload speed with 0's
		memset(_mUploadSpeedData, 0, sizeof(_mUploadSpeedData));

		// Fill the download speed with 0's
		memset(_mDownloadSpeedData, 0, sizeof(_mDownloadSpeedData));


		connect(this, &PeerWireClient::readyRead, this, &PeerWireClient::readyToTransfer);
		connect(this, &PeerWireClient::connected, this, &PeerWireClient::readyToTransfer);

		connect(&_mSocket, &QTcpSocket::connected, this, &PeerWireClient::connected);
		connect(&_mSocket, &QTcpSocket::readyRead, this, &PeerWireClient::readyRead);
		connect(&_mSocket, &QTcpSocket::disconnected, this, &PeerWireClient::disconnected);
		connect(&_mSocket, &QTcpSocket::errorOccurred, this, &PeerWireClient::errorOccurred);
		connect(&_mSocket, &QTcpSocket::bytesWritten, this, &PeerWireClient::bytesWritten);
		connect(&_mSocket, &QTcpSocket::stateChanged, this, &PeerWireClient::socketStateChanged);
	}


	// Initialize
	void PeerWireClient::initialize(const QByteArray &infoHash, int pieceCount) {
		_mInfoHash = infoHash;
		_mPeerPieces.resize(pieceCount);
		if (!_mSentHandshake)
			sendHandshake();
	}

	// Peer Setter
	void PeerWireClient::setPeer(TorrentPeer *peer) {
		_mTorrentPeer = peer;
	}

	// Peer
	TorrentPeer *PeerWireClient::peer() const {
		return _mTorrentPeer;
	}

	// Available Pieces
	QBitArray PeerWireClient::availablePieces() const {
		return _mPeerPieces;
	}

	// Incoming Blocks
	QList<TorrentBlock> PeerWireClient::incomingBlocks() const {
		return _mIncoming;
	}


	//
	// Protocol
	//

	// Choke Peer
	void PeerWireClient::chokePeer() {
		const char message[] = {0, 0, 0, 1, 0};
		write(message, sizeof(message));
		_mPeerWireState |= ChokingPeer;

		// After receiving a choke message, the peer will assume all pending requests are lost.
		_mPendingBlocks.clear();
		_mPendingBlockSizes = 0;
	}


	// Unchoke Peer
	void PeerWireClient::unchokePeer() {
		const char message[] = {0, 0, 0, 1, 1};
		write(message, sizeof(message));
		_mPeerWireState &= ~ChokingPeer;

		// If there is a pending request timer, then kill it.
		if (_mPendingRequestTimer)
			killTimer(_mPendingRequestTimer);
	}


	// Send Interested
	void PeerWireClient::sendInterested() {
		const char message[] = {0, 0, 0, 1, 2};
		write(message, sizeof(message));
		_mPeerWireState |= InterestedInPeer;

		// After telling the peer that we're interested, we expect to get
		// unchoked within a certain timeframe; otherwise we'll drop the connection
		if (_mPendingRequestTimer)
			killTimer(_mPendingRequestTimer);
		_mPendingRequestTimer = startTimer(PendingRequestTimeout);
	}


	// Send Keep Alive
	void PeerWireClient::sendKeepAlive() {
		const char message[] = {0, 0, 0, 0};
		write(message, sizeof(message));
	}


	// Send Piece Notification
	void PeerWireClient::sendPieceNotification(int piece) {
		if (!_mSentHandshake)
			sendHandshake();

		char message[] = {0, 0, 0, 5, 4, 0, 0, 0, 0};
		toNetworkData(piece, &message[5]);
		write(message, sizeof(message));
	}


	// Send Piece List
	void PeerWireClient::sendPieceList(const QBitArray  &bitField) {

		// The bitfield message may only be sent immediately after the handshaking sequence is completed,
		// and before any other messages are sent.
		if (!_mSentHandshake)
			sendHandshake();

		// Don't send the bitfield if it's all zeros
		if (bitField.count(true) == 0)
			return;

		int bitFieldSize = bitField.size();
		int size = (bitFieldSize + 7) / 8;
		QByteArray bits(size, '\0');
		for (int i = 0; i < bitFieldSize; ++i) {
			if (bitField.testBit(i)) {
				quint32 byte = quint32(i) / 8;
				quint32 bit = quint32(i) % 8;
				bits[byte] = uchar(bits.at(byte)) | (1 << (7 - bit));
			}
		}

		char message[] = {0, 0, 0, 1, 5};
		toNetworkData(bits.size() + 1, &message[0]);
		write(message, sizeof(message));
		write(bits);
	}


	// Request Block
	void PeerWireClient::requestBlock(int piece, int offset, int length) {

		char message[] = {0, 0, 0, 1, 6};
		toNetworkData(13, &message[0]);
		write(message, sizeof(message));

		char numbers[4 * 3];
		toNetworkData(piece, &numbers[0]);
		toNetworkData(offset, &numbers[4]);
		toNetworkData(length, &numbers[8]);
		write(numbers, sizeof(numbers));

		_mIncoming << TorrentBlock(piece, offset, length);

		// After requesting a block, we expect the block to be sent by the other
		// peer within a certain number of seconds. Otherwise, we drop the connection.
		if (_mPendingRequestTimer)
			killTimer(_mPendingRequestTimer);
		_mPendingRequestTimer = startTimer(PendingRequestTimeout);
	}


	// Cancel Request
	void PeerWireClient::cancelRequest(int piece, int offset, int length) {

		char message[] = {0, 0, 0, 1, 8};
		toNetworkData(13, &message[0]);
		write(message, sizeof(message));

		char numbers[4 * 3];
		toNetworkData(piece, &numbers[0]);
		toNetworkData(offset, &numbers[4]);
		toNetworkData(length, &numbers[8]);
		write(message, sizeof(numbers));

		_mIncoming.removeAll(TorrentBlock(piece, offset, length));
	}


	// Send Block
	void PeerWireClient::sendBlock(int piece, int offset, const QByteArray &data) {

		QByteArray block;

		char message[] = {0, 0, 0, 1, 7};
		toNetworkData(9 + data.size(), &message[0]);
		block += QByteArray(message, sizeof(message));

		char numbers[4 * 2];
		toNetworkData(piece, &numbers[0]);
		toNetworkData(offset, &numbers[4]);
		block += QByteArray(numbers, sizeof(numbers));
		block += data;

		BlockInfo blockInfo;
		blockInfo.pieceIndex = piece;
		blockInfo.offset = offset;
		blockInfo.length = data.size();
		blockInfo.block = block;

		_mPendingBlocks << blockInfo;
		_mPendingBlockSizes += block.size();

		if (_mPendingRequestTimer > 32 * 16384) {
			chokePeer();
			unchokePeer();
			return;
		}

		emit readyToTransfer();
	}


	/// Rate Control

	// Write To Socket
	qint64 PeerWireClient::writeToSocket(qint64 bytes) {

		qint64 totalWritten = 0;
		do {
			if (_mOutgoingBuffer.isEmpty() && !_mPendingBlocks.isEmpty()) {
				BlockInfo block = _mPendingBlocks.takeFirst();
				_mPendingBlockSizes -= block.length;
				_mOutgoingBuffer += block.block;
			}
			qint64 written = _mSocket.write(_mOutgoingBuffer.constData(),
			                                qMin<qint64>(bytes - totalWritten, _mOutgoingBuffer.size()));

			if (written <= 0)
				return totalWritten ? totalWritten : written;

			totalWritten += written;
			_mUploadSpeedData[0] += written;
			_mOutgoingBuffer.remove(0, written);
		}
		while(totalWritten < bytes && (!_mOutgoingBuffer.isEmpty()) || !_mPendingBlocks.isEmpty());

		return totalWritten;
	}


	// Read From Socket
	qint64 PeerWireClient::readFromSocket(qint64 bytes) {

		char buffer[1024];
		qint64 totalRead = 0;

		do {
			// Read into buffer with the maximum size of second parameter
			qint64 bytesRead = _mSocket.read(buffer, qMin<qint64>(sizeof(buffer), bytes - totalRead));
			if (bytesRead <= 0)
				break;

			qint64 oldSize = _mIncomingBuffer.size();
			_mIncomingBuffer.resize(oldSize + bytesRead);
			memcpy(_mIncomingBuffer.data() + oldSize, buffer, bytesRead);

			totalRead += bytesRead;
		}
		while (totalRead < bytes);

		if (totalRead > 0) {
			_mDownloadSpeedData[0] += totalRead;
			emit bytesReceived(totalRead);
			processIncomingData();
		}

		return totalRead;
	}


	// Download Speed
	qint64 PeerWireClient::downloadSpeed() const {

		qint64 sum = 0;
		for (long long i : _mDownloadSpeedData)
			sum += i;
		return sum / (8 * 2);
	}


	// Upload Speed
	qint64 PeerWireClient::uploadSpeed() const {

		qint64 sum = 0;
		for (long long i : _mUploadSpeedData)
			sum += i;
		return sum / (8 * 2);
	}


	// Can Fetch More
	bool PeerWireClient::canTransferMore() const {

		return bytesAvailable() > 0 || _mSocket.bytesAvailable() > 0 || !_mOutgoingBuffer.isEmpty()
		       || !_mPendingBlocks.isEmpty();
	}


	// Set Read Buffer Size
	void PeerWireClient::setReadBufferSize(qint64 size) {
		_mSocket.setReadBufferSize(size);
	}


	// Connect To Host
	void PeerWireClient::connectToHost(const QString &address, quint16 port, OpenMode openMode,
	                                   NetworkLayerProtocol protocol) {

		setOpenMode(openMode);
		_mSocket.connectToHost(address, port, openMode, protocol);
	}

	// Disconnect From Host
	void PeerWireClient::disconnectFromHost() {
		_mSocket.disconnectFromHost();
	}


	// Timer Event
	void PeerWireClient::timerEvent(QTimerEvent *event) {

		if (event->timerId() == _mTransferSpeedTimer) {
			// Rotate the upload / download records
			for (int i = 6; i >= 0; --i) {
				_mUploadSpeedData[i + 1] = _mUploadSpeedData[i];
				_mDownloadSpeedData[i + 1] = _mDownloadSpeedData[i];
			}

			_mUploadSpeedData[0] = 0;
			_mDownloadSpeedData[0] = 0;
		}
		else if (event->timerId() == _mTimeoutTimer) {
			// Disconnect if we timed out; otherwise the timeout is restarted.
			if (_mInvalidateTimeout) {
				_mInvalidateTimeout = false;
			}
			else {
				abort();
				emit infoHashReceived(QByteArray());
			}
		}
		else if (event->timerId() == _mPendingRequestTimer) {
			abort();
		}
		else if (event->timerId() == _mKeepAliveTimer) {
			sendKeepAlive();
		}

		QTcpSocket::timerEvent(event);
	}


	// Send Handshake
	void PeerWireClient::sendHandshake() {

		_mSentHandshake = true;

		// Restart the timeout
		if (_mTimeoutTimer)
			killTimer(_mTimeoutTimer);
		_mTimeoutTimer = startTimer(ClientTimeout);


		// Write the 68 byte PeerWire handshake.
		write(&ProtocolIdSize, 1);
		write(ProtocolId, ProtocolIdSize);
		write(QByteArray(8, '\0'));
		write(_mInfoHash);
		write(_mPeerIdString);
	}


	// Process Incoming Data
	void PeerWireClient::processIncomingData() {

		_mInvalidateTimeout = true;
		if (!_mReceivedHandshake) {
			// Checks that we received enough data
			if (bytesAvailable() < MinimalHeaderSize)
				return;

			// Sanity check the protocol id
			QByteArray id = read(ProtocolIdSize + 1);
			if (id.at(0) != ProtocolIdSize || !id.mid(1).startsWith(ProtocolId)) {
				abort();
				return;
			}

			// Discard 8 reserved bytes, then read the info hash and peer id
			(void) read(8);

			// Read infoHash
			QByteArray peerInfoHash = read(20);
			if (!_mInfoHash.isEmpty() && peerInfoHash != _mInfoHash) {
				abort();
				return;
			}

			emit infoHashReceived(peerInfoHash);
			if (_mInfoHash.isEmpty()) {
				abort();
				return;
			}

			// Send handshake
			if (!_mSentHandshake)
				sendHandshake();
			_mReceivedHandshake = true;
		}


		// Handle delayed peer id arrival
		if (!_mGotPeerId) {
			if (bytesAvailable() < 20)
				return;
			_mGotPeerId = true;
			if (read(20) == _mPeerIdString) {
				// We connected to ourselves
				abort();
				return;
			}
		}


		// Initializing keep-alive timer
		if (!_mKeepAliveTimer)
			_mKeepAliveTimer = startTimer(KeepAliveInterval);


		do {
			// Find the packet length
			if (_mNextPacketLength == -1) {
				if (bytesAvailable() < 4)
					return;

				char tmp[4];
				read(tmp, sizeof(tmp));
				_mNextPacketLength = fromNetworkData(tmp);

				if (_mNextPacketLength < 0 || _mNextPacketLength > 200000) {
					// Prevent DoS
					abort();
					return;
				}
			}

			// Keep Alive
			if (_mNextPacketLength == 0) {
				_mNextPacketLength = -1;
				continue;
			}


			// Wait with parsing until the whole packet has been received
			if (bytesAvailable() < _mNextPacketLength)
				return;


			// Read the packet
			QByteArray packet = read(_mNextPacketLength);
			if (packet.size() != _mNextPacketLength) {
				abort();
				return;
			}

			switch (packet.at(0)) {
				case ChockePacket:
					// We have been choked
					_mPeerWireState |= ChokedByPeer;
					_mIncomingBuffer.clear();
					if (_mPendingRequestTimer)
						killTimer(_mPendingRequestTimer);

					emit choked();
					break;

				case UnchockePacket:
					// We have been unchoked
					_mPeerWireState &= ~ChokedByPeer;
					emit unchoked();
					break;


				case InterestedPacket:
					// The peer is interested in downloading.
					_mPeerWireState |= PeerIsInterested;
					emit interested();
					break;

				case NotInterestedPacket:
					// The peer is not interested in downloading.
					_mPeerWireState &= ~PeerIsInterested;
					emit notInterested();
					break;

				case HavePacket: {
					// The peer has a new piece available.
					quint64 index = fromNetworkData(&packet.data()[1]);
					if (index < quint64(_mPeerPieces.size())) {
						// Only accept indexes within the valid range
						_mPeerPieces.setBit(int(index));
					}
					emit piecesAvailable(availablePieces());
					break;
				}

				case BitFiledPacket:
					// The peer has the following pieces available.
					for (int i = 1; i < packet.size(); ++i) {
						for (int bit = 0; bit < 8; ++bit) {
							if (packet.at(i) & (1 << (7 - bit))) {
								int bitIndex = int(((i - 1) * 8) + bit);
								if (bitIndex >= 0 && bitIndex < _mPeerPieces.size()) {
									// Occasionally, broken clients claim to have pieces whose index
									// is outside the valid range.
									_mPeerPieces.setBit(bitIndex);
								}
							}
						}
					}

					emit piecesAvailable(availablePieces());
					break;

				case RequestPacket: {
					// The peer requests a block.
					quint64 index = fromNetworkData(&packet.data()[1]);
					quint64 begin = fromNetworkData(&packet.data()[5]);
					quint64 length = fromNetworkData(&packet.data()[9]);
					emit blockRequested(int(index), int(begin), int(length));
					break;
				}

				case PiecePacket: {
					int index = int(fromNetworkData(&packet.data()[1]));
					int begin = int(fromNetworkData(&packet.data()[5]));

					_mIncoming.removeAll(TorrentBlock(index, begin, packet.size() - 9));

					// The peer sends a block.
					emit blockReceived(index, begin, packet.mid(9));

					// Kill the pending block timer.
					if (_mPendingRequestTimer) {
						killTimer(_mPendingRequestTimer);
						_mPendingRequestTimer = 0;
					}
					break;
				}

				case CancelPacket: {
					// The peer cancels a block request.
					quint32 index = fromNetworkData(&packet.data()[1]);
					quint32 begin = fromNetworkData(&packet.data()[5]);
					quint32 length = fromNetworkData(&packet.data()[9]);
					for (int i = 0; i < _mPendingBlocks.size(); ++i) {
						const BlockInfo &blockInfo = _mPendingBlocks.at(i);
						if (blockInfo.pieceIndex == int(index) && blockInfo.offset == int(begin)
							&& blockInfo.length == int(length)) {

							_mPendingBlocks.removeAt(i);
							break;
						}
					}
					break;
				}

				default:
					// Unsupported packet type; just ignore it.
					break;
			}
			_mNextPacketLength = -1;
		}
		while (bytesAvailable() > 0);
	}


	// Socket State Changed
	void PeerWireClient::socketStateChanged(QAbstractSocket::SocketState state) {

		setLocalAddress(_mSocket.localAddress());
		setLocalPort(_mSocket.localPort());
		setPeerName(_mSocket.peerName());
		setPeerAddress(_mSocket.peerAddress());
		setPeerPort(_mSocket.peerPort());
		setSocketState(state);
	}


	// Read Data
	qint64 PeerWireClient::readData(char *data, qint64 size) {

		int n = qMin<int>(size, _mIncomingBuffer.size());
		memcpy(data, _mIncomingBuffer.constData(), n);
		_mIncomingBuffer.remove(0, n);
		return n;
	}


	// Read Line Data
	qint64 PeerWireClient::readLineData(char *data, qint64 maxlen) {
		return QIODevice::readLineData(data, maxlen);
	}

	// Write Data
	qint64 PeerWireClient::writeData(const char *data, qint64 len) {

		int oldSize = _mOutgoingBuffer.size();
		_mOutgoingBuffer.resize(oldSize + len);
		memcpy(_mOutgoingBuffer.data() + oldSize, data, len);

		emit readyToTransfer();
		return len;
	}


} // uTorrent