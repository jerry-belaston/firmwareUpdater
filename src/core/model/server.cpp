#include "server.hpp"
#include "core/toolkit/timerTask.hpp"
#include "private/templatesSerialization.hpp"
#include "private/defaultTemplates.hpp"

#include <QDir>
#include <QCoreApplication>
#include <QJsonDocument>

auto constexpr TemplatesFileName{ "templates.json" };

namespace firmwareUpdater::core::model
{

//////////////////////////////////////////////////////////////////////////////////////////////////
// PImpl
//////////////////////////////////////////////////////////////////////////////////////////////////

class Server::PImpl
{
public:
	PImpl(Server& parent);

	void createTemplatesFileIfNonExisting();
	void retrieveTemplatesIfChanged();

	static QFile getTemplatesFile();

	// Members
	Server& _parent;
	toolkit::TimerTask::UniquePointer _timerTask{};
	TemplateInfoListChangeHandler _onTemplateInfoListChanged{};
	type::TemplateInfoList _currentTemplateInfoList{};
};

Server::PImpl::PImpl(Server& parent)
	: _parent{ parent }
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
	templatesFile.write(QJsonDocument{ priv::templatesSerialization::save(priv::DefaultTemplateInfoList) }.toJson());
}

void Server::PImpl::retrieveTemplatesIfChanged()
{
	auto templatesFile = getTemplatesFile();

	auto newTemplateInfoList = type::TemplateInfoList{};
	if (templatesFile.open(QIODevice::ReadOnly))
		newTemplateInfoList = priv::templatesSerialization::load(QJsonDocument::fromJson(templatesFile.readAll()).object());

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

Server::UniquePointer Server::create()
{
	return UniquePointer(new Server());
}

Server::Server()
	: _pImpl(std::make_unique<PImpl>(*this))
{
}

Server::~Server()
{
}

void Server::start()
{
	_pImpl->createTemplatesFileIfNonExisting();

	// Start timer task
	_pImpl->_timerTask = core::toolkit::TimerTask::create([this] 
		{ 
			_pImpl->retrieveTemplatesIfChanged();
		});
}

void Server::setTemplateInfoListChangeHandler(TemplateInfoListChangeHandler const& onTemplateInfoListChanged)
{
	_pImpl->_onTemplateInfoListChanged = onTemplateInfoListChanged;
}

} // namespace firmwareUpdater::core::model
