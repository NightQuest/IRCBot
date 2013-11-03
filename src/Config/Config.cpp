#include "preCompiled.h"

Config::Config()
{
}

Config::~Config()
{
	data.clear();
}

bool Config::load(const std::string& filename)
{
	std::ifstream file(filename);
	if( !file.is_open() )
		return false;

	std::string line;
	while( std::getline(file, line) )
	{
		if( line[0] == ';' )
			continue;

		size_t pos = line.find_first_of('=');
		if( pos != std::string::npos )
		{
			std::string index = line.substr(0, pos);
			std::string value = line.substr(pos+1);
			size_t firstQuote = value.find_first_of('"');
			size_t lastQuote = value.find_last_of('"');

			if( firstQuote != std::string::npos && lastQuote != std::string::npos )
				value = value.substr(firstQuote+1, lastQuote-2);

			size_t leadingWhiteSpace = index.find_first_not_of(" \t");
			if( leadingWhiteSpace != std::string::npos )
				index = index.substr(leadingWhiteSpace);

			size_t trailingWhiteSpace = index.find_last_not_of(" \t");
			if( trailingWhiteSpace != std::string::npos )
				index = index.substr(0, ++trailingWhiteSpace);


			std::transform(index.begin(), index.end(), index.begin(), ::tolower);
			data[index] = value;
		}
	}
	file.close();

	return !data.empty();
}


int Config::getInt(const std::string& index) const
{
	auto itr = data.find(index);
	if( itr != data.end() )
		return std::stoi(itr->second);
	return 0;
}

unsigned int Config::getUInt(const std::string& index) const
{
	auto itr = data.find(index);
	if( itr != data.end() )
		return static_cast<unsigned int>(std::stoul(itr->second));
	return 0;
}

std::string Config::getString(const std::string& index) const
{
	auto itr = data.find(index);
	if( itr != data.end() )
		return itr->second;
	return "";
}

float Config::getFloat(const std::string& index) const
{
	auto itr = data.find(index);
	if( itr != data.end() )
		return std::stof(itr->second);
	return 0;
}

double Config::getDouble(const std::string& index) const
{
	auto itr = data.find(index);
	if( itr != data.end() )
		return std::stod(itr->second);
	return 0;
}

bool Config::getBool(const std::string& index) const
{
	return getUInt(index) == 1;
}
