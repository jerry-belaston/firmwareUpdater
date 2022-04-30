#include "deviceSerialization.hpp"

#include <QStringView>

namespace firmwareUpdater::core::model::device::priv::deviceSerialization
{

auto constexpr XmlVersion = "1.0";

type::Device load(std::string const& deviceUserName, QXmlStreamReader& xml)
{
	auto device = type::Device{ .userName = deviceUserName };

	if (xml.readNextStartElement()) 
	{
		if (xml.name().toString() == "device") 
		{
			while (xml.readNextStartElement()) 
			{
				if (xml.name().toString() == "release_name")
					device.releaseName = xml.readElementText().toStdString();
				else if (xml.name().toString() == "mac_address")
					device.macAddress = xml.readElementText().toStdString();
				else if (xml.name().toString() == "brand")
					device.brand = xml.readElementText().toStdString();
				else if (xml.name().toString() == "type")
					device.type = xml.readElementText().toStdString();
				else if (xml.name().toString() == "description")
					device.description = xml.readElementText().toStdString();
				else 
					xml.skipCurrentElement();
			}
		}
	}

	return device;
}

void save(type::Device const& device, QXmlStreamWriter& xml)
{
	xml.setAutoFormatting(true);
	xml.writeStartDocument(XmlVersion);
	xml.writeStartElement("device");
	xml.writeTextElement("release_name", QString::fromStdString(device.releaseName));
	xml.writeTextElement("mac_address", QString::fromStdString(device.macAddress));
	xml.writeTextElement("brand", QString::fromStdString(device.brand));
	xml.writeTextElement("type", QString::fromStdString(device.type));
	xml.writeTextElement("description", QString::fromStdString(device.description));
	xml.writeEndElement();
	xml.writeEndDocument();
}

} // firmwareUpdater::core::model::device::priv::deviceSerialization
