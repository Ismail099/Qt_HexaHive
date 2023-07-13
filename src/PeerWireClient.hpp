//
// Created by 3301-byte on 07.02.2023.
//

#ifndef UTORRENT_PEERWIRECLIENT_HPP
#define UTORRENT_PEERWIRECLIENT_HPP

#include <QBitArray>
#include <QList>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QHostAddress;
class QTimerEvent;
QT_END_NAMESPACE


class TorrentPeer;


namespace uTorrent {

	/**
	 * @brief TorrentBlock Is a struct that represents a block of torrent.
	 */
	struct TorrentBlock {
		inline TorrentBlock(int p, int o, int  l)
			: pieceIndex(p), offset(o), length(l) {

		}

		inline bool operator==(const TorrentBlock &other) const {
			return pieceIndex == other.pieceIndex &&
				offset == other.offset &&
				length == other.length;
		}

		int pieceIndex;
		int offset;
		int length;
	};


	/*!
	 * @brief
	 */
	class PeerWireClient : public QTcpSocket {

			Q_OBJECT

		public:
			enum PeerWireStateFlag {
				ChokingPeer = 0x1,
				InterestedInPeer = 0x2,
				ChokedByPeer = 0x4,
				PeerIsInterested = 0x8
			};
			Q_DECLARE_FLAGS(PeerWireState, PeerWireStateFlag)

			/*!
			 * @brief PeerWireClient
			 * @param peerId
			 * @param parent
			 */
			explicit PeerWireClient(const QByteArray &peerId, QObject *parent = nullptr);


			/*!
			 * @brief initialize Registers the peer ID and SHA3 sum of the torrent, and initiates the handshake.
			 * @param infoHash
			 * @param pieceCount
			 */
			void initialize(const QByteArray &infoHash, int pieceCount);


			void setPeer(TorrentPeer *peer);
			TorrentPeer *peer() const;


			// State
			inline PeerWireState peerWireState() const {
				return _mPeerWireState;
			}
			QBitArray availablePieces() const;
			QList<TorrentBlock> incomingBlocks() const;


			// Protocol
			/*!
			 * @brief chokePeer Sends a "choke" message, asking the peer to stop requesting blocks.
			 */
			void chokePeer();

			/*!
			 * @brief unchokePeer Sends an "unchoke" message, allowing the peer to start/resume
			 * 					  requesting blocks.
			 */
			void unchokePeer();

			/*!
			 * @brief sendInterested Sends an "interested" message, informing the peer that it has got pieces that we'd
			 * 						 like to download.
			 */
			void sendInterested();

			/*!
			 * @brief sendKeepAlive Sends a "keep-alive" message to prevent the peer from closing the connection when
			 * 						there's no activity.
			 */
			void sendKeepAlive();

			/*!
			 * @brief sendNotInterested Sends an "not-interested" message, informing the peer that it does not have
			 * 							any pieces that we'd like to download.
			 */
			void sendNotInterested();

			/*!
			 * @brief sendPieceNotification Sends a piece notification / a "have" message, informing the peer that we
			 *								have just downloaded a new piece.
			 * @param piece
			 */
			void sendPieceNotification(int piece);

			/*!
			 * @brief sendPieceList Sends the complete list of pieces that we have downloaded.
			 * @param bitField
			 */
			void sendPieceList(const QBitArray  &bitField);

			/*!
			 * @brief requestBlock Sends a request for a block.
			 * @param piece
			 * @param offset
			 * @param length
			 */
			void requestBlock(int piece, int offset, int length);

			/*!
			 * @brief cancelRequest Cancels a request for a block.
			 * @param piece
			 * @param offset
			 * @param length
			 */
			void cancelRequest(int piece, int offset, int length);

			/*!
			 * @brief sendBlock Sends a block to the peer.
			 * @param piece
			 * @param offset
			 * @param data
			 */
			void sendBlock(int piece, int offset, const QByteArray &data);


			/// Rate Control
			/*!
			 * @brief writeToSocket Attempts to write 'bytes' to the socket from the buffer.
			 * This is used by RateController, which precisely controls how much each client can write.
			 * @param bytes
			 * @return 64-bit integer
			 */
			qint64 writeToSocket(qint64 bytes);

			/*!
			 * @brief readFromSocket Attempts to read at most 'bytes' bytes from the socket.
			 * @param bytes
			* @return 64-bit integer
			 */
			qint64 readFromSocket(qint64 bytes);

			/*!
			 * @brief downloadSpeed Returns the average number of bytes per second this client is downloading.
			 * @return
			 */
			qint64 downloadSpeed() const;

			/*!
			 * @brief uploadSpeed Returns the average number of bytes per second this client is uploading.
			 * @return
			 */
			qint64 uploadSpeed() const;

			/*!
			 * @brief canTransferMore
			 * @return
			 */
			bool canTransferMore() const;

			/*!
			 * @brief bytesAvailable
			 * @return
			 */
			qint64 bytesAvailable() const override {
				return _mIncomingBuffer.size() + QTcpSocket::bytesAvailable();
			}
			qint64 socketBytesAvailable() const {
				return _mSocket.bytesAvailable();
			}
			qint64 socketBytesToWrite() const {
				return _mSocket.bytesToWrite();
			}

			/*!
			 * @brief setReadBufferSize
			 * @param size
			 */
			void setReadBufferSize(qint64 size) override;

			using QTcpSocket::connectToHost;
			void connectToHost(const QString &address, quint16 port, OpenMode openMode = ReadWrite,
							   NetworkLayerProtocol protocol = AnyIPProtocol);
			void disconnectFromHost() override;


		signals:
			void infoHashReceived(const QByteArray &infoHash);
			void readyToTransfer();

			void choked();
			void unchoked();
			void interested();
			void notInterested();


			void piecesAvailable(const QBitArray &pieces);
			void blockRequested(int pieceIndex, int begin, int length);
			void blockReceived(int pieceIndex, int begin, const QByteArray &data);


			void bytesReceived(qint64 size);

		protected:
			void timerEvent(QTimerEvent *event) override;

			qint64 readData(char *data, qint64 maxlen) override;
			qint64 readLineData(char *data, qint64 maxlen) override;
			qint64 writeData(const char *data, qint64 len) override;


		private slots:
			/*!
			 * @brief sendHandShake Sends the handshake to the peer.
			 */
			void sendHandshake();

			/*!
			 * @brief processIncomingData
			 */
			void processIncomingData();

			void socketStateChanged(QAbstractSocket::SocketState state);


		private:

			// Data waiting to be read/write
			QByteArray _mIncomingBuffer;
			QByteArray _mOutgoingBuffer;

			struct BlockInfo {
				int pieceIndex;
				int offset;
				int length;
				QByteArray block;
			};

			QList<BlockInfo> _mPendingBlocks;
			int _mPendingBlockSizes;
			QList<TorrentBlock> _mIncoming;


			enum PacketType {
				ChockePacket = 0,
				UnchockePacket = 1,
				InterestedPacket = 2,
				NotInterestedPacket = 3,
				HavePacket = 4,
				BitFiledPacket = 5,
				RequestPacket = 6,
				PiecePacket = 7,
				CancelPacket = 8
			};


			// State
			PeerWireState _mPeerWireState;
			bool _mReceivedHandshake;
			bool _mGotPeerId;
			bool _mSentHandshake;
			int _mNextPacketLength;


			// Upload/Download speed records
			qint64 _mUploadSpeedData[8];
			qint64 _mDownloadSpeedData[8];
			int _mTransferSpeedTimer;

			// Timeout handling
			int _mTimeoutTimer;
			int _mPendingRequestTimer;
			bool _mInvalidateTimeout;
			int _mKeepAliveTimer;

			// Checksum, peer ID and set of available pieces
			QByteArray _mInfoHash;
			QByteArray _mPeerIdString;
			QBitArray _mPeerPieces;

			TorrentPeer *_mTorrentPeer;

			QTcpSocket _mSocket;
	};

} // uTorrent

Q_DECLARE_OPERATORS_FOR_FLAGS(uTorrent::PeerWireClient::PeerWireState)

#endif //UTORRENT_PEERWIRECLIENT_HPP
