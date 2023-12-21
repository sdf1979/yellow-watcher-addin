#pragma once

#include <Windows.h>
#include <sql.h>
#include <sqltypes.h>
#include <sqlext.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include "boost/json.hpp"

#include "DbFunction.h"
#include "SqlTextHash.h"

#define SQL_XML (-152)

namespace Soldy {

	enum class DbValueType {
		Unknown,
		Number,
		String,
		Datetime,
		Binary,
		Guid,
		Xml
	};

}

