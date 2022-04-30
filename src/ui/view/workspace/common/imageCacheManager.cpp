#include "imageCacheManager.hpp"

namespace firmwareUpdater::ui::view::workspace
{

ImageCacheManager::ImageCacheManager(QObject* parent)
	: QObject(parent)
{
}

void ImageCacheManager::clear()
{
	_dataDownloaders.clear();
}

void ImageCacheManager::prepareImageCache(core::type::TemplateInfoList const& templateInfoList)
{
	_dataDownloaders.clear();
	for (auto i = 0u; i < templateInfoList.size(); ++i)
	{
		for (auto j = 0u; j < templateInfoList[i].stepInfoList.size(); ++j)
		{
			_dataDownloaders[{ i, j }] = std::make_unique<toolkit::DataDownloader>(
				QString::fromStdString(templateInfoList[i].stepInfoList[j].imageUrl));
		}
	}
}

toolkit::DataDownloader const& ImageCacheManager::getDataDownloader(TemplateIndex const templateIndex, 
	TemplateStepIndex const templateStepIndex) const
{
	if (auto it = _dataDownloaders.find({ templateIndex, templateStepIndex }); it != std::end(_dataDownloaders))
		return *it->second;
	return _dummyDataDownloader;
}

} // namespace firmwareUpdater::ui::view::workspace
