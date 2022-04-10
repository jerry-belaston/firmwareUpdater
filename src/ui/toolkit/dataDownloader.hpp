#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace firmwareUpdater::ui::toolkit
{

class DataDownloader : public QObject
{
	Q_OBJECT
public:
	explicit DataDownloader(QUrl imageUrl, QObject* parent = 0);

signals:
	void downloaded(QByteArray const data);

private:
	QNetworkAccessManager _networkManager;
};

} // namespace firmwareUpdater::ui::toolkit