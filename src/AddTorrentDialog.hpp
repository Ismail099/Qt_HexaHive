//
// Created by Haidar Ibo on 13.02.2023.
//

#ifndef UTORRENT_ADDTORRENTDIALOG_HPP
#define UTORRENT_ADDTORRENTDIALOG_HPP

#include <QDialog>
#include "ui_AddTorrentForm.h"

namespace uTorrent {

	QT_BEGIN_NAMESPACE
	namespace Ui { class AddTorrentDialog; }
	QT_END_NAMESPACE

	class AddTorrentDialog : public QDialog {

			Q_OBJECT

		public:
			explicit AddTorrentDialog(QWidget *parent = nullptr);
			// ~AddTorrentDialog();

			QString torrentFileName() const;
			QString destinationFolder() const;

		public slots:
			void setTorrent(const QString &torrentFile);

		private slots:
			void selectTorrent();
			void selectDestination();
			void enableOkButton() const;


		private:
			Ui_AddTorrentFile _mUi;
			QString _mDestinationDirectory;
			QString _mLastDirectory;
			QString _mLastDestinationDirectory;
	};
} // uTorrent

#endif //UTORRENT_ADDTORRENTDIALOG_HPP
