/*!
 * @author      Haidar Ibo
 * @date        19.02.2022
 * @namespace   uTorrent
 * @brief       Meta Information.
 * @details
 */
#include "MetaInformation.h"


namespace uTorrent {

	MetaInformation::MetaInformation() {
		clear();
	}


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


	//
	// Getter function of the error string.
	//
	const QString &MetaInformation::errorString() const {
		return _ErrorString;
	}

	bool MetaInformation::parse(const QByteArray &data) {
		return false;
	}

}
