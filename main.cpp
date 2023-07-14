// Cpp
#include <iostream>

// Qt
#include <QApplication>

// Source
#include <src/utils/stylesheet.hpp>
#include <src/BencodeParser.h>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	// Initialize stylesheet
	app.setStyleSheet(styleSheet().readAll());


    return app.exec();
}
