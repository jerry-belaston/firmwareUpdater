#include "templateSerialization.hpp"

#include <QJsonArray>

namespace firmwareUpdater::core::model::templateInfo::priv::templateSerialization
{

auto constexpr JsonVersion = 1.0;

type::TemplateInfoList load(QJsonObject const& json)
{
	auto templateInfoList = type::TemplateInfoList{};

	if (json.contains("templates") && json["templates"].isArray())
	{
		auto const& jsonTemplateInfoList = json["templates"].toArray();
		for (auto i = 0; i < jsonTemplateInfoList.size(); ++i)
		{
			auto const& jsonTemplateInfo = jsonTemplateInfoList[i].toObject();
			auto templateInfo = type::TemplateInfo{};

			if (jsonTemplateInfo.contains("steps") && jsonTemplateInfo["steps"].isArray())
			{
				auto const& jsonStepInfoList = jsonTemplateInfo["steps"].toArray();
				for (auto j = 0; j < jsonStepInfoList.size(); ++j)
				{
					auto const& jsonStepInfo = jsonStepInfoList[j].toObject();
					auto stepInfo = type::TemplateInfo::StepInfo{};
					if (jsonStepInfo.contains("duration") && jsonStepInfo["duration"].isDouble())
						stepInfo.duration = std::chrono::milliseconds(jsonStepInfo["duration"].toInt());
					if (jsonStepInfo.contains("description") && jsonStepInfo["description"].isObject())
					{
						auto const& jsonDescription = jsonStepInfo["description"].toObject();
						if (jsonDescription.contains("text") && jsonDescription["text"].isString())
							stepInfo.descriptionText = jsonDescription["text"].toString().toStdString();
						if (jsonDescription.contains("color") && jsonDescription["color"].isString())
							stepInfo.descriptionColor = jsonDescription["color"].toString().toStdString();
						if (jsonDescription.contains("size") && jsonDescription["size"].isDouble())
							stepInfo.descriptionSize = jsonDescription["size"].toInt();
					}
					if (jsonStepInfo.contains("image_url") && jsonStepInfo["image_url"].isString())
						stepInfo.imageUrl = jsonStepInfo["image_url"].toString().toStdString();
					if (jsonStepInfo.contains("button_previous") && jsonStepInfo["button_previous"].isObject())
					{
						auto const& jsonButtonPrevious = jsonStepInfo["button_previous"].toObject();
						if (jsonButtonPrevious.contains("text") && jsonButtonPrevious["text"].isString())
							stepInfo.previousButtonText = jsonButtonPrevious["text"].toString().toStdString();
						if (jsonButtonPrevious.contains("displayed") && jsonButtonPrevious["displayed"].isBool())
							stepInfo.previousButtonDisplayed = jsonButtonPrevious["displayed"].toBool();
					}
					if (jsonStepInfo.contains("button_next") && jsonStepInfo["button_next"].isObject())
					{
						auto const& jsonButtonNext = jsonStepInfo["button_next"].toObject();
						if (jsonButtonNext.contains("text") && jsonButtonNext["text"].isString())
							stepInfo.nextButtonText = jsonButtonNext["text"].toString().toStdString();
					}
					templateInfo.stepInfoList.push_back(stepInfo);
				}
			}

			// Get template only if there is at least one step
			if (!templateInfo.stepInfoList.empty())
			{
				if (jsonTemplateInfo.contains("name") && jsonTemplateInfo["name"].isString())
					templateInfo.name = jsonTemplateInfo["name"].toString().toStdString();

				templateInfoList.push_back(templateInfo);
			}
		}
	}

	return templateInfoList;
}

QJsonObject save(type::TemplateInfoList const& templateInfoList)
{
	auto json = QJsonObject{};
	json["version"] = JsonVersion;

	auto jsonTemplateList = QJsonArray{};
	for (auto const& templateInfo : templateInfoList)
	{
		auto jsonTemplateInfo = QJsonObject{};
		jsonTemplateInfo["name"] = QString::fromStdString(templateInfo.name);

		auto jsonStepInfoList = QJsonArray{};
		for (auto const& stepInfo : templateInfo.stepInfoList)
		{
			auto jsonStepInfo = QJsonObject{};
			auto jsonDescription = QJsonObject{};
			jsonDescription["text"] = QString::fromStdString(stepInfo.descriptionText);
			jsonDescription["color"] = QString::fromStdString(stepInfo.descriptionColor);
			jsonDescription["size"] = stepInfo.descriptionSize;
			jsonStepInfo["duration"] = stepInfo.duration.count();
			jsonStepInfo["description"] = jsonDescription;
			jsonStepInfo["image_url"] = QString::fromStdString(stepInfo.imageUrl);
			auto jsonButtonPrevious = QJsonObject{};
			jsonButtonPrevious["text"] = QString::fromStdString(stepInfo.previousButtonText);
			jsonButtonPrevious["displayed"] = stepInfo.previousButtonDisplayed;
			jsonStepInfo["button_previous"] = jsonButtonPrevious;
			auto jsonButtonNext = QJsonObject{};
			jsonButtonNext["text"] = QString::fromStdString(stepInfo.nextButtonText);
			jsonStepInfo["button_next"] = jsonButtonNext;

			jsonStepInfoList.append(jsonStepInfo);
		}
		jsonTemplateInfo["steps"] = jsonStepInfoList;

		jsonTemplateList.append(jsonTemplateInfo);
	}

	json["templates"] = jsonTemplateList;

	return json;
}

} // firmwareUpdater::core::model::templateInfo::priv::templateSerialization
