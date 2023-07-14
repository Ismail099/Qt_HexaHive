//
// Created by Haidar Ibo on 13.02.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddTorrentForm.h" resolved

#include <AddTorrentDialog.hpp>
#include <QDir>
#include <QFileDialog>
#include "ui_AddTorrentForm.h"
#include "MetaInformation.h"

namespace uTorrent {

	// Constructor
	AddTorrentDialog::AddTorrentDialog(QWidget *parent) : QDialog(parent, Qt::Sheet) {

		_mUi.setupUi(this);

		connect(_mUi._mBrowseTorrents, &QPushButton::clicked, this, &AddTorrentDialog::selectTorrent);
		connect(_mUi._mBrowseDestination, &QPushButton::clicked, this, &AddTorrentDialog::selectDestination);
		connect(_mUi._mTorrentFile, &QLineEdit::textChanged, this, &AddTorrentDialog::setTorrent);

		_mUi._mDestinationFolder->setText(_mDestinationDirectory = QDir::current().path());
		_mUi._mTorrentFile->setFocus();
	}


	// String Number
	static QString stringNumber(qint64 number) {
		if (number > (1024 * 1024 * 1024)) {
			return QString::asprintf("%.2f GB", static_cast<double>(number) / (1024.0 * 1024.0 * 1024.0));
		}
		else if (number > (1024 * 1024)) {
			return QString::asprintf("%.2f MB", static_cast<double>(number) / (1024.0 * 1024.0));
		}
		else if (number > 1024) {
			return QString::asprintf("%.2f KB", static_cast<double>(number) / 1024.0);
		}
		else {
            return QString::asprintf("%d bytes", static_cast<qint64>(number));
        }
	}


	// Select Torrent File
	void AddTorrentDialog::selectTorrent() {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a torrent file"), _mLastDirectory,
														tr("Torrents (*.torrent);; All files (*.*)"));
		if (fileName.isEmpty())
			return;
		_mLastDirectory = QFileInfo(fileName).absolutePath();
		setTorrent(fileName);
	}


	// Select Destination Folder
	void AddTorrentDialog::selectDestination() {
		QString dir = QFileDialog::getExistingDirectory(this, tr("Choose a destination folder"), _mDestinationDirectory);
		if (dir.isEmpty())
			return;
		_mLastDestinationDirectory = _mDestinationDirectory = dir;
		_mUi._mDestinationFolder->setText(_mDestinationDirectory);
		enableOkButton();
	}


	// Enable Ok Button
	void AddTorrentDialog::enableOkButton() const {
		_mUi._mOkButton->setEnabled(!_mUi._mTorrentFile->text().isEmpty() &&!_mUi._mDestinationFolder->text().isEmpty());
	}


	void AddTorrentDialog::setTorrent(const QString &fileName) {
		if (fileName.isEmpty()) {
			enableOkButton();
			return;
		}

		_mUi._mTorrentFile->setText(fileName);
		if (!fileName.isEmpty()) {
			_mLastDirectory = QFileInfo(fileName).absolutePath();
		}

		if (_mLastDestinationDirectory.isEmpty()) {
			_mLastDestinationDirectory = _mDestinationDirectory;
		}

		MetaInformation metaInfo;
		QFile torrent(fileName);
		if (!torrent.open(QIODevice::ReadOnly) || !metaInfo.parse(torrent.readAll())) {
			enableOkButton();
			return;
		}

		_mUi._mTorrentFile->setText(fileName);
		_mUi._mAnnounceUrl->setText(metaInfo.announceUrl());
		if (metaInfo.comment().isEmpty()) {
			_mUi._mCommentLabel->setText(tr("<unknown>"));
		}
		else {
			_mUi._mCommentLabel->setText(metaInfo.comment());
		}

		if (metaInfo.createdBy().isEmpty()) {
			_mUi._mCreatorLabel->setText(tr("<unknown>"));
		}
		else {
			_mUi._mCreatorLabel->setText(metaInfo.createdBy());
		}

		_mUi._mSizeLabel->setText(stringNumber(metaInfo.totalSize()));
		if (metaInfo.fileForm() == MetaInformation::SingleFileForm) {
			_mUi._mTorrentContents->setHtml(metaInfo.singleFile().name);
		}
		else {
			QString html;
			const QList<MetaInfoMultiFile> multiFiles = metaInfo.multiFiles();
			for (const MetaInfoMultiFile &file: multiFiles) {
				QString name = metaInfo.name();
				if (!name.isEmpty()) {
					html += name;
					if (!name.endsWith("/")) {
						html += "/";
					}
				}
				html += file.path + "<br>";
			}
			_mUi._mTorrentContents->setHtml(html);
		}

		QFileInfo info(fileName);
		_mUi._mDestinationFolder->setText(info.absolutePath());

		enableOkButton();
	}


	QString AddTorrentDialog::torrentFileName() const {
		return _mUi._mTorrentFile->text();
	}


	QString AddTorrentDialog::destinationFolder() const {
		return _mUi._mDestinationFolder->text();
	}

} // uTorrent
