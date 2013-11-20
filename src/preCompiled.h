#pragma once
#pragma warning(disable:4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <sstream>
#include <regex>

using namespace std;

#include "Util.h"
#include "Config/Config.h"
#include "MariaDB/MariaDB.h"
#include "IRC/User.h"
#include "IRC/Channel.h"
#include "Networking/Socket.h"
#include "Networking/IdentServer.h"
#include "Networking/IRCSocket.h"
#include "Scripting/IRCScript.h"
#include "Networking/IRCClient.h"
#include "Scripting/SetupScripts.h"
#include "Scripting/ScriptMgr.h"

#include "App.h"
