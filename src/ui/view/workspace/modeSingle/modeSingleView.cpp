#include "modeSingleView.hpp"

#include "core/controller/controllerSingle.hpp"

namespace firmwareUpdater::ui::view::workspace::modeSingle
{

SingleModeView::SingleModeView(QWidget* parent)
	: QFrame{ parent }
{	
	// Main layout
	_layout.setContentsMargins(0, 0, 0, 0);
	_layout.setSpacing(0);

	// ModeZone
	_layout.addWidget(&_modeZone);

	// Stacked layout
	_stackedWidget.setContentsMargins(0, 0, 0, 0);
	//_stackedWidget.setSpacing(0);
	_stackedWidget.addWidget(&_projectZoneSingle);
	_stackedWidget.addWidget(&_templateInfoWidget);
	_layout.addWidget(&_stackedWidget);

	// Commander
	_layout.addWidget(&_commander);

	// Main window properties
	setContentsMargins(0, 0, 0, 0);

	connect(&_modeZone, &ModeZone::modeSwitched, this, &SingleModeView::modeSwitched);
	connect(&_commander, &commander::Commander::uploadButtonClicked, this, [this]()
	{
		_controller->onStartButtonClicked();
	});
	connect(&_projectZoneSingle, &projectZone::ProjectZoneSingle::deviceListItemChanged, this, [this](auto const& deviceUserName)
	{
		_controller->onDeviceListItemClicked(deviceUserName);
	});
	connect(&_projectZoneSingle, &projectZone::ProjectZoneSingle::templateIndexChanged, this, [this](auto index)
	{
		_templateInfoWidget.setTemplateInfo(index, _templateInfoList[index]);
	});
	connect(&_templateInfoWidget, &templateInfo::TemplateInfo::stepsEnded, this, [this]
	{
		_controller->onStepsEnded();
	});
	connect(&_templateInfoWidget, &templateInfo::TemplateInfo::stepsCancelled, this, [this]
	{
		_controller->onStepsCancelled();
	});
}

void SingleModeView::setController(core::controller::ControllerSingle& controller)
{
	_controller = &controller;
}

void SingleModeView::resetView()
{
	QMetaObject::invokeMethod(this, [this]
	{
		// Reset all cache and view 
		// (but not the controller which is valid during the whole runtime)
		_templateInfoList.clear();
		_imageCacheManager.clear();
		QSignalBlocker lock{ &_stackedWidget };
		_stackedWidget.setCurrentIndex(0);
		QSignalBlocker lock1{ &_projectZoneSingle };
		_projectZoneSingle.resetView();
		QSignalBlocker lock2{ &_templateInfoWidget };
		_templateInfoWidget.resetView();
		QSignalBlocker lock3{ &_commander };
		_commander.resetView();
	});
}

void SingleModeView::setCurrentView(ViewType const viewType)
{
	QMetaObject::invokeMethod(this, [this, viewType]
	{
		QSignalBlocker lock{ &_stackedWidget };
		_stackedWidget.setCurrentIndex(viewType == ViewType::ProjectZoneSingle ? 0 : 1);
	}, Qt::QueuedConnection);
}

void SingleModeView::setUploadButtonState(core::controller::workspace::UploadButtonState const uploadButtonState)
{
	QMetaObject::invokeMethod(this, [this, uploadButtonState]
	{
		QSignalBlocker lock{ &_commander};
		_commander.setUploadButtonState(uploadButtonState);
	}, Qt::QueuedConnection);
}

void SingleModeView::setTemplateInfoList(core::type::TemplateInfoList const& templateInfoList)
{
	QMetaObject::invokeMethod(this, [this, templateInfoList]
	{
		_templateInfoList = templateInfoList;

		// Update Porject zone view
		QSignalBlocker lock1{ &_projectZoneSingle };
		auto templateNameList = core::type::TemplateNameList(_templateInfoList.size());
		for (auto i = 0u; i < _templateInfoList.size(); ++i)
			templateNameList[i] = _templateInfoList[i].name;
		_projectZoneSingle.setTemplateNameList(templateNameList);
		
		// Update image cache
		QSignalBlocker lock2{ &_imageCacheManager };
		_imageCacheManager.prepareImageCache(_templateInfoList);
		
		// Update Template info view
		QSignalBlocker lock3{ &_templateInfoWidget };
		auto const templateIndex = 0;
		auto const templateInfo = _templateInfoList.size() ? _templateInfoList.front() : core::type::TemplateInfo{};
		_templateInfoWidget.setTemplateInfo(templateIndex, templateInfo);
	});
}

void SingleModeView::startProcessing()
{
	QMetaObject::invokeMethod(this, [this]
	{
		QSignalBlocker lock{ &_templateInfoWidget };
		_templateInfoWidget.startProcessing();
	});
}

void SingleModeView::addDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		QSignalBlocker lock{ &_projectZoneSingle };
		_projectZoneSingle.addDeviceUserName(deviceUserName);
	});
}

void SingleModeView::removeDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		QSignalBlocker lock{ &_projectZoneSingle };
		_projectZoneSingle.removeDeviceUserName(deviceUserName);
	});
}

void SingleModeView::selectDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		QSignalBlocker lock{ &_projectZoneSingle };
		_projectZoneSingle.selectDeviceUserName(deviceUserName);
	});
}

} // namespace firmwareUpdater::ui::view::workspace::modeSingle
