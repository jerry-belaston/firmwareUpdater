#pragma once

#include <memory>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::priv
{

auto const DefaultTemplateInfoList = type::TemplateInfoList 
{
	{
		.name = "Default",
		.stepInfoList = type::TemplateInfo::StepInfoList
		{
			{
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "First step",
				.descriptionColor = "black",
				.descriptionSize = 16,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "Second step",
				.descriptionColor = "blue",
				.descriptionSize = 20,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "Third and last important step",
				.descriptionColor = "#CC00FF",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
		}
	},
	{
		.name = "Airy",
		.stepInfoList = type::TemplateInfo::StepInfoList
		{
			{
				.imageUrl = "https://www.gstatic.com/webp/gallery/1.png",
				.descriptionText = "Airy: step 1",
				.descriptionColor = "#00FF00",
				.descriptionSize = 12,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
			{
				.imageUrl = "https://www.gstatic.com/webp/gallery/4.png",
				.descriptionText = "#Airy: step 2",
				.descriptionColor = "black",
				.descriptionSize = 10,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
		}
	},
	{
		.name = "Enjoy",
		.stepInfoList = type::TemplateInfo::StepInfoList
		{
			{
				.imageUrl = "https://www.webstickersmuraux.com/fr/img/asmu071-png/folder/products-detalle-png/autocollants-homer-simpson-reggae-.png",
				.descriptionText = "Enjoy: only one funny step",
				.descriptionColor = "red",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			}
		}
	}
};

} // firmwareUpdater::core::model::priv
