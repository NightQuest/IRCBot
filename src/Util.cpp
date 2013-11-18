#include "preCompiled.h"

std::string Util::implode(const std::deque<std::string>& in, const char delim)
{
	std::stringstream tmp;
	for( auto itr = in.begin(); itr != in.end(); itr++ )
	{
		tmp << *itr;
		if( itr != (in.end() - 1) )
			tmp << delim;
	}

	return tmp.str();
}

std::deque<std::string> Util::explode(const std::string& in, const char delim)
{
	std::deque<std::string> tmp;
	std::string str;
	std::istringstream iss(in);
	while( getline(iss, str, delim) )
		if( !str.empty() )
			tmp.push_back(str);

	return tmp;
}
