#include "WinDeployQtGUI.hpp"
#include <QtWidgets/QApplication>

#ifndef PRINT_COMPILER
#define PRINT_COMPILER
#endif


int main(int argc, char *argv[])
{
#ifdef PRINT_COMPILER
	printf("MSC_VER : %d \n", _MSC_VER);
	printf("MSC_FULL_VER : %d \n", _MSC_FULL_VER);
	#ifdef _MSVC_LANG
		printf("MSVC_LANG : C++%d \n", (_MSVC_LANG / 100) % 2000);
	#endif
#endif

	QApplication app(argc, argv);

	WinDeployQt::Gui::WinDeployQtGUI w;
	w.show();
	return app.exec();
}

