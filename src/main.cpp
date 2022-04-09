#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	// Create Qt application
	QApplication application(argc, argv);

	// Create view
	auto mainWindow = QWidget{};

	// Display view
	mainWindow.show();

	// Enter the QApplication main event loop and wait until exit is called
	return application.exec();
}