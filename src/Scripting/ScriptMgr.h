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
	
	void onJoin(const std::string& channel, const std::string& user);
	void onPart(const std::string& channel, const std::string& user, const std::string& message);
	void onQuit(const std::string& user, const std::string& message);
	void onChatText(const std::string& user, const std::string& target, const std::string& message);
	void onCTCP(const std::string& user, const std::string& target, const std::string& ctcp, const std::string& message);
	void onChatAction(const std::string& user, const std::string& target, const std::string& message);
};

extern const std::unique_ptr<ScriptMgr> sScriptMgr;

#define all_scripts() \
	if( !scripts.empty() )\
		for( auto itr = scripts.begin(); itr != scripts.end(); itr++ )\
			itr->second