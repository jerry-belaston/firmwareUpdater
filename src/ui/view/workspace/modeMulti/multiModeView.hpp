#pragma once

#include <QFrame>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "core/controller/viewInterface.hpp"
#include "ui/view/workspace/common/modeZone.hpp"
#include "ui/view/workspace/common/imageCacheManager.hpp"
#include "ui/view/workspace/common/templateInfo/templateInfo.hpp"
#include "projectZone/projectZoneMulti.hpp"

namespace firmwareUpdater::ui::view::workspace::modeMulti
{

class MultiModeView
	: public QFrame
	, public core::controller::workspace::MultiModeViewInterface
{
	Q_OBJECT

public:
	explicit MultiModeView(QWidget* parent = nullptr);
	virtual ~MultiModeView();

	// ViewInterface overrides
	void setController(core::controller::ControllerMulti& controller) override;
	void resetView() override;
	void setTemplateInfoList(core::type::TemplateInfoList const& templateInfoList) override;
	void addDeviceUserName(core::type::Device::UserName const& deviceUserName) override;
	void removeDeviceUserName(core::type::Device::UserName const& deviceUserName) override;
	void setUploadButtonState(core::type::Device::UserName const& deviceUserName,
		core::controller::workspace::UploadButtonState const /*uploadButtonState*/) override;
	void startProcessing(core::type::Device::UserName const& deviceUserName) override;
	void stopProcessing(core::type::Device::UserName const& /*deviceUserName*/) override;
	void setProgress(core::type::Device::UserName const& /*deviceUserName*/, double const ratio) override;

	Q_SIGNAL void modeSwitched(bool isModeMulti);

private:
	void updateMainStackedWidget();

private:

	ImageCacheManager _imageCacheManager{ this };
	core::type::TemplateInfoList _templateInfoList{};

	QVBoxLayout _layout{ this };
	ModeZone _modeZone{ true, this };
	QStackedWidget _mainStackedWidget{ this };
	QLabel _emptyLabel{ &_mainStackedWidget };
	projectZone::ProjectZoneMulti _projectZoneMulti{ &_mainStackedWidget };
	QStackedWidget _templateInfoStackedWidget{ &_mainStackedWidget };

	// For what we're doing, unordered_map should be prefered here...
	std::map<core::type::Device::UserName, templateInfo::TemplateInfo*> _deviceTemplateInfoMap{};

	// Controller
	core::controller::ControllerMulti* _controller{ nullptr };
};

} // namespace firmwareUpdater::ui::view::workspace::modeMulti
