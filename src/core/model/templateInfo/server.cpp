#include "server.hpp"
#include "core/model/private/taskManager.hpp"
#include "private/templateSerialization.hpp"
#include "private/defaultTemplates.hpp"

#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>

auto constexpr TemplatesFileName{ "templates.json" };

namespace firmwareUpdater::core::model::templateInfo
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Server::PImpl
{
public:
	PImpl(Server& parent, firmwareUpdater::core::model::priv::TaskManager& taskManager);

	void createTemplatesFileIfNonExisting();
	void retrieveTemplatesIfChanged();

	static QFile getTemplatesFile();

	// Members
	Server& _parent;
	firmwareUpdater::core::model::priv::TaskManager& _taskManager;
	TemplateInfoListChangeHandler _dummyHandler = [](auto const&) {};
	TemplateInfoListChangeHandler _onTemplateInfoListChanged{ _dummyHandler };
	type::TemplateInfoList _currentTemplateInfoList{};
};

Server::PImpl::PImpl(Server& parent, firmwareUpdater::core::model::priv::TaskManager& taskManager)
	: _parent{ parent }
	, _taskManager{ taskManager }
{
}

void Server::PImpl::createTemplatesFileIfNonExisting()
{
	auto templatesFile = getTemplatesFile();

	// Do nothing if file already exists
	if (templatesFile.exists())
		return;

	// Stop processing if not possible to write
	if (!templatesFile.open(QIODevice::WriteOnly))
		return;

	// Create file if non existing
	templatesFile.write(QJsonDocument{ priv::templateSerialization::save(priv::DefaultTemplateInfoList) }.toJson());
}

void Server::PImpl::retrieveTemplatesIfChanged()
{
	auto templatesFile = getTemplatesFile();

	auto newTemplateInfoList = type::TemplateInfoList{};
	if (templatesFile.open(QIODevice::ReadOnly))
		newTemplateInfoList = priv::templateSerialization::load(QJsonDocument::fromJson(templatesFile.readAll()).object());

	if (newTemplateInfoList == _currentTemplateInfoList)
		return;

	_currentTemplateInfoList = newTemplateInfoList;
	_onTemplateInfoListChanged(newTemplateInfoList);
}

QFile Server::PImpl::getTemplatesFile()
{
	// The templates file is closed to the application excutable, which means that,
	// on MacOS, it is inside the bundle (into <project_name>.app/Contents/MacOS)
	return QCoreApplication::applicationDirPath() + QDir::separator() + TemplatesFileName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Server
//////////////////////////////////////////////////////////////////////////////////////////////////

Server::UniquePointer Server::create(firmwareUpdater::core::model::priv::TaskManager& taskManager)
{
	return UniquePointer(new Server(taskManager));
}

Server::Server(firmwareUpdater::core::model::priv::TaskManager& taskManager)
	: _pImpl(std::make_unique<PImpl>(*this, taskManager))
{
}

Server::~Server()
{
}

void Server::start()
{
	_pImpl->createTemplatesFileIfNonExisting();

	// Add task to TaskManager
	_pImpl->_taskManager.addTask([this] 
	{ 
		_pImpl->retrieveTemplatesIfChanged();
	});
}

void Server::reset()
{
	// Just reset current model will cause a full diff... ;-)
	_pImpl->_currentTemplateInfoList = {};
}

void Server::setTemplateInfoListChangeHandler(TemplateInfoListChangeHandler const& onTemplateInfoListChanged)
{
	_pImpl->_onTemplateInfoListChanged = onTemplateInfoListChanged ? onTemplateInfoListChanged : _pImpl->_dummyHandler;
}

} // namespace firmwareUpdater::core::model::templateInfo
