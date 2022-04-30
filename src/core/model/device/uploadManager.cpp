#include "uploadManager.hpp"
#include "discovery.hpp"
#include <map>

namespace firmwareUpdater::core::model::device
{

namespace helper
{
	template <class Container, class Predicate>
	bool containsItemIf(Container const& items, Predicate&& predicate)
	{
		return std::end(items) != std::find_if(std::begin(items), std::end(items), std::forward<Predicate>(predicate));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class UploadManager::PImpl
{
public:
	PImpl(UploadManager& parent, Discovery& discovery);

	void addUpload(Upload const& upload);
	void removeUpload(Key const& key);
	void updateUpload(Upload const& upload);

	// callbacks
	void onDeviceListChanged(type::DeviceList const& deviceList);

	// Members
	UploadManager& _parent;
	Discovery& _discovery;
	Delegate dummyDelegate;
	Delegate* _delegate{ &dummyDelegate };

	// KeyList and uploadMap must always in line
	KeyList _keyList;
	std::map<Key, device::Upload> _uploadMap; //  For quick access
};

UploadManager::PImpl::PImpl(UploadManager& parent, Discovery& discovery)
	: _parent{ parent }
	, _discovery{ discovery }
{
}

void UploadManager::PImpl::addUpload(Upload const& upload)
{
	auto const key = upload.device.userName;
	_keyList.push_back(key);
	_uploadMap[key] = upload;
	_delegate->onUploadAdded(upload);
}

void UploadManager::PImpl::removeUpload(Key const& key)
{
	if (auto it = std::find(std::begin(_keyList), std::end(_keyList), key); it != std::end(_keyList))
	{
		_keyList.erase(it);
		_uploadMap.erase(key);
		_delegate->onUploadRemoved(key);
	}
}

void UploadManager::PImpl::updateUpload(Upload const& upload)
{
	_uploadMap[upload.device.userName] = upload;
	_delegate->onUploadChanged(upload);
}

void UploadManager::PImpl::onDeviceListChanged(type::DeviceList const& newDeviceList)
{
	// First remove all devices which disapeared
	// (For each one of our uploads, check it existency in new device list)
	KeyList uploadsToRemove;
	for (auto const& key : _keyList)
	{
		if (!helper::containsItemIf(newDeviceList, [&key](auto const& newDevice) { return key == newDevice.userName; }))
			uploadsToRemove.push_back(key);
	}
	for (auto const& key : uploadsToRemove)
		removeUpload(key);

	// Now check for upload change and addition
	UploadList uploadsToAdd;
	for (auto const& newDevice : newDeviceList)
	{
		// Check for change
		if (auto it = _uploadMap.find(newDevice.userName); it != std::end(_uploadMap))
		{
			if (it->second.device != newDevice)
				updateUpload({ .device = newDevice });
			continue;
		}

		// Add the upload
		uploadsToAdd.push_back({ .device = newDevice });
	}
	for (auto const& upload : uploadsToAdd)
		addUpload(upload);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// UploadManager
//////////////////////////////////////////////////////////////////////////////////////////////////

UploadManager::UniquePointer UploadManager::create(Discovery& discovery)
{
	return UniquePointer(new UploadManager(discovery));
}

UploadManager::UploadManager(Discovery& discovery)
	: _pImpl(std::make_unique<PImpl>(*this, discovery))
{
	_pImpl->_discovery.setDeviceListChangeHandler([this](type::DeviceList const& deviceList)
	{
		_pImpl->onDeviceListChanged(deviceList);
	});
}

UploadManager::~UploadManager()
{
}

void UploadManager::reset()
{
	// Just reset current model will cause a full diff... ;-)
	_pImpl->_keyList = {};
	_pImpl->_uploadMap = {};
}

void UploadManager::setDelegate(Delegate* delegate)
{
	_pImpl->_delegate = delegate ? delegate : &_pImpl->dummyDelegate;
}

UploadManager::KeyList const& UploadManager::getKeyList() const
{
	return _pImpl->_keyList;
}

void UploadManager::setUploadTemplate(Key const& key, std::uint32_t const templateIndex)
{
	if (auto it = _pImpl->_uploadMap.find(key); it != std::end(_pImpl->_uploadMap))
		it->second.templateIndex = templateIndex;
}

void UploadManager::setUploadState(Key const& key, bool const isStarted)
{
	if (auto it = _pImpl->_uploadMap.find(key); it != std::end(_pImpl->_uploadMap))
		it->second.isStarted = isStarted;
}

} // namespace firmwareUpdater::core::model::device
