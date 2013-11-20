#include "preCompiled.h"

void setupEcho();
void setupHandleCTCP();
void setupHandleLogin();
void setupQuitCommand();
void setupSayCommand();
void setupSQLCommand();

void setupScripts()
{
	setupEcho();
	setupHandleCTCP();
	setupHandleLogin();
	setupQuitCommand();
	setupSayCommand();
	setupSQLCommand();
}
