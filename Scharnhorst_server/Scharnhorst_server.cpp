#include "pch.h"
#include "client.h"
#include "player.h"
#include "includes.h"
#include "server.h"
#include <thread>



int main()
{
	Server server1;
	server1.serverLoop();
	return 0;
}