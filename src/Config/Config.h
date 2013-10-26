#pragma once

class Config
{
private:
	std::map<std::string,std::string> data;

public:
	Config();
	~Config();

	bool load(const std::string& filename);

	int getInt(const std::string& index) const;
	unsigned int getUInt(const std::string& index) const;
	std::string getString(const std::string& index) const;
	float getFloat(const std::string& index) const;
	double getDouble(const std::string& index) const;
	bool getBool(const std::string& index) const;
};
