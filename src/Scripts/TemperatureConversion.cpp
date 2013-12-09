#include "preCompiled.h"

class TemperatureConversion : public IRCScript
{
public:
	TemperatureConversion() : IRCScript("TemperatureConversion") { }

	std::vector<ChatCommand> getCommands() override
	{
		ChatCommand commands[] = {
			{ "f2k", "Converts from Fahrenheit to Kelvin", handleFahrenheitToKelvin, ACCESS_NONE, vector<ChatCommand>() },
			{ "f2c", "Converts from Fahrenheit to Celcius", handleFahrenheitToCelcius, ACCESS_NONE, vector<ChatCommand>() },

			{ "c2k", "Converts from Celcius to Kelvin", handleCelciusToKelvin, ACCESS_NONE, vector<ChatCommand>() },
			{ "c2f", "Converts from Celcius to Fahrenheit", handleCelciusToFahrenheit, ACCESS_NONE, vector<ChatCommand>() },

			{ "k2f", "Converts from Kelvin to Fahrenheit", handleKelvinToFahrenheit, ACCESS_NONE, vector<ChatCommand>() },
			{ "k2c", "Converts from Kelvin to Celcius", handleKelvinToCelcius, ACCESS_NONE, vector<ChatCommand>() },
		};

		return vector<ChatCommand>(begin(commands), end(commands));
	}

	static void handleFahrenheitToKelvin(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf((((stof(arguments) - 32.0f) / 1.8f) + 273.15f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " K");
	}

	static void handleFahrenheitToCelcius(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf(((stof(arguments) - 32.0f) / 1.8f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °C");
	}

	static void handleCelciusToKelvin(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf((stof(arguments) + 273.15f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " K");
	}

	static void handleCelciusToFahrenheit(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf(((stof(arguments) * 1.8f) + 32.0f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °F");
	}

	static void handleKelvinToFahrenheit(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf((((stof(arguments) - 273.15f) * 1.8f) + 32.0f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °F");
	}

	static void handleKelvinToCelcius(const std::shared_ptr<User>& user, const std::string& target, const std::string& arguments)
	{
		string messageTarget = (target == sSock->getNickname()) ? user->getNickname() : target;
		stringstream ss;
		ss << fixed << setprecision(2) << (ceilf((stof(arguments) - 273.15f) * 100) / 100);

		sSock->sendMessage(messageTarget, ss.str() + " °C");
	}

};

void setupTemperatureConversion()
{
	new TemperatureConversion();
}