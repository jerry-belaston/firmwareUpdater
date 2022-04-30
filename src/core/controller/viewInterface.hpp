#pragma once

#include "core/common/type.hpp"
#include "controller.hpp"
#include "controllerSingle.hpp"
#include "controllerMulti.hpp"

namespace firmwareUpdater::core::controller
{

enum class PageType
{
	SingleModeWorkspace,
	MultiModeWorkspace,
	TraceLog,
	Settings,
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Controller / ViewInterface
//////////////////////////////////////////////////////////////////////////////////////////////////

class ViewInterface
{
public:
	virtual void setController(Controller& /*controller*/) {}
};

class MainWindowViewInterface : public ViewInterface
{
public:
	virtual ~MainWindowViewInterface() {}
	virtual void setPageType(PageType const& /*pageType*/, bool const /*shouldExpand*/, bool const /*shouldDisplayToolbar*/) {}
};

namespace workspace
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// ControllerSingle / ViewSingleInterface
//////////////////////////////////////////////////////////////////////////////////////////////////

enum class UploadButtonState
{
	On,
	Off,
	Disabled
};

class ViewSingleInterface
{
public:
	virtual void setController(ControllerSingle& /*controllerSingle*/) {}
	virtual void resetView() {}
};

class SingleModeViewInterface : public ViewSingleInterface
{
public:
	virtual ~SingleModeViewInterface() {}
	enum class ViewType
	{
		ProjectZoneSingle,
		TemplateInfo,
	};
	virtual void setCurrentView(ViewType const /*viewType*/) {}
	virtual void setUploadButtonState(UploadButtonState const /*uploadButtonState*/) {}
	// Here by simplicity we send all the model but we should not
	virtual void setTemplateInfoList(type::TemplateInfoList const& /*templateInfoList*/) {}
	virtual void startProcessing() {}
	virtual void addDeviceUserName(type::Device::UserName const& /*deviceUserName*/) {}
	virtual void removeDeviceUserName(type::Device::UserName const& /*deviceUserName*/) {}
	virtual void selectDeviceUserName(type::Device::UserName const& /*deviceUserName*/) {}
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// ControllerMulti / ViewMultiInterface
//////////////////////////////////////////////////////////////////////////////////////////////////

class ViewMultiInterface
{
public:
	virtual void setController(ControllerMulti& /*controllerMulti*/) {}
	virtual void resetView() {}
};

// In multiview, this MultiModeViewInterface class is in charge of the whole 
// controller notifications
class MultiModeViewInterface : public ViewMultiInterface
{
public:
	virtual ~MultiModeViewInterface() {}

	// The whole template list model is provided by simplicity but should not
	virtual void setTemplateInfoList(type::TemplateInfoList const& /*templateInfoList*/) {}
	virtual void addDeviceUserName(type::Device::UserName const& /*deviceUserName*/) {}
	virtual void removeDeviceUserName(type::Device::UserName const& /*deviceUserName*/) {}
	virtual void setUploadButtonState(type::Device::UserName const& /*deviceUserName*/, UploadButtonState const /*uploadButtonState*/) {}
	virtual void setProgress(type::Device::UserName const& /*deviceUserName*/, double const /*ratio*/) {}
	virtual void startProcessing(type::Device::UserName const& /*deviceUserName*/) {}
	virtual void stopProcessing(type::Device::UserName const& /*deviceUserName*/) {}
};

}  // namespace workspace

} // namespace firmwareUpdater::core::controller
