#include "preCompiled.h"

IRCScript::IRCScript(const std::string& _scriptName) : scriptName(_scriptName)
{
	sScriptMgr->registerScript(_scriptName, this);
}

IRCScript::~IRCScript()
{
}
