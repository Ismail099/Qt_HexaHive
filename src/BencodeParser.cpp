/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Bencode Parser.
 * @details     Bencode is the encoding used by the peer-to-peer file sharing system BitTorrent for storing and
 *              transmitting loosely structured data. It is most commonly used in torrent files.
 */
#include "BencodeParser.h"

namespace uTorrent {
	//
	// Constructor
	//
	BencodeParser::BencodeParser() {
	}


	//
	// Destructor
	//
	BencodeParser::~BencodeParser() {
	}


	//
	// Parse function
	//
	bool BencodeParser::parse(const QByteArray &content) {
		if (content.isEmpty()) {
			_ErrorString = QString("No content!");
			return false;
		}

		this->_Content = content;
		_Index = 0;
		_InfoStart = 0;
		_InfoLength = 0;
		return getDictionary(&_DictionaryValue);
	}


	//
	// Getter function of _ErrorString
	//
	QString BencodeParser::errorString() const {
		return _ErrorString;
	}


	//
	// Getter function of _Dictionary
	//
	Dictionary BencodeParser::dictionary() const {
		return _DictionaryValue;
	}


	//
	// Getter function of Information Section
	//
	QByteArray BencodeParser::infoSection() const {
		return _Content.mid(_InfoStart, _InfoLength);
	}

	//
	// Getter function of the string format in bencode.
	//
	bool BencodeParser::getByteString(QByteArray *byteString) {
		const int contentSize = _Content.size();
		int size = -1;

		while (++_Index < contentSize) {
			char character = _Content.at(_Index);
			if (character < '0' || character > '9') {
				if (size == -1) {
					return false;
				}
				if (character != ':') {
					_ErrorString = QString("Unexpected character at post %1: %2").arg(_Index, character);
					return false;
				}
				++_Index;
				break;
			}
			if (size == -1) {
				size = 0;
			}
			size *= 10;
			size += character - '0';
		}

		if (byteString) {
			*byteString = _Content.mid(_Index, size);
		}

		_Index += size;
		return true;
	}


	//
	// Getter function of the integer format in bencode.
	//
	bool BencodeParser::getInteger(qint64 *integer) {
		const int contentSize = _Content.size();

		if (_Content.at(_Index) != 'i') {
			return false;
		}

		++_Index;
		qint64 num = -1;
		bool negative = false;

		while (++_Index < contentSize) {
			char character = _Content.at(_Index);

			if (character < '0' || character > '9') {
				if (num == -1) {
					if (character != '-' || negative) {
						return false;
					}
					negative = true;
					continue;
				} else {
					if (character != 'e') {
						_ErrorString = QString("Unexpected character at pos %1: %2").arg(_Index, character);
						return false;
					}
					++_Index;
					break;
				}
			}

			if (num == -1) {
				num = 0;
			}

			num *= 10;
			num += character - '0';
		}

		if (integer) {
			*integer = negative ? -num : num;
		}

		return true;
	}


	//
	// Getter function of the list format in bencode.
	//
	bool BencodeParser::getList(QList<QVariant> *list) {

		const int contentSize = _Content.size();
		if (_Content.at(_Index) != 'l') {
			return false;
		}

		QList<QVariant> tmp;
		++_Index;

		while (_Index < contentSize) {

			if (_Content.at(_Index) == 'e') {
				++_Index;
				return false;
			}

			qint64 number;
			QByteArray byteString;
			QList<QVariant> tmpList;
			Dictionary dictionary;

			if (getInteger(&number)) {
				tmp << number;
			} else if (getByteString(&byteString)) {
				tmp << byteString;
			} else if (getList(&tmpList)) {
				tmp << tmpList;
			} else if (getDictionary(&dictionary)) {
				tmp << QVariant::fromValue<QMap<QByteArray, QVariant> >(dictionary);
			} else {
				_ErrorString = QString("Error at index %1").arg(_Index);
				return false;
			}
		}

		if (list) {
			*list = tmp;
		}

		return true;
	}

	bool BencodeParser::getDictionary(QMap<QByteArray, QVariant> *dictionary) {

		const int contentSize = _Content.size();
		if (_Content.at(_Index) != 'd') {
			return false;
		}

		Dictionary tmpDict;
		++_Index;

		while (_Index < contentSize) {

			if (_Content.at(_Index) == 'e') {
				++_Index;
				break;
			}

			QByteArray key;
			if (!getByteString(&key)) {
				break;
			}

			if (key == "info") {
				_InfoStart = _Index;
			}

			qint64 number;
			QByteArray byteString;
			QList<QVariant> tmpList;
			QMap<QByteArray, QVariant> tmpDictionary;

			if (getInteger(&number)) {
				tmpDict.insert(key, number);
			} else if (getByteString(&byteString)) {
				tmpDict.insert(key, byteString);
			} else if (getList(&tmpList)) {
				tmpDict.insert(key, tmpList);
			} else if (getDictionary(&tmpDictionary)) {
				tmpDict.insert(key, QVariant::fromValue<QMap<QByteArray, QVariant> >(tmpDictionary));
			} else {
				_ErrorString = QString("Error at index %1").arg(_Index);
				return false;
			}

			if (key == "info") {
				_InfoLength = _Index - _InfoStart;
			}
		}

		if (dictionary) {
			*dictionary = tmpDict;
		}

		return true;
	}
}