#pragma once
#pragma warning(disable:4290) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <sstream>
#include <regex>

using namespace std;

#include "Config/Config.h"
#include "MariaDB/MariaDB.h"
#include "Networking/Socket.h"
#include "Networking/IRCClient.h"

#include "App.h"
