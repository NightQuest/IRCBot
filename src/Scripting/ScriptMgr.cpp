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

void ScriptMgr::onJoin(const std::string& channel, const std::string& user)
{
	all_scripts()->onJoin(channel, user);
}

void ScriptMgr::onPart(const std::string& channel, const std::string& user, const std::string& message)
{
	all_scripts()->onPart(channel, user, message);
}

void ScriptMgr::onQuit(const std::string& user, const std::string& message)
{
	all_scripts()->onQuit(user, message);
}

void ScriptMgr::onChatText(const std::string& user, const std::string& target, const std::string& message)
{
	all_scripts()->onChatText(user, target, message);
}

void ScriptMgr::onCTCP(const std::string& user, const std::string& target, const std::string& ctcp, const std::string& message)
{
	all_scripts()->onCTCP(user, target, ctcp, message);
}

void ScriptMgr::onChatAction(const std::string& user, const std::string& target, const std::string& message)
{
	all_scripts()->onChatAction(user, target, message);
}
