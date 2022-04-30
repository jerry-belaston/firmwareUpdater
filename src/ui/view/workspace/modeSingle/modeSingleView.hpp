#pragma once

#include <QFrame>
#include <QWidget>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "core/controller/viewInterface.hpp"
#include "ui/view/workspace/common/modeZone.hpp"
#include "ui/view/workspace/common/imageCacheManager.hpp"
#include "ui/view/workspace/common/templateInfo/templateInfo.hpp"
#include "commander/commander.hpp"
#include "projectZone/projectZoneSingle.hpp"

namespace firmwareUpdater::ui::view::workspace::modeSingle
{

class SingleModeView 
	: public QFrame
	, public core::controller::workspace::SingleModeViewInterface
{
	Q_OBJECT

public:
	explicit SingleModeView(QWidget* parent = nullptr);

	// ViewInterface overrides
	void setController(core::controller::ControllerSingle& controller) override;
	void resetView() override;
	void setCurrentView(ViewType const viewType) override;
	void setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState) override;
	void setTemplateInfoList(core::type::TemplateInfoList const& templateInfoList) override;
	void startProcessing() override;
	void addDeviceUserName(core::type::Device::UserName const& deviceUserName) override;
	void removeDeviceUserName(core::type::Device::UserName const& deviceUserName) override;
	void selectDeviceUserName(core::type::Device::UserName const& deviceUserName) override;

public:
	Q_SIGNAL void modeSwitched(bool isModeMulti);

private:

	ImageCacheManager _imageCacheManager{ this };
	core::type::TemplateInfoList _templateInfoList{};

	// Single Mode Widgets
	QVBoxLayout _layout{ this };
	ModeZone _modeZone{ false, this };
	QStackedWidget _stackedWidget{ this };
	projectZone::ProjectZoneSingle _projectZoneSingle{ &_stackedWidget };
	templateInfo::TemplateInfo _templateInfoWidget{ _imageCacheManager, &_stackedWidget };
	commander::Commander _commander{ this };

	// Controller
	core::controller::ControllerSingle* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::workspace::modeSingle
