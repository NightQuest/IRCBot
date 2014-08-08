#pragma once

#define MINUTE (60)
#define HOUR (MINUTE*60)
#define DAY (HOUR*24)
#define WEEK (DAY*7)
#define YEAR (DAY*365)

namespace Util
{
	std::string implode(const std::deque<std::string>& in, const char delim);
	std::deque<std::string> explode(const std::string& in, const char delim);
};
