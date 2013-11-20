#include "preCompiled.h"

class handlePing : public IRCScript
{
public:
	handlePing() : IRCScript("handlePing") { }

	void onPing(const std::string& argument)
	{
		sSock->sendLine("PONG :" + argument);

		if( externalDB && externalDB->keepAlive )
			externalDB->ping();
		if( internalDB && internalDB->keepAlive )
			internalDB->ping();
	}
};

void setupHandlePing()
{
	new handlePing();
}
