#pragma once

#include <memory>

#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::device::priv
{

auto const DefaultDeviceList = type::DeviceList
{
	{
		.userName = "1_Bass",
		.releaseName = "Bass Guitare",
		.macAddress = "A7-63-D6-CF-66-8E",
		.brand = "Fender",
		.type = "Instrument",
		.description = 
			"The bass guitar, electric bass or simply bass, is the lowest-pitched member of the "
			"guitar family. It is a plucked string instrument similar in appearance and construction "
			"to an electric or an acoustic guitar, but with a longer neck and scale length, and "
			"typically four to six strings or courses. Since the mid-1950s, the bass guitar has largely "
			"replaced the double bass in popular music."
			"More info here: https://en.wikipedia.org/wiki/Bass_guitar"
	},
	{
		.userName = "2_Guitare",
		.releaseName = "Acoustic Guitare",
		.macAddress = "25-C7-E8-86-3F-BC",
		.brand = "Instrument",
		.type = "Gibson",
		.description =
			"An acoustic guitar is a musical instrument[1] in the string family.[2] When a string "
			"is plucked its vibration is transmitted from the bridge, resonating throughout the top "
			"of the guitar.It is also transmitted to the sideand back of the instrument, resonating "
			"through the air in the body,and producing sound from the sound hole.[3] The original, "
			"general term for this stringed instrument is guitar,and the retronym 'acoustic guitar' "
			"distinguishes it from an electric guitar, which relies on electronic amplification."
			"More info here: https://en.wikipedia.org/wiki/Acoustic_guitar"
	},
	{
		.userName = "3_Mac",
		.releaseName = "Mac",
		.macAddress = "D6-B3-53-00-E3-21",
		.brand = "Macintosh",
		.type = "Computer",
		.description =
			"The Macintosh (mainly Mac since 1998)[1] is a family of personal computers designed, "
			"manufactured, and sold by Apple Inc. (originally as Apple Computer, Inc.) since January 1984. "
			"The original Macintosh is the first successful mass - market all - in - one desktop personal "
			"computer to have featured a graphical user interface, built - in screen,and mouse.[2] Apple "
			"sold the Macintosh alongside its popular Apple II, Apple IIGS, Apple III,and Apple Lisa families "
			"of computers until the other models were discontinued in the 1990s."
			"More info here: https://en.wikipedia.org/wiki/Macintosh"
	},
};

} // firmwareUpdater::core::model::device::priv
