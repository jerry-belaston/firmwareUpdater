#pragma once

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMetaMethod>

namespace firmwareUpdater::ui::toolkit
{

class DataDownloader : public QObject
{
	Q_OBJECT
public:
	explicit DataDownloader(QUrl imageUrl, QObject* parent = 0);
	void connectNotify(const QMetaMethod& signal) override;

signals:
	void downloaded(QByteArray const data);

private:
	QNetworkAccessManager _networkManager;
	QByteArray _data;
	bool _downloaded{ false };
};

} // namespace firmwareUpdater::ui::toolkit