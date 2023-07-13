/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Meta Information.
 * @details
 */

#ifndef UTORRENT_METAINFORMATION_H
#define UTORRENT_METAINFORMATION_H


#include <Exceptions/UnImplementedException.hpp>

#include <QByteArray>
#include <QDateTime>
#include <QList>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QVariant>


namespace uTorrent {
	/*!
	 * @brief MetaInfoSingleFile is a struct that represents a file structure of a torrent file.
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


	/*!
	 * @brief class MetaInformation represents the meta information of torrent files.
	 **/
	class MetaInformation {

		public:
			enum FileForm {
				SingleFileForm,
				MultiFileForm,
			};


			/*!
			 * @brief MetaInformation() an explicit constructor.
			 **/
			explicit MetaInformation();


			/*!
			 * @brief clear() member function which clears the meta-information.
			 **/
			void clear();


			/*!
			 * @brief parse     Parse function of the metadata of the torrent files.
			 * @param data      Data of the file to be parsed.
			 * @return          const QString&
			 **/
			bool parse(const QByteArray &data);


			/*!
			 * @brief errorString getter function of the error string.
			 * @return constant reference of QString.
			 **/
			const QString &errorString();


			/*!
			 * @brief infoValue()
			 * @return
			 **/
			QByteArray infoValue() const;


			/*!
			 * @brief announceUrl Getter of the tracker's url.
			 * @return url as a string
			 **/
			QString announceUrl() const;


			/*!
			 * @brief fileForm Getter of File Form.
			 * @return
			 */
			FileForm fileForm() const;


			/*!
			 * @brief announceList Returns a list of tracker urls.
			 * @return list of string.
			 **/
			QStringList announceList() const;


			/*!
			 * @brief creationDate
			 * @return
			 */
			QDateTime creationDate() const;


			/*!
			 * @brief comment
			 * @return
			 */
			QString comment() const;


			/*!
			 * @brief
			 * @return
			 */
			QString createdBy() const;

			/*!
			 * @brief singleFile getter function of the metadata of a single torrent file.
			 * @return MetaInfoSingleFile.
			 **/
			MetaInfoSingleFile singleFile() const;

			/*!
			 * @brief multiFiles getter function of the metadata of multiple torrent files.
			 * @return List of MetaInfoMultiFile.
			 **/
			QList<MetaInfoMultiFile> multiFiles() const;

			/*!
			 * @brief name getter function of the metadata name.
			 * @return QString.
			 **/
			QString name() const;

			/*!
			 * @brief pieceLength   Getter function of the piece length.
			 * @return              int
			 **/
			int pieceLength() const;


			/*!
			 * @brief the checksum with SHA-3 hash algorithm.
			 * @return List of Byte arrays.
			 **/
			QList<QByteArray> sha3Sums() const;

			/*!
			 * @brief totalSize total size of the metadata.
			 * @return 64 bit integer
			 **/
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
