#include "dataDownloader.hpp"

namespace firmwareUpdater::ui::toolkit
{

DataDownloader::DataDownloader(QUrl imageUrl, QObject* parent)
	: QObject(parent)
{
	connect(&_networkManager, &QNetworkAccessManager::finished, this, [this](auto* reply)
	{
		auto data = reply->readAll();
		reply->deleteLater();
		emit downloaded(data);
	});
	_networkManager.get(QNetworkRequest{ imageUrl });
}

} // namespace firmwareUpdater::ui::toolkit
