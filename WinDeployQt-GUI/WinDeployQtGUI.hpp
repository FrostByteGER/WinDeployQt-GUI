#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_WinDeployQtGUI.h"


namespace WinDeployQt::Gui
{
	class WinDeployQtGUI : public QMainWindow
	{
		Q_OBJECT

	public:
		WinDeployQtGUI(QWidget *parent = Q_NULLPTR);

	private:
		Ui::WinDeployQtGUIClass ui;
	};
}
