#pragma once

#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::device::priv::deviceSerialization
{

type::Device load(std::string const& deviceUserName, QXmlStreamReader& xml);
void save(type::Device const& device, QXmlStreamWriter& xml);

} // firmwareUpdater::core::model::device::priv::deviceSerialization
