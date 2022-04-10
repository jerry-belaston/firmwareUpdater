#pragma once

#include <vector>
#include <string>

namespace firmwareUpdater::core::type
{

struct TemplateInfo
{
	using Name = std::string;
	Name name;

	struct StepInfo
	{
		std::string imageUrl{};
		std::string descriptionText{ "no info" };
		std::string descriptionColor{ "black"}; // Format #RRGGBB or standard color name text
		int descriptionSize{ 12 }; // in pt
		std::string previousButtonText{ "Previous" };
		bool previousButtonDisplayed{ true };
		std::string nextButtonText{ "Next" };

		bool operator==(const StepInfo& other) const noexcept
		{
			return
				imageUrl == other.imageUrl &&
				descriptionText == other.descriptionText &&
				descriptionColor == other.descriptionColor &&
				descriptionSize == other.descriptionSize &&
				previousButtonText == other.previousButtonText &&
				previousButtonDisplayed == other.previousButtonDisplayed &&
				nextButtonText == other.nextButtonText;
		}
	};
	using StepInfoList = std::vector<StepInfo>;
	StepInfoList stepInfoList;

	bool operator==(const TemplateInfo& other) const noexcept
	{
		return
			name == other.name &&
			stepInfoList == other.stepInfoList;
	}
};

using TemplateInfoList = std::vector<TemplateInfo>;
using TemplateNameList = std::vector<TemplateInfo::Name>;

} // namespace firmwareUpdater::core::type
