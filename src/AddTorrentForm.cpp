//
// Created by hedar on 13.02.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_AddTorrentForm.h" resolved

#include <AddTorrentForm.hpp>
#include "ui_AddTorrentForm.h"

namespace uTorrent {
	AddTorrentDialog::AddTorrentDialog(QWidget *parent) :
		QDialog(parent), ui(reinterpret_cast<Ui::AddTorrentForm *>(new AddTorrentDialog)) {
		ui->setupUi(this);
	}

	AddTorrentDialog::~AddTorrentDialog() {
		delete ui;
	}
} // uTorrent
