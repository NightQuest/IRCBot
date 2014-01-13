#pragma once

class ScriptMgr
{
private:
	std::map<std::string, IRCScript*> scripts;

public:
	ScriptMgr();
	~ScriptMgr();

	void registerScript(const std::string& scriptName, IRCScript* script);
	void unregisterScript(const std::string& scriptName);
	void unregisterAll();

	bool isRegistered(const std::string& scriptName);
	IRCScript* operator[](const std::string& scriptName);

	std::vector<ChatCommand> getCommands();

	void onPing(const std::string& argument);
	void onWelcome(const std::string& message);

	void onJoin(const std::shared_ptr<User>& user, const std::string& channel);
	void onPart(const std::shared_ptr<User>& user, const std::string& channel, const std::string& message);
	void onQuit(const std::shared_ptr<User>& user, const std::string& message);
	void onKick(const std::shared_ptr<User>& user, const std::string& channel, const std::string& target, const std::string& message);
	void onNick(const std::shared_ptr<User>& user, const std::string& newNick);
	void onNotice(const std::shared_ptr<User>& user, const std::string& target, const std::string& message);
	void onChatText(const std::shared_ptr<User>& user, const std::string& target, const std::string& message);
	void onCTCP(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message);
	void onCTCPReply(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message);
	void onChatAction(const std::shared_ptr<User>& user, const std::string& target, const std::string& message);
};

extern const std::unique_ptr<ScriptMgr> sScriptMgr;

#define all_scripts() \
	if( !scripts.empty() )\
		for( auto itr = scripts.begin(); itr != scripts.end(); itr++ )\
			itr->second