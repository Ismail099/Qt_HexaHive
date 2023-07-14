//
// Created by hedar on 13.07.2023.
//

#ifndef UTORRENT_STYLESHEET_HPP
#define UTORRENT_STYLESHEET_HPP

#include <QTextStream>
#include <QFile>

QTextStream styleSheet() {
	Q_INIT_RESOURCE(breeze_themes);
	QFile file(":/dark_green/stylesheet.qss");
	file.open(QFile::ReadOnly | QFile::Text);
	return QTextStream(&file);
}

#endif //UTORRENT_STYLESHEET_HPP
