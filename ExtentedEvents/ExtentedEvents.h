#pragma once

#include "boost/json.hpp"
#include "pugixml.hpp"
#include <string>
#include <string_view>

namespace Soldy {

	std::string ParseExtentedEvent(const std::string& data);
}
