/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Bencode Parser.
 * @details     Bencode is the encoding used by the peer-to-peer file sharing system BitTorrent for storing and
 *              transmitting loosely structured data. It is most commonly used in torrent files.
 *              For more information, have a look at https://en.wikipedia.org/wiki/Bencode.
 */

#ifndef UTORRENT_BENCODEPARSER_H
#define UTORRENT_BENCODEPARSER_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QList>

typedef QMap<QByteArray, QVariant> Dictionary;

Q_DECLARE_METATYPE(Dictionary)

namespace uTorrent {
	/*!
	 * @brief BencodeParser class
	 */
	class BencodeParser {

		public:
			explicit BencodeParser();

			~BencodeParser();

			bool parse(const QByteArray &content);

			QString errorString() const;

			Dictionary dictionary() const;

			/*!
			 * @brief infoSection       Getter function of all the data contained in the information section.
			 * @return                  QByteArray
			 */
			QByteArray infoSection() const;

		private:
			bool getByteString(QByteArray *byteString);

			bool getInteger(qint64 *integer);

			bool getList(QList<QVariant> *list);

			bool getDictionary(QMap<QByteArray, QVariant> *dictionary);

			QMap<QByteArray, QVariant> _DictionaryValue;

			QString _ErrorString;
			QByteArray _Content;

			int _Index;
			int _InfoStart;
			int _InfoLength;
	};
}


#endif //UTORRENT_BENCODEPARSER_H
