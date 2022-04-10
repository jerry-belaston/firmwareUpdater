#pragma once

#include <memory>
#include <functional>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model
{

class Server
{
public:
	using UniquePointer = std::unique_ptr<Server>;
	using TemplateInfoListChangeHandler = std::function<void(type::TemplateInfoList const&)>;

	// Construction
	static UniquePointer create();
	virtual ~Server();

	void start();
	void setTemplateInfoListChangeHandler(TemplateInfoListChangeHandler const& onTemplateInfoListChanged);

private:
	// Constructor disabled (use create)
	Server();

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model
