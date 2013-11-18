#pragma once

namespace Util
{
	std::string implode(const std::deque<std::string>& in, const char delim);
	std::deque<std::string> explode(const std::string& in, const char delim);
};
