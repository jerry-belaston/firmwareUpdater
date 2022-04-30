#include "discovery.hpp"
#include "core/model/private/taskManager.hpp"
#include "private/deviceSerialization.hpp"
#include "private/defaultDevices.hpp"

#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QJsonDocument>

auto constexpr DeviceFilenamePrefix = "device_";
auto constexpr DeviceFlenameExtension = "xml";
auto const DeviceFilenameFilter = QString{ DeviceFilenamePrefix } + "*." + DeviceFlenameExtension;

namespace firmwareUpdater::core::model::device
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Discovery::PImpl
{
public:
	PImpl(Discovery& parent, firmwareUpdater::core::model::priv::TaskManager& taskManager);

	void createDeviceFilesIfNonExisting();
	void retrieveDevicesIfChanged();

	static QFileInfoList getDeviceFiles();

	// Members
	Discovery& _parent;
	firmwareUpdater::core::model::priv::TaskManager& _taskManager;
	DeviceListChangeHandler _onDeviceListChanged{};
	type::DeviceList _currentDeviceList{};
};

Discovery::PImpl::PImpl(Discovery& parent, firmwareUpdater::core::model::priv::TaskManager& taskManager)
	: _parent{ parent }
	, _taskManager{ taskManager }
{
}

void Discovery::PImpl::createDeviceFilesIfNonExisting()
{
	auto const deviceFilePaths = getDeviceFiles();

	// Do nothing if file already exists
	if (!deviceFilePaths.isEmpty())
		return;

	for (auto const& deviceInfo : priv::DefaultDeviceList)
	{
		auto deviceFile = QFile{ QCoreApplication::applicationDirPath() + QDir::separator() +
			DeviceFilenamePrefix + QString::fromStdString(deviceInfo.userName) + "." + DeviceFlenameExtension};

		// Stop processing if not possible to write
		if (!deviceFile.open(QIODevice::WriteOnly))
			continue;

		// Create file if non existing
		auto xmlWriter = QXmlStreamWriter{ &deviceFile };
		priv::deviceSerialization::save(deviceInfo, xmlWriter);
	}
}

void Discovery::PImpl::retrieveDevicesIfChanged()
{
	auto const deviceFilePaths = getDeviceFiles();

	auto newDeviceList = type::DeviceList{};
	for (auto const& deviceFileInfo : deviceFilePaths)
	{
		auto deviceFile = QFile{ deviceFileInfo.absoluteFilePath() };
		if (deviceFile.open(QIODevice::ReadOnly))
		{
			auto xmlReader = QXmlStreamReader{ &deviceFile };
			auto const deviceUserName = QString{ deviceFileInfo.baseName() }.remove(0, QString{ DeviceFilenamePrefix }.count());
			newDeviceList.push_back(priv::deviceSerialization::load(deviceUserName.toStdString(), xmlReader));
		}
	}

	// For now we don't check for diff (For now UploadManager is in charge of this job)
	_onDeviceListChanged(newDeviceList);
}

QFileInfoList Discovery::PImpl::getDeviceFiles()
{
	// The templates file is closed to the application excutable, which means that,
	// on MacOS, it is inside the bundle (into <project_name>.app/Contents/MacOS)
	auto const deviceDir = QDir{ QCoreApplication::applicationDirPath() };
	return deviceDir.entryInfoList(QStringList() << DeviceFilenameFilter, QDir::Files);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Discovery
//////////////////////////////////////////////////////////////////////////////////////////////////

Discovery::UniquePointer Discovery::create(firmwareUpdater::core::model::priv::TaskManager& taskManager)
{
	return UniquePointer(new Discovery(taskManager));
}

Discovery::Discovery(firmwareUpdater::core::model::priv::TaskManager& taskManager)
	: _pImpl(std::make_unique<PImpl>(*this, taskManager))
{
}

Discovery::~Discovery()
{
}

void Discovery::start()
{
	_pImpl->createDeviceFilesIfNonExisting();

	// Add task to TaskManager
	_pImpl->_taskManager.addTask([this] 
	{ 
		_pImpl->retrieveDevicesIfChanged();
	});
}

void Discovery::setDeviceListChangeHandler(DeviceListChangeHandler const& onDeviceListChanged)
{
	_pImpl->_onDeviceListChanged = onDeviceListChanged;
}

} // namespace firmwareUpdater::core::model::device
