#pragma once

class IRCScript : public Socket
{
private:
	std::string scriptName;

protected:
	IRCScript(const std::string& _scriptName, const SocketDesc& sock);

	int sendLine(const std::string& line) { return send(line + "\r\n"); }
	int sendCTCPResponse(const std::string& target, const std::string& ctcp, const std::string& msg) { return sendLine("NOTICE " + target + " :\x1" + ctcp + " " + msg + "\x1"); }
	int sendCTCP(const std::string& target, const std::string& ctcp) { return sendLine("PRIVMSG " + target + " :\x1" + ctcp + "\x1"); }
	int sendAction(const std::string& target, const std::string& message) { return sendCTCP(target, "ACTION " + message); }
	int sendMessage(const std::string& target, const std::string& message) { return sendLine("PRIVMSG " + target + " :" + message); }

public:
	virtual ~IRCScript();

	virtual void onJoin(const std::string& channel, const std::string& user) { }
	virtual void onPart(const std::string& channel, const std::string& user, const std::string& message) { }
	virtual void onQuit(const std::string& user, const std::string& message) { }
	virtual void onChatText(const std::string& user, const std::string& target, const std::string& message) { }
	virtual void onCTCP(const std::string& user, const std::string& target, const std::string& ctcp, const std::string& message) { }
	virtual void onChatAction(const std::string& user, const std::string& target, const std::string& message) { }
};
