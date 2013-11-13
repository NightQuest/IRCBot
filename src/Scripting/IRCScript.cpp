#include "preCompiled.h"

IRCScript::IRCScript(const std::string& _scriptName, const SocketDesc& sock) : Socket(sock), scriptName(_scriptName)
{
	sScriptMgr->registerScript(_scriptName, this);
}

IRCScript::~IRCScript()
{
}
