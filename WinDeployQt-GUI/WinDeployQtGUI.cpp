#include "WinDeployQtGUI.hpp"
#include "Windows.h"
#include <iostream>

WinDeployQt::Gui::WinDeployQtGUI::WinDeployQtGUI(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);


	connect(ui.btnDryRun, &QPushButton::clicked, this, &WinDeployQtGUI::onDryRunClicked);
	connect(ui.btnDeploy, &QPushButton::clicked, this, &WinDeployQtGUI::onDeployClicked);
}

void WinDeployQt::Gui::WinDeployQtGUI::onDryRunClicked()
{
}

void WinDeployQt::Gui::WinDeployQtGUI::onDeployClicked()
{
	QString commands = QString(" \"C:\\Users\\kevin.kuegler\\Desktop\\KAUTORelease\" --release --force --no-system-d3d-compiler --no-quick-import");

	const LPWSTR commandsConverted = new wchar_t[commands.length() + 1];
	const int length = commands.toWCharArray(commandsConverted);
	commandsConverted[length] = 0;

	const LPCWSTR path = reinterpret_cast<LPCWSTR>(winDeployQtExePath.utf16());
	

	PROCESS_INFORMATION processInfo;
	STARTUPINFO startupInfo;
	GetStartupInfo(&startupInfo);

	const bool createResult = CreateProcess(path, commandsConverted, nullptr, nullptr, false, 0, nullptr, nullptr, &startupInfo, &processInfo);
	if(!createResult) std::cout << getLastErrorAsString();
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