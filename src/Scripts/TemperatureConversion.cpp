#include "preCompiled.h"

class TemperatureConversion : public IRCScript
{
public:
	TemperatureConversion() : IRCScript("TemperatureConversion") { }

	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "f2c", "Adds perks to a specified account", handleFahrenheitToCelcius, ACCESS_NONE, vector<ChatCommand>() },
			{ "c2f", "Removes perks from a specified account", handleCelciusToFahrenheit, ACCESS_NONE, vector<ChatCommand>() }
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleFahrenheitToCelcius(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf(((stof(arguments) - 32.0f) / 1.8f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °C");
	}

	static void handleCelciusToFahrenheit(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf(((stof(arguments) * 1.8f) + 32.0f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °F");
	}
};

void setupTemperatureConversion()
{
	new TemperatureConversion;
}