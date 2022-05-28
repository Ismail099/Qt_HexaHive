/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Meta Information.
 * @details
 */

#ifndef UTORRENT_METAINFORMATION_H
#define UTORRENT_METAINFORMATION_H


#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QString>
#include <QStringList>
#include <QVariant>


namespace uTorrent {
	/*!
	 * @brief MetaInfoSingleFile    A struct represents a file structure of a torrent file.
	 */
	struct MetaInfoSingleFile {
		qint64 length;              // size of the file in bytes.
		QByteArray checksum;
		QString name;
		int pieceLength;            // number of bytes per piece. This is commonly 2^8 KiB = 256 KiB = 262.144 Byte.
		QList<QByteArray> sha3sum;  // Is a hash list, i.e, a concatenation of each piece's SHA-3 hash.
	};


	struct MetaInfoMultiFile {
		qint64 length;
		QByteArray checksum;
		QString path;
	};


	class MetaInformation {

		public:
			enum FileForm {
				SingleFileForm,
				MultiFileForm,
			};

			explicit MetaInformation();

			void clear();

			/*!
			 * @brief parse     Parse function of the metadata of the torrent files.
			 * @param data      Data of the file to be parsed.
			 * @return          const QString&
			 */
			bool parse(const QByteArray &data);

			/*!
			 * @brief errorString       Getter function of the error string.
			 * @return                  const QString&
			 */
			const QString &errorString() const;

			QByteArray infoValue() const;

			QString announceUrl() const;

			QStringList announceList() const;

			QDateTime creationDate() const;

			QString comment() const;

			QString createdBy() const;

			/*!
			 * @brief singleFile    Getter function of the metadata of a single torrent file.
			 * @return              MetaInfoSingleFile
			 */
			MetaInfoSingleFile singleFile() const;

			/*!
			 * @brief multiFiles    Getter function of the metadata of multiple torrent files.
			 * @return              QList<MetaInfoMultiFile>
			 */
			QList<MetaInfoMultiFile> multiFiles() const;

			/*!
			 * @brief name      Getter function of the metadata name.
			 * @return          QString
			 */
			QString name() const;

			/*!
			 * @brief pieceLength   Getter function of the piece length.
			 * @return              int
			 */
			int pieceLength() const;


			QList<QByteArray> sha3Sums() const;

			/*!
			 * @brief totalSize     Total size of the metadata.
			 * @return              64 bit integer
			 */
			qint64 totalSize() const;

		private:
			QString _ErrorString;
			QByteArray _Content;
			QByteArray _InfoData;

			FileForm _MetaInfoFileForm;
			MetaInfoSingleFile _MetaInfoSingleFile;
			QList<MetaInfoMultiFile> _MetaInfoMultiFiles;

			QString _MetaInfoAnnounce;
			QStringList _MetaInfoAnnounceList;
			QDateTime _MetaInfoCreationDate;
			QString _MetaInfoComment;
			QString _MetaInfoCreatedBy;
			QString _MetaInfoName;
			int _MetaInfoPieceLength;
			QList<QByteArray> _MetaInfoSha3Sums;
	};
}


#endif //UTORRENT_METAINFORMATION_H
