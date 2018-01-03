#include "WinDeployQtGUI.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WinDeployQtGUI w;
	w.show();
	return a.exec();
}
