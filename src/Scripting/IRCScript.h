#pragma once

class IRCScript
{
private:
	std::string scriptName;

protected:
	IRCScript(const std::string& _scriptName);

public:
	virtual ~IRCScript();

	virtual void onJoin(const std::string& channel, const std::string& user) { }
	virtual void onPart(const std::string& channel, const std::string& user, const std::string& message) { }
	virtual void onQuit(const std::string& user, const std::string& message) { }
	virtual void onChatText(const std::string& user, const std::string& target, const std::string& message) { }
	virtual void onCTCP(const std::string& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onChatAction(const std::string& user, const std::string& target, const std::string& message) { }
};
