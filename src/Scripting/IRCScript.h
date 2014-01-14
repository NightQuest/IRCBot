#pragma once

typedef void (*ChatCommandFunction)(const UserPtr& user, const std::string& target, const std::string& arguments);
struct ChatCommand
{
	std::string command;
	std::string help;
	ChatCommandFunction function;
	unsigned int required_access;
	vector<ChatCommand> subcommands;
};

class IRCScript
{
private:
	std::string scriptName;

protected:
	IRCScript(const std::string& _scriptName);

public:
	virtual ~IRCScript();

	virtual std::vector<ChatCommand> getCommands() { return std::vector<ChatCommand>(); }

	virtual void onPing(const std::string& argument) { }
	virtual void onWelcome(const std::string& message) { }

	virtual void onJoin(const UserPtr& user, const std::string& channel) { }
	virtual void onPart(const UserPtr& user, const std::string& channel, const std::string& message) { }
	virtual void onQuit(const UserPtr& user, const std::string& message) { }
	virtual void onKick(const UserPtr& user, const std::string& channel, const std::string& target, const std::string& message) { }
	virtual void onNick(const UserPtr& user, const std::string& newNick) { }
	virtual void onNotice(const UserPtr& user, const std::string& target, const std::string& message) { }
	virtual void onChatText(const UserPtr& user, const std::string& target, const std::string& message) { }
	virtual void onCTCP(const UserPtr& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onCTCPReply(const UserPtr& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onChatAction(const UserPtr& user, const std::string& target, const std::string& message) { }
};
