#pragma once

#include <memory>
#include <functional>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::priv
{
	class TaskManager;
}

namespace firmwareUpdater::core::model::templateInfo
{

class Server
{
public:
	using UniquePointer = std::unique_ptr<Server>;
	using TemplateInfoListChangeHandler = std::function<void(type::TemplateInfoList const&)>;

	// Construction
	static UniquePointer create(firmwareUpdater::core::model::priv::TaskManager& taskManager);
	virtual ~Server();

	void start();
	void reset();
	void setTemplateInfoListChangeHandler(TemplateInfoListChangeHandler const& onTemplateInfoListChanged);

private:
	// Constructor disabled (use create)
	Server(firmwareUpdater::core::model::priv::TaskManager& taskManager);

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model::templateInfo
