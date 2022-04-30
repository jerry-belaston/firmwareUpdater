#include "multiModeView.hpp"

#include <QHBoxLayout>

#include "core/controller/controllerMulti.hpp"
#include "ui/toolkit/dataDownloader.hpp"
#include "ui/view/workspace/common/templateInfo/templateInfo.hpp"

namespace firmwareUpdater::ui::view::workspace::modeMulti
{


MultiModeView::MultiModeView(QWidget* parent)
	: QFrame{ parent }
{
	// Main layout
	_layout.setContentsMargins(0, 0, 0, 0);
	_layout.setSpacing(0);

	// ModeZone
	_layout.addWidget(&_modeZone);

	// Empty label
	_emptyLabel.setText("No connected device");
	_emptyLabel.setAlignment(Qt::AlignCenter);
	_emptyLabel.setObjectName("MultiModeView-EmptyLabel");
	_mainStackedWidget.addWidget(&_emptyLabel);

	// ProjectZone and templateInfoStackedWidget
	auto* frame = new QFrame{};
	auto* hlayout = new QHBoxLayout{ this };
	hlayout->addWidget(&_projectZoneMulti);
	hlayout->addWidget(&_templateInfoStackedWidget);
	frame->setLayout(hlayout);
	_mainStackedWidget.addWidget(frame);
	_mainStackedWidget.setCurrentIndex(0);
	
	_layout.addWidget(&_mainStackedWidget);

	// Main window properties
	setContentsMargins(0, 0, 0, 0);

	connect(&_modeZone, &ModeZone::modeSwitched, this, &MultiModeView::modeSwitched);
	connect(&_projectZoneMulti, &projectZone::ProjectZoneMulti::deviceUploadButtonClicked, this, 
		[this](std::string deviceUserName)
		{
			if (_controller)
				_controller->onStartButtonClicked(deviceUserName);
		});
	connect(&_projectZoneMulti, &projectZone::ProjectZoneMulti::deviceTemplateIndexChanged, this,
		[this](std::string deviceUserName, int index)
		{
			if (auto it = _deviceTemplateInfoMap.find(deviceUserName); it != std::end(_deviceTemplateInfoMap))
			{
				auto* templateInfoWidget = it->second;
				templateInfoWidget->setTemplateInfo(index, _templateInfoList[index]);
			}
		});
	connect(&_projectZoneMulti, &projectZone::ProjectZoneMulti::deviceSelectionChanged, this,
		[this](std::string deviceUserName)
		{
			if (auto it = _deviceTemplateInfoMap.find(deviceUserName); it != std::end(_deviceTemplateInfoMap))
			{
				auto* templateInfoWidget = it->second;
				_templateInfoStackedWidget.setCurrentWidget(templateInfoWidget);
			}
		});
}

MultiModeView::~MultiModeView()
{
}

void MultiModeView::setController(core::controller::ControllerMulti& controller)
{
	_controller = &controller;
}

void MultiModeView::resetView()
{
	QMetaObject::invokeMethod(this, [this]
	{
		// Reset all cache and view 
		// (but not the controller which is valid during the whole runtime)
		_templateInfoList.clear();
		_imageCacheManager.clear();
		_deviceTemplateInfoMap.clear();
		QSignalBlocker lock{ &_mainStackedWidget };
		_mainStackedWidget.setCurrentIndex(0);
		QSignalBlocker lock1{ &_projectZoneMulti };
		_projectZoneMulti.resetView();
		QSignalBlocker lock2{ &_templateInfoStackedWidget };
		for (auto i = 0; i < _templateInfoStackedWidget.count(); ++i)
		{
			auto* widget = _templateInfoStackedWidget.widget(i);
			_templateInfoStackedWidget.removeWidget(widget);
			widget->deleteLater();
		}
	});
}

void MultiModeView::setTemplateInfoList(core::type::TemplateInfoList const& templateInfoList)
{
	QMetaObject::invokeMethod(this, [this, templateInfoList]
	{
		_templateInfoList = templateInfoList;

		// Update Project zone view
		QSignalBlocker lock{ &_projectZoneMulti };
		auto templateNameList = core::type::TemplateNameList(_templateInfoList.size());
		for (auto i = 0u; i < _templateInfoList.size(); ++i)
			templateNameList[i] = _templateInfoList[i].name;
		_projectZoneMulti.setTemplateNameList(templateNameList);

		// Update image cache
		QSignalBlocker lock1{ &_projectZoneMulti };
		_imageCacheManager.prepareImageCache(templateInfoList);

		// Update template Info view
		auto const templateIndex = 0;
		auto const templateInfo = _templateInfoList.size() ? _templateInfoList.front() : core::type::TemplateInfo{};
		for (auto const& [_, templateInfoWidget] : _deviceTemplateInfoMap)
		{
			QSignalBlocker lock2{ templateInfoWidget };
			templateInfoWidget->setTemplateInfo(templateIndex, templateInfo);
		}
	});
}

void MultiModeView::addDeviceUserName(core::type::Device::UserName const& deviceUserName)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		// Add device in project zone view
		QSignalBlocker lock{ &_projectZoneMulti };
		_projectZoneMulti.addDeviceUserName(deviceUserName);
		
		// Add new template info view page for this new device
		// By the way, init the view with the first available templateInfo
		QSignalBlocker lock2{ &_templateInfoStackedWidget };
		auto* templateInfoWidget = new templateInfo::TemplateInfo{ _imageCacheManager, &_templateInfoStackedWidget };
		QSignalBlocker lock3{ templateInfoWidget };
		if (_templateInfoList.size())
			templateInfoWidget->setTemplateInfo(0, _templateInfoList.front());
		_deviceTemplateInfoMap[deviceUserName] = templateInfoWidget;
		_templateInfoStackedWidget.addWidget(templateInfoWidget);
		if (_templateInfoStackedWidget.count() == 1)
			_templateInfoStackedWidget.setCurrentIndex(0);

		connect(templateInfoWidget, &templateInfo::TemplateInfo::stepsProgressed, this, [this, deviceUserName](auto ratio)
		{
			_controller->onStepsProgressed(deviceUserName, ratio);
		});
		connect(templateInfoWidget, &templateInfo::TemplateInfo::stepsEnded, this, [this, deviceUserName]()
		{
			_controller->onStepsEnded(deviceUserName);
		});
		connect(templateInfoWidget, &templateInfo::TemplateInfo::stepsCancelled, this, [this, deviceUserName]()
		{
			_controller->onStepsCancelled(deviceUserName);
		});

		updateMainStackedWidget();
	});
}

void MultiModeView::removeDeviceUserName(core::type::Device::UserName const& deviceUserName) 
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		// Remove device from project zone view
		QSignalBlocker lock{ &_projectZoneMulti };
		_projectZoneMulti.removeDeviceUserName(deviceUserName);

		// Remove corresponding template info view 
		QSignalBlocker lock2{ &_templateInfoStackedWidget };
		if (auto it = _deviceTemplateInfoMap.find(deviceUserName); it != std::end(_deviceTemplateInfoMap))
		{
			auto* templateInfoWidget = it->second;
			auto const wasSelected = _templateInfoStackedWidget.currentWidget() == templateInfoWidget;
			_deviceTemplateInfoMap.erase(deviceUserName);
			_templateInfoStackedWidget.removeWidget(templateInfoWidget);
			templateInfoWidget->deleteLater();
			if(wasSelected)
				_templateInfoStackedWidget.setCurrentIndex(_templateInfoStackedWidget.count() ? 0 : -1);
		}

		updateMainStackedWidget();
	});
}

void MultiModeView::setUploadButtonState(core::type::Device::UserName const& deviceUserName,
	core::controller::workspace::UploadButtonState const uploadButtonState)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName, uploadButtonState]
	{
		QSignalBlocker lock{ &_projectZoneMulti };
		_projectZoneMulti.setUploadButtonState(deviceUserName, uploadButtonState);
	});
}

void MultiModeView::setProgress(core::type::Device::UserName const& deviceUserName, double const ratio)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName, ratio]
	{
		QSignalBlocker lock{ &_projectZoneMulti };
		_projectZoneMulti.setProgress(deviceUserName, ratio);
	});
}

void MultiModeView::startProcessing(core::type::Device::UserName const& deviceUserName) 
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		if (auto it = _deviceTemplateInfoMap.find(deviceUserName); it != std::end(_deviceTemplateInfoMap))
		{
			auto* templateInfoWidget = it->second;
			QSignalBlocker lock{ templateInfoWidget };
			templateInfoWidget->startProcessing();
		}
	});
}

void MultiModeView::stopProcessing(core::type::Device::UserName const& deviceUserName)
{
	QMetaObject::invokeMethod(this, [this, deviceUserName]
	{
		if (auto it = _deviceTemplateInfoMap.find(deviceUserName); it != std::end(_deviceTemplateInfoMap))
		{
			auto* templateInfoWidget = it->second;
			QSignalBlocker lock{ templateInfoWidget };
			templateInfoWidget->stopProcessing();
		}
	});
}

void MultiModeView::updateMainStackedWidget()
{
	_mainStackedWidget.setCurrentIndex(_templateInfoStackedWidget.count() ? 1 : 0);
}

} // namespace firmwareUpdater::ui::view::workspace::modeMulti
