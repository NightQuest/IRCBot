#pragma once

class ChannelMember : public User
{
private:
	std::string channelModes;

public:
	ChannelMember(const std::string& user, const std::string& modes = std::string(""));
};

class Channel
{
private:
	std::string name;
	std::string topic;
	std::map<std::string, ChannelMember> users;

public:
	Channel(const std::string& _name) : name(_name) { }
	const std::string getName() const { return name; }
	const std::string getTopic() const { return topic; }

	void setTopic(const std::string& _topic) { topic = _topic; }

	void addUser(const ChannelMember& user);
	void delUser(const std::string& user);
};