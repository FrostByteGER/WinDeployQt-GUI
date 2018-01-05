#pragma once

#include "ui_WinDeployQtGUI.h"

namespace WinDeployQt::Gui
{
	enum BuildMode
	{
		Release,
		Debug
	};

	enum PrintType
	{
		None,
		Source,
		Target,
		Relative,
		Mapping
	};

	class WinDeployQtGUI : public QMainWindow
	{
		Q_OBJECT

	public:
		explicit WinDeployQtGUI(QWidget *parent = Q_NULLPTR);

		QString winDeployQtExePath = "C:\\Qt\\5.10.0\\msvc2017_64\\bin\\windeployqt.exe";
		QString qtPath;

		QString binaryDir;
		QString dir;
		QString libDir;
		QString pluginDir;
		QString qmlImportsDir;

		BuildMode mode = Release;
		PrintType printType = None;

	public slots:
		void onDryRunClicked();
		void onDeployClicked();
		static std::string getLastErrorAsString();


	private:
		Ui::WinDeployQtGUIClass ui{};
	};
}
