#include "WinDeployQtGUI.hpp"
#include "Windows.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <iostream>

WinDeployQt::Gui::WinDeployQtGUI::WinDeployQtGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionQuit, &QAction::triggered, this, &WinDeployQtGUI::onQuitClicked);
	connect(ui.actionSettings, &QAction::triggered, this, &WinDeployQtGUI::onSettingsClicked);
	connect(ui.actionAbout, &QAction::triggered, this, &WinDeployQtGUI::onAboutClicked);

	connect(ui.cbBxBuildMode, SIGNAL(currentIndexChanged(int)), SLOT(onBuildModeChanged(const int)));
	connect(ui.cbBxListPrintOnly, SIGNAL(currentIndexChanged(int)), SLOT(onPrintTypeChanged(const int)));

	connect(ui.btnBinDir, &QPushButton::clicked, this, &WinDeployQtGUI::onBrowseBinDirClicked);
	connect(ui.btnDir, &QPushButton::clicked, this, &WinDeployQtGUI::onBrowseDirClicked);
	connect(ui.btnLibDir, &QPushButton::clicked, this, &WinDeployQtGUI::onBrowseLibTargetDirClicked);
	connect(ui.btnPluginDir, &QPushButton::clicked, this, &WinDeployQtGUI::onBrowsePluginsTargetDirClicked);
	connect(ui.btnQmlDir, &QPushButton::clicked, this, &WinDeployQtGUI::onBrowseQmlImportsTargetDirClicked);

	connect(ui.btnDryRun, &QPushButton::clicked, this, &WinDeployQtGUI::onDryRunClicked);
	connect(ui.btnDeploy, &QPushButton::clicked, this, &WinDeployQtGUI::onDeployClicked);
}

void WinDeployQt::Gui::WinDeployQtGUI::executeCommand() const
{
	//QString commands = QString(" \"C:\\Users\\kevin.kuegler\\Desktop\\KAUTORelease\" --release --force --no-system-d3d-compiler --no-quick-import");

	const LPWSTR commandsConverted = new wchar_t[commandToExecute.length() + 1];
	const int length = commandToExecute.toWCharArray(commandsConverted);
	commandsConverted[length] = 0;

	const LPCWSTR path = reinterpret_cast<LPCWSTR>(winDeployQtExePath.utf16());


	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	GetStartupInfo(&startupInfo);

	const bool createResult = CreateProcess(path, commandsConverted, nullptr, nullptr, false, 0, nullptr, nullptr, &startupInfo, &processInfo);
	if (!createResult) std::cout << getLastErrorAsString();
}

void WinDeployQt::Gui::WinDeployQtGUI::onDryRunClicked()
{
	dryRun = true;
	onCommandUpdate();
	executeCommand();
	dryRun = false;
	onCommandUpdate();
}

void WinDeployQt::Gui::WinDeployQtGUI::onDeployClicked()
{
	dryRun = false;
	qDebug() << commandToExecute;
	executeCommand();
}

void WinDeployQt::Gui::WinDeployQtGUI::onQuitClicked()
{
	QApplication::instance()->quit();
}

void WinDeployQt::Gui::WinDeployQtGUI::onSettingsClicked()
{
}

void WinDeployQt::Gui::WinDeployQtGUI::onAboutClicked()
{
	QMessageBox::about(this, QString(tr("About")), QString(tr("Version 1.00\nMade by Kevin Kuegler")));
}

void WinDeployQt::Gui::WinDeployQtGUI::onBuildModeChanged(int index)
{
	if(index > -1) buildMode = static_cast<BuildMode>(index);
}

void WinDeployQt::Gui::WinDeployQtGUI::onPrintTypeChanged(int index)
{
	if (index > -1) 
		printType = static_cast<PrintType>(index);
}

void WinDeployQt::Gui::WinDeployQtGUI::onBrowseBinDirClicked()
{
	const QString dir = QFileDialog::getExistingDirectory(this, QString(tr("Open Binary Directory")));
	if (dir.isNull() || dir.isEmpty()) return;
	ui.lineEditBinDir->setText(dir);
	binaryDir = dir;
}

void WinDeployQt::Gui::WinDeployQtGUI::onBrowseDirClicked()
{
	const QString dir = QFileDialog::getExistingDirectory(this, QString(tr("Open Directory")));
	if (dir.isNull() || dir.isEmpty()) return;
	ui.lineEditDir->setText(dir);
	this->dir = dir;
}

void WinDeployQt::Gui::WinDeployQtGUI::onBrowseLibTargetDirClicked()
{
	const QString dir = QFileDialog::getExistingDirectory(this, QString(tr("Open Library Directory")));
	if (dir.isNull() || dir.isEmpty()) return;
	ui.lineEditLibDir->setText(dir);
	libDir = dir;
}

void WinDeployQt::Gui::WinDeployQtGUI::onBrowsePluginsTargetDirClicked()
{
	const QString dir = QFileDialog::getExistingDirectory(this, QString(tr("Open Plugins Directory")));
	if (dir.isNull() || dir.isEmpty()) return;
	ui.lineEditPluginDir->setText(dir);
	pluginDir = dir;
}

void WinDeployQt::Gui::WinDeployQtGUI::onBrowseQmlImportsTargetDirClicked()
{
	const QString dir = QFileDialog::getExistingDirectory(this, QString(tr("Open QML-Imports Directory")));
	if (dir.isNull() || dir.isEmpty()) return;
	ui.lineEditQmlDir->setText(dir);
	qmlImportsDir = dir;
}

std::string WinDeployQt::Gui::WinDeployQtGUI::getLastErrorAsString()
{
	//Get the error message, if any.
	const DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return std::string(); //No error message has been recorded

	LPSTR messageBuffer = nullptr;
	const std::size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&messageBuffer), 0, nullptr);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

void WinDeployQt::Gui::WinDeployQtGUI::onCommandUpdate()
{
	/*
	 * Syntax: 
	 * %1 = \"PATH\"
	 * %2 = --dir \"DIR\"
	 * %3 = --libdir \"LIBDIR\"
	 * %4 = --plugindir \"PLUGINDIR\"
	 * %5 = --QMLdir \"QMLDIR\"
	 * %6 = --debug/release
	 * %7 = --pdb
	 * %8 = --force
	 * %9 = --dry-run
	 * %10 = --nopatchqt
	 * %11 = --no-plugins
	 * %12 = --no-libraries
	 * %13 = --no-quick-import
	 * %14 = --no-translations
	 * %15 = --no-system-d3d-compiler
	 * %16 = --compiler-runtime
	 * %17 = --no-compiler-runtime
	 * %18 = --webkit2/--no-webkit2
	 * %20 = --json
	 * %21 = --angle/--no-angle
	 * %22 = --no-opengl-sw
	 * %23 = --list "PRINTTYPE"
	 * %24 = verbose "???"
	 */
	commandToExecute = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16 %17 %18 %19 %20 %21 %22 %23 %24")
		.arg("%1", binaryDir.isEmpty() ? "" : "\"" + binaryDir + "\"")
		.arg("%2", dir.isEmpty() ? "" : "--dir \"" + dir + "\"")
		.arg("%3", libDir.isEmpty() ? "" : "--libdir \"" + libDir + "\"")
		.arg("%4", pluginDir.isEmpty() ? "" : "--plugindir \"" + pluginDir + "\"")
		.arg("%5", qmlImportsDir.isEmpty() ? "" : "--qmldir \"" + qmlImportsDir + "\"")
		.arg("%6", QString(buildMode == Release ? "--release" : "--debug"))
		.arg("%7", QString(ui.ckBxDeployPDB->isChecked() ? "--pdb" : ""))
		.arg("%8", QString(ui.ckBxForceUpdate->isChecked() ? "--force" : ""))
		.arg("%9", QString(dryRun ? "--dry-run" : ""))
		.arg("%10", QString(ui.ckBxPatchCoreLib->isChecked() ? "--no-patchqt" : ""))
		.arg("%11", QString(ui.ckBxSkipPlugins->isChecked() ? "--no-plugins" : ""))
		.arg("%12", QString(ui.ckBxSkipLibs->isChecked() ? "--no-libraries" : ""))
		.arg("%13", QString(ui.ckBxDeployQuickImports->isChecked() ? "--no-quick-import" : ""))
		.arg("%14", QString(ui.ckBxDeployTranslations->isChecked() ? "--no-translations" : ""))
		.arg("%15", QString(ui.ckBxDeployD3DCompiler->isChecked() ? "--no-system-d3d-compiler" : ""))
		.arg("%16", QString(ui.ckBxDeployCompilerRuntime->isChecked() ? "--compiler-runtime" : ""))
		.arg("%17", QString(ui.ckBxSkipCompilerRuntime->isChecked() ? "--no-compiler-runtime" : ""))
		.arg("%18", QString(ui.ckBxDeployWebkit2->isChecked() ? "--webkit2" : ""))
		.arg("%19", QString(ui.ckBxSkipWebkit2->isChecked() ? "--no-webkit2" : ""))
		.arg("%20", QString(ui.ckBxPrintProgressJSON->isChecked() ? "--json" : ""))
		.arg("%21", QString(ui.ckBxForceDeployAngle->isChecked() ? "--angle" : "--no-angle"))
		.arg("%22", QString(ui.ckBxDeployOpenGLSW->isChecked() ? "--no-opengl-sw" : ""))
		.arg("%23", QString(""))
		.arg("%24", QString(""));
	qDebug() << commandToExecute.simplified();
}
