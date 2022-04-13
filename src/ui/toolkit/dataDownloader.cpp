#include "dataDownloader.hpp"
#include <QDebug>

namespace firmwareUpdater::ui::toolkit
{

DataDownloader::DataDownloader(QUrl imageUrl, QObject* parent)
	: QObject(parent)
{
	connect(&_networkManager, &QNetworkAccessManager::finished, this, [this](auto* reply)
	{
		_data = reply->readAll();
		_downloaded = true;
		reply->deleteLater();
		emit downloaded(_data);
	});
	_networkManager.get(QNetworkRequest{ imageUrl });
}

void DataDownloader::connectNotify(const QMetaMethod& signal)
{
	if (signal == QMetaMethod::fromSignal(&DataDownloader::downloaded)) 
	{
		if (_downloaded)
			emit downloaded(_data);
	}
}

} // namespace firmwareUpdater::ui::toolkit
