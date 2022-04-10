#pragma once

#include <memory>

namespace firmwareUpdater::core::model
{
class Server;
}

namespace firmwareUpdater::core::controller
{

class MainViewInterface;
class WelcomeViewInterface;
class TemplateBrowserViewInterface;
class TemplateInfoViewInterface;
class Controller
{
public:
	using UniquePointer = std::unique_ptr<Controller>;

	// Construction
	static UniquePointer create(model::Server& server);
	virtual ~Controller();

	// View initialization
	void setView(MainViewInterface& mainViewInterface);
	void setView(WelcomeViewInterface& welcomeViewInterface);
	void setView(TemplateBrowserViewInterface& templateBrowserViewInterface);
	void setView(TemplateInfoViewInterface& templateInfoViewInterface);

	// WelcomeView callbacks
	void onStartButtonClicked();

	// TemplateBrowserView callbacks
	void onListItemClicked(std::uint32_t const itemIndex);

	// TemplateInfoView callbacks
	void onStepsEnded();
	void onStepsCancelled();

private:
	// Constructor disabled (use create)
	Controller(model::Server& server);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::controller
