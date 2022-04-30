#pragma once

#include <vector>
#include <string>
#include <chrono>

using namespace std::chrono_literals;

namespace firmwareUpdater::core::type
{

struct TemplateInfo
{
	using Name = std::string;
	Name name;

	struct StepInfo
	{
		std::chrono::milliseconds duration{ 1000ms };
		std::string imageUrl{};
		std::string descriptionText{ "no info" };
		std::string descriptionColor{ "black"}; // Format #RRGGBB or standard color name text
		int descriptionSize{ 12 }; // in pt
		std::string previousButtonText{ "Previous" };
		bool previousButtonDisplayed{ true };
		std::string nextButtonText{ "Next" };

		bool operator==(const StepInfo& other) const = default;
	};
	using StepInfoList = std::vector<StepInfo>;
	StepInfoList stepInfoList;

	bool operator==(const TemplateInfo& other) const = default;
};

using TemplateInfoList = std::vector<TemplateInfo>;
using TemplateNameList = std::vector<TemplateInfo::Name>;

struct Device
{
    using UserName = std::string;
    using ReleaseName = std::string;

    UserName userName;
    ReleaseName releaseName;
    std::string macAddress;
    std::string brand;
    std::string type;
    std::string description;

    bool operator==(const Device& other) const = default;
};

using DeviceList = std::vector<Device>;
using DeviceUserNameList = std::vector<Device::UserName>;

} // namespace firmwareUpdater::core::type
