#include "preCompiled.h"

static const unique_ptr<ScriptMgr> sScriptMgr(new ScriptMgr);

ScriptMgr::ScriptMgr()
{
}

ScriptMgr::~ScriptMgr()
{
	unregisterAll();
}

void ScriptMgr::registerScript(const std::string& scriptName, IRCScript* script)
{
	scripts[scriptName] = script;
}

void ScriptMgr::unregisterScript(const std::string& scriptName)
{
	auto pos = scripts.find(scriptName);
	if( pos != scripts.end() )
		scripts.erase(pos);
}

void ScriptMgr::unregisterAll()
{
	if( !scripts.empty() )
	{
		for( auto itr = scripts.begin(); itr != scripts.end(); itr++ )
		{
			delete itr->second;
		}
		scripts.clear();
	}
}

bool ScriptMgr::isRegistered(const std::string& scriptName)
{
	return (scripts.find(scriptName) != scripts.end());
}

IRCScript* ScriptMgr::operator[](const std::string& scriptName)
{
	auto pos = scripts.find(scriptName);
	if( pos == scripts.end() )
		return nullptr;

	return pos->second;
}

std::vector<ChatCommand> ScriptMgr::getCommands()
{
	std::vector<ChatCommand> commands;

	if( !scripts.empty() )
	{
		for( auto itr = scripts.begin(); itr != scripts.end(); itr++ )
		{
			auto tmp = itr->second->getCommands();
			if( !tmp.empty() )
				commands.insert(commands.end(), tmp.begin(), tmp.end());
		}
	}

	return commands;
}

void ScriptMgr::onPing(const std::string& argument)
{
	all_scripts()->onPing(argument);
}

void ScriptMgr::onWelcome(const std::string& message)
{
	all_scripts()->onWelcome(message);
}

void ScriptMgr::onJoin(const std::shared_ptr<User>& user, const std::string& channel)
{
	all_scripts()->onJoin(user, channel);
}

void ScriptMgr::onPart(const std::shared_ptr<User>& user, const std::string& channel, const std::string& message)
{
	all_scripts()->onPart(user, channel, message);
}

void ScriptMgr::onQuit(const std::shared_ptr<User>& user, const std::string& message)
{
	all_scripts()->onQuit(user, message);
}

void ScriptMgr::onNotice(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
{
	all_scripts()->onNotice(user, target, message);
}

void ScriptMgr::onChatText(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
{
	all_scripts()->onChatText(user, target, message);
}

void ScriptMgr::onCTCPReply(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message)
{
	all_scripts()->onCTCPReply(user, target, ctcp, message);
}

void ScriptMgr::onCTCP(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message)
{
	all_scripts()->onCTCP(user, target, ctcp, message);
}

void ScriptMgr::onChatAction(const std::shared_ptr<User>& user, const std::string& target, const std::string& message)
{
	all_scripts()->onChatAction(user, target, message);
}
