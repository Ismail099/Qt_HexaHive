/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Bencode Parser.
 * @details     Bencode is the encoding used by the peer-to-peer file sharing system BitTorrent for storing and
 *              transmitting loosely structured data. It is most commonly used in torrent files.
 **/
#include <BencodeParser.h>


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

		_Content = content;
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

		do {
			char character = _Content.at(_Index);

			// If character is less than 0 and greater than 9
			if (character < '0' || character > '9') {
				if (size == -1) {
					return false;
				}

				// Missing colon between size and content
				if (character != ':') {
					_ErrorString = QString("Unexpected character at post %1: %2").arg(_Index, character);
					return false;
				}

				++_Index;
				break;
			}

			// negative size
			if (size == -1) {
				size = 0;
			}
			size *= 10;
			size += character - '0';
		} while (++_Index < contentSize);

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

		// Invalid syntax if it doesn't start with an 'i' (not an integer)
		if (_Content.at(_Index) != 'i') {
			return false;
		}

		// Else increment the index
		++_Index;
		qint64 num = -1;
		bool negative = false;

		// Iterate the value
		while (++_Index < contentSize) {
			// Take a character
			char character = _Content.at(_Index);

			// If character is less than 0 and greater than 9
			if (character < '0' || character > '9') {

				// If integer is negative
				if (num == -1) {

					if (character != '-' || negative) {
						return false;
					}
					negative = true;
					continue;
				} else {
					// If value is not ended with an 'e', syntax error.
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

		// If it doesn't start with an 'l' (not a list)
		if (_Content.at(_Index) != 'l')
			return false;

		QList<QVariant> tmp;
		++_Index;

		do {

			// Index is at the end of the string
			if (_Content.at(_Index) == 'e') {
				++_Index;
				return false;
			}

			qint64 number;
			QByteArray byteString;
			QList<QVariant> tmpList;
			Dictionary dictionary;

			// Parse integers, byte strings, lists and dictionaries.
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
		} while (_Index < contentSize);

		if (list) {
			*list = tmp;
		}

		return true;
	}


	//
	// Getter function of the dictionary format in bencode.
	//
	bool BencodeParser::getDictionary(QMap<QByteArray, QVariant> *dictionary) {

		const int contentSize = _Content.size();

		// If content doesn't start with a 'd' (not a dictionary), return false
		if (_Content.at(_Index) != 'd') {
			return false;
		}

		Dictionary tmpDict;
		++_Index;

		do {

			// Index is at the end of the string
			if (_Content.at(_Index) == 'e') {
				++_Index;
				break;
			}

			// Try to get byte string
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

			// Parse integers, byte strings, lists and dictionaries.
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
		} while (_Index < contentSize);

		if (dictionary) {
			*dictionary = tmpDict;
		}

		return true;
	}
}