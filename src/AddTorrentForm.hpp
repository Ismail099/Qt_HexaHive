//
// Created by Haidar Ibo on 13.02.2023.
//

#ifndef UTORRENT_ADDTORRENTFORM_HPP
#define UTORRENT_ADDTORRENTFORM_HPP

#include <QDialog>

#include "ui_AddTorrentForm.h"


namespace uTorrent {

	QT_BEGIN_NAMESPACE
	namespace Ui { class AddTorrentForm; }
	QT_END_NAMESPACE

	class AddTorrentDialog : public QDialog {

			Q_OBJECT

		public:
			explicit AddTorrentDialog(QWidget *parent = nullptr);



		private:
			Ui_AddTorrentFile _mUi;
			QString _mDestinationDirectory;
			QString _mLastDirectory;
			QString _mLastDestinationDirectory;

	};
} // uTorrent

#endif //UTORRENT_ADDTORRENTFORM_HPP
