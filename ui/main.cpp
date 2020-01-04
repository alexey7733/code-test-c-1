#include "main.h"
#include "ui.h"
#include "eng.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	SymbioUi ui;
	ui.show();

	return app.exec();
}
