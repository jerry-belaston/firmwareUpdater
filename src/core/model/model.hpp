#pragma once

#include <memory>
#include <functional>


namespace firmwareUpdater::core::model
{

namespace templateInfo
{
	class Server;
}
namespace device
{
	class Discovery;
	class UploadManager;
}

class Model
{
public:
	using UniquePointer = std::unique_ptr<Model>;

	// Construction
	static UniquePointer create();
	virtual ~Model();

	void start();
	templateInfo::Server& getServer();
	device::Discovery& getDiscovery();
	device::UploadManager& getUploadManager();

private:
	// Constructor disabled (use create)
	Model();


private:
	class PImpl;
	std::unique_ptr<PImpl> _pImpl{ nullptr };
};

} // firmwareUpdater::core::model
