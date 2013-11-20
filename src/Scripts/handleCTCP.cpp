#include "preCompiled.h"

class handleCTCP : public IRCScript
{
public:
	handleCTCP() : IRCScript("handleCTCP") { }

	void onCTCP(const std::shared_ptr<User>& user, const std::string& target, const std::string& ctcp, const std::string& message)
	{
		if( ctcp == "VERSION" )
			sSock->sendCTCPResponse(user->getNickname(), ctcp, config->getString("irc.ctcpversion"));
	}
};

void setupHandleCTCP()
{
	new handleCTCP();
}
