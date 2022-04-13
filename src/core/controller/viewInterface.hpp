#pragma once

#include "core/common/type.hpp"
#include "controller.hpp"

namespace firmwareUpdater::core::controller
{

class ViewInterface
{
public:
	virtual void setController(Controller& /*controller*/) {}
};

class MainViewInterface : public ViewInterface
{
public:
	enum class ViewType
	{
		Welcome,
		TemplateList,
		TemplateInfo,
	};
	virtual void setCurrentView(ViewType const viewType) {}
};

class WelcomeViewInterface : public ViewInterface
{
public:
	virtual void setButtonEnabled(bool isEnabled) {}
};

class TemplateBrowserViewInterface : public ViewInterface
{
public:
	virtual void setTemplateNameList(type::TemplateNameList const& templateNameList) {}
};

class TemplateInfoViewInterface : public ViewInterface
{
public:
	// TO FIX: Here by simplicity we send all the model but we should not
	virtual void prepareImageCache(type::TemplateInfoList const& templateInfoList) {}
	virtual void setTemplateInfo(std::uint32_t const templateIndex, type::TemplateInfo const& templateInfo) {}
};

} // namespace firmwareUpdater::core::controller
