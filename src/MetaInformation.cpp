/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Meta Information.
 * @details
 */
#include <MetaInformation.h>
#include <BencodeParser.h>

#include <QDateTime>
#include <QMetaType>
#include <QString>


namespace uTorrent {

	// Constructor
	MetaInformation::MetaInformation() {
		clear();
	}


	// Clear
	void MetaInformation::clear() {
		_ErrorString = "Unknown error";
		_Content.clear();
		_InfoData.clear();
		_MetaInfoMultiFiles.clear();
		_MetaInfoAnnounce.clear();
		_MetaInfoAnnounceList.clear();
		_MetaInfoCreationDate = QDateTime();
		_MetaInfoComment.clear();
		_MetaInfoCreatedBy.clear();
		_MetaInfoName.clear();
		_MetaInfoPieceLength = 0;
		_MetaInfoSha3Sums.clear();
	}


	// Parse
	bool MetaInformation::parse(const QByteArray &data) {
		clear();
		_Content = data;


		// Parse the data
		BencodeParser parser;
		if (!parser.parse(_Content)) {
			_ErrorString = parser.errorString();
			return false;
		}

		_InfoData = parser.infoSection();

		// Get the dictionary 'info'
		QMap<QByteArray, QVariant> dict = parser.dictionary();
		if (!dict.contains("info"))
			return false;


		// Parse files
		QMap<QByteArray, QVariant> info = qvariant_cast<Dictionary>(dict.value("info"));
		if (info.contains("files")) {
			_MetaInfoFileForm = MultiFileForm;

			// Get the files
			QList<QVariant> files = info.value("files").toList();
			for (const auto &i: files) {

				// Get the list of strings corresponding to subdirectory names.
				const auto file = qvariant_cast<Dictionary>(i);
				const auto pathElements = file.value("path").toList();

				QByteArray path;
				for (const QVariant &p: pathElements) {
					if (!path.isEmpty()) {
						path += '/';
					}
					path += p.toByteArray();
				}


				MetaInfoMultiFile multiFile;
				multiFile.length = file.value("length").toLongLong();
				multiFile.path = QString::fromUtf8(path);
				multiFile.checksum = file.value("sha3sum").toByteArray();

				_MetaInfoMultiFiles << multiFile;
			}

			_MetaInfoName = QString::fromUtf8(info.value("name").toByteArray());
			_MetaInfoPieceLength = info.value("piece length").toInt();
			QByteArray pieces = info.value("pieces").toByteArray();

			for (int i = 0; i < pieces.size(); i += 20) {
				_MetaInfoSha3Sums << pieces.mid(i, 20);
			}
		}
		else if (info.contains("length")) {
			_MetaInfoFileForm = SingleFileForm;
			_MetaInfoSingleFile.length = info.value("length").toLongLong();
			_MetaInfoSingleFile.checksum = info.value("sha3sum").toByteArray();
			_MetaInfoSingleFile.name = QString::fromUtf8(info.value("name").toByteArray());
			_MetaInfoSingleFile.pieceLength = info.value("piece length").toInt();

			QByteArray pieces = info.value("pieces").toByteArray();
			for (int i = 0; i < pieces.size(); i += 20) {
				_MetaInfoSingleFile.sha3sum << pieces.mid(i, 20);
			}
		}


		_MetaInfoAnnounce = QString::fromUtf8(dict.value("announce").toByteArray());
		if (dict.contains("announce-list")) {
			throw UnImplementedException("Unimplemented Meta Information: announce-list");
		}

		if (dict.contains("creation date")) {
			_MetaInfoCreationDate.setSecsSinceEpoch(dict.value("creation date").toInt());
		}
		if (dict.contains("comment")) {
			_MetaInfoComment = QString::fromUtf8(dict.value("comment").toByteArray());
		}
		if (dict.contains("created by")) {
			_MetaInfoCreatedBy = QString::fromUtf8(dict.value("created by").toByteArray());
		}

		return true;
	}


	// Getter Error String
	const QString &MetaInformation::errorString() {
		return _ErrorString;
	}


	// Getter Info Value
	QByteArray MetaInformation::infoValue() const {
		return _InfoData;
	}


	// Getter Announce Url
	QString MetaInformation::announceUrl() const {
		return _MetaInfoAnnounce;
	}


	// Getter Announce List
	QStringList MetaInformation::announceList() const {
		return _MetaInfoAnnounceList;
	}


	// Getter Creation Date
	QDateTime MetaInformation::creationDate() const {
		return _MetaInfoCreationDate;
	}


	// Comment
	QString MetaInformation::comment() const {
		return _MetaInfoComment;
	}


	// Created By
	QString MetaInformation::createdBy() const {
		return _MetaInfoCreatedBy;
	}


	// Single File
	MetaInfoSingleFile MetaInformation::singleFile() const {
		return _MetaInfoSingleFile;
	}


	// Multi Files
	QList<MetaInfoMultiFile> MetaInformation::multiFiles() const {
		return _MetaInfoMultiFiles;
	}


	// Name
	QString MetaInformation::name() const {
		return _MetaInfoName;
	}


	// Piece Length
	int MetaInformation::pieceLength() const {
		return _MetaInfoPieceLength;
	}


	// SHA3 Sums
	QList<QByteArray> MetaInformation::sha3Sums() const {
		return _MetaInfoSha3Sums;
	}


	// Total Size
	qint64 MetaInformation::totalSize() const {
		if (fileForm() == SingleFileForm)
			return singleFile().length;

		qint64 size = 0;
		for (const MetaInfoMultiFile &file : _MetaInfoMultiFiles)
			size += file.length;

		return size;
	}


	// File Form
	MetaInformation::FileForm MetaInformation::fileForm() const {
		return _MetaInfoFileForm;
	}
}
