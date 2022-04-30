#include "qssHelper.hpp"

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace firmwareUpdater::ui::toolkit::qssHelper
{

bool loadQSS(QFile qFile)
{
	if (!qFile.open(QFile::ReadOnly))
		return false;

	qApp->setStyleSheet(qFile.readAll());
	return true;
}

bool loadQSS(QString const& qssFilePath)
{
	return loadQSS(QFile{ qssFilePath} );
}

bool loadProjectQSS()
{
	auto qssFilePath = QString{};
#ifdef Q_OS_MAC
	qssFilePath = ":/main.qss";
#else
	auto const currentFolder = QFileInfo(__FILE__).absoluteDir().absolutePath();
	auto const rootFolder = QDir(currentFolder + "/../../../");
	qssFilePath = rootFolder.absoluteFilePath("res/main.qss");
#endif
	return loadQSS(qssFilePath);
}

} // namespace firmwareUpdater::ui::toolkit::qssHelper
