#pragma once

#include <memory>

namespace firmwareUpdater::core
{

namespace model
{
	class Model;
}
namespace controller
{
	class Controller;
}

class Core
{
public:
	using UniquePointer = std::unique_ptr<Core>;

	// Construction
	static UniquePointer create();
	virtual ~Core();

	void start();
	model::Model& getModel();
	controller::Controller& getController();

private:
	// Constructor disabled (use create)
	Core();

private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core
