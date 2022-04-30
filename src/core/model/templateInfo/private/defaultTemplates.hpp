#pragma once

#include <memory>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::templateInfo::priv
{

auto const DefaultTemplateInfoList = type::TemplateInfoList 
{
	{
		.name = "Default",
		.stepInfoList = type::TemplateInfo::StepInfoList
		{
			{
				.duration = 1000ms,
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "This is the first step of the firmware update processing. Please wait for the next one. Thanks.",
				.descriptionColor = "lightgrey",
				.descriptionSize = 11,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 3000ms,
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "This is the second step of the firmware update processing. Please wait for the next one. Thanks.",
				.descriptionColor = "lightgrey",
				.descriptionSize = 11,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 1000ms,
				.imageUrl = "https://d3kqkuy1hpjocx.cloudfront.net/s3fs-public/styles/thumbnail/public/pictures/picture-134773-1541674067.jpg",
				.descriptionText = "This is the last step. Please wait until the end of the processing. Thanks.",
				.descriptionColor = "lightgrey",
				.descriptionSize = 11,
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
				.duration = 4000ms,
				.imageUrl = "https://www.gstatic.com/webp/gallery/1.png",
				.descriptionText = "Airy: step 1",
				.descriptionColor = "#00FF00",
				.descriptionSize = 12,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
			{
				.duration = 1000ms,
				.imageUrl = "https://www.gstatic.com/webp/gallery/2.png",
				.descriptionText = "Airy: step 2",
				.descriptionColor = "lightgrey",
				.descriptionSize = 10,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
			{
				.duration = 4000ms,
				.imageUrl = "https://www.gstatic.com/webp/gallery/3.png",
				.descriptionText = "Airy: step 3",
				.descriptionColor = "#00FF00",
				.descriptionSize = 12,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
			{
				.duration = 1000ms,
				.imageUrl = "https://www.gstatic.com/webp/gallery/4.png",
				.descriptionText = "Airy: step 4",
				.descriptionColor = "lightgrey",
				.descriptionSize = 10,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = false,
				.nextButtonText = "Next",
			},
			{
				.duration = 4000ms,
				.imageUrl = "https://www.gstatic.com/webp/gallery/5.png",
				.descriptionText = "Airy: step 5",
				.descriptionColor = "#00FF00",
				.descriptionSize = 12,
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
				.duration = 1000ms,
				.imageUrl = "https://www.webstickersmuraux.com/fr/img/asmu071-png/folder/products-detalle-png/autocollants-homer-simpson-reggae-.png",
				.descriptionText = "Enjoy: Homer Simpson",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://images.ladepeche.fr/api/v1/images/view/5c34a8a28fe56f06b069e6b4/full/image.jpg",
				.descriptionText = "Enjoy: Kung Fu Panda",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://www.chroniquedisney.fr/imgPerso/mickeyville/1928-mickey-mouse-00.png",
				.descriptionText = "Enjoy: Mickey",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://www.chroniquedisney.fr/imgPerso/donaldville/1934-donald-duck-00.png",
				.descriptionText = "Enjoy: Donal",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://static.turbosquid.com/Preview/2019/11/06__00_22_24/blender.jpg7AE32B49-AA42-47EE-B4A0-33907CC9F555Large.jpg",
				.descriptionText = "Enjoy: Olaf",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://www.cdiscount.com/pdt2/5/9/0/1/700x700/auc5060219944590/rw/figurine-en-carton-taille-reelle-le-roi-lion-simba.jpg",
				.descriptionText = "Enjoy: Lion King",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			},
			{
				.duration = 2000ms,
				.imageUrl = "https://cdn.radiofrance.fr/s3/cruiser-production/2015/07/aef7be57-24bb-11e5-bab6-005056a87c30/640_066800jpg-r-640-600-b-1-d6d6d6-f-jpg-q-x-xxyxx.jpg",
				.descriptionText = "Enjoy: Minions",
				.descriptionColor = "lightgrey",
				.descriptionSize = 8,
				.previousButtonText = "Previous",
				.previousButtonDisplayed = true,
				.nextButtonText = "Next",
			}
		}
	},
};

} // firmwareUpdater::core::model::templateInfo::priv
