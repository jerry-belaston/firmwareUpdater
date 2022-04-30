#pragma once

#include <map>

#include <QObject>

#include "ui/toolkit/dataDownloader.hpp"
#include "core/common/type.hpp"

namespace firmwareUpdater::ui::view::workspace
{

class ImageCacheManager : public QObject
{
	Q_OBJECT
public:
	using TemplateIndex = std::uint32_t;
	using TemplateStepIndex = std::uint32_t;

	ImageCacheManager(QObject* parent = nullptr);
	void clear();
	void prepareImageCache(core::type::TemplateInfoList const& templateInfoList);
	toolkit::DataDownloader const& getDataDownloader(TemplateIndex const templateIndex, TemplateStepIndex const templateStepIndex) const;

private:

	toolkit::DataDownloader const _dummyDataDownloader{ {} };

	// For what we're doing, unordered_map should be prefered here...
	using Key = std::pair<TemplateIndex, TemplateStepIndex>;
	std::map<Key, std::unique_ptr<toolkit::DataDownloader>> _dataDownloaders;
};

} // namespace firmwareUpdater::ui::view::workspace
