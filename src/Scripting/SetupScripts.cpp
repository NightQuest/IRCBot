#include "preCompiled.h"

void setupEcho();
void setupHandleCTCP();
void setupHandleLogin();
void setupHandlePing();
void setupQuitCommand();
void setupSayCommand();
void setupSQLCommand();

void setupScripts()
{
	setupEcho();
	setupHandleCTCP();
	setupHandleLogin();
	setupHandlePing();
	setupQuitCommand();
	setupSayCommand();
	setupSQLCommand();
}
