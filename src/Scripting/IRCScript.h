#pragma once

typedef void (*ChatCommandFunction)(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments);
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

	virtual void onWelcome(const std::string& message) { }

	virtual void onJoin(const std::shared_ptr<User>& user, const std::string& channel) { }
	virtual void onPart(const std::shared_ptr<User>& user, const std::string& channel, const std::string& message) { }
	virtual void onQuit(const std::shared_ptr<User>& user, const std::string& message) { }
	virtual void onNotice(const std::shared_ptr<User>& user, const std::string& target, const std::string& message) { }
	virtual void onChatText(const std::shared_ptr<User>& user, const std::string& target, const std::string& message) { }
	virtual void onCTCP(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onCTCPReply(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onChatAction(const std::shared_ptr<User>& user, const std::string& target, const std::string& message) { }
};
