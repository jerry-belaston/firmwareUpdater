#pragma once

#include <memory>
#include <functional>
#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::device
{

struct Upload
{
	type::Device device;
	std::uint32_t templateIndex{ 0 };
	bool isStarted{ false };
};

class Discovery;
class UploadManager
{
public:
	using UniquePointer = std::unique_ptr<UploadManager>;
	using UploadList = std::vector<Upload>;
	using Key = type::Device::UserName;
	using KeyList = type::DeviceUserNameList;

	class Delegate
	{
	public:
		virtual void onUploadAdded(Upload const& /*upload*/) {}
		virtual void onUploadRemoved(Key const& /*key*/) {}
		virtual void onUploadChanged(Upload const& /*upload*/) {}
	};

	// Construction
	static UniquePointer create(Discovery& discovery);
	virtual ~UploadManager();
	
	void reset();

	void setDelegate(Delegate* delegate);

	KeyList const& getKeyList() const;

	// Allow to the controller to update the model (no Delegate notification will be triggered)
	void setUploadTemplate(Key const& key, std::uint32_t const templateIndex);
	void setUploadState(Key const& key, bool const isStarted);

private:
	// Constructor disabled (use create)
	UploadManager(Discovery& discovery);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model::device
