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

#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Config/Config.h"
#include "MariaDB/MariaDB.h"
#include "Socket/Socket.h"
#include "IRCClient/IRCClient.h"
