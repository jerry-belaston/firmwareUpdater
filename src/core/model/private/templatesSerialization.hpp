#pragma once

#include <QJsonObject>
#include "core/common/type.hpp"

namespace firmwareUpdater::core::model::priv::templatesSerialization
{

type::TemplateInfoList load(QJsonObject const& json);
QJsonObject save(type::TemplateInfoList const& templateInfoList);

} // firmwareUpdater::core::model::priv::templatesSerialization
