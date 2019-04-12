#pragma once
#include "client.h"
#include "player.h"
#include "bullet.h"
#include "includes.h"
#include <thread>
#include <mutex>
#include <atomic>
class Server
{
private:

	std::mutex mutex;
	std::mutex TcpMutex;
	std::atomic<bool> endFlag=0;
	bool connectingFlag = 0;
	unsigned short serverUdpPort;
	sf::IpAddress localIP = sf::IpAddress::getLocalAddress();

	std::vector<std::shared_ptr<Client>> clients;
	std::vector<std::shared_ptr<Player>> players;
	std::vector<std::shared_ptr<Bullet>> bullets;
	sf::UdpSocket inUdpSocket;
	sf::TcpSocket inTcpSocket;

	std::shared_ptr<Player> getPlayerById(unsigned int);

public:
	void sendingEvent(); //Funkcja obs³uguj¹ca wysy³anie, jako sta³y element gry
	void sendTcpToEveryone(sf::Packet&);
	void sendUdpToEveryone(sf::Packet);
	void acceptTcpMessages();
	void acceptUdpMessages();
	void doStuff();
	void joinClients(std::vector<std::shared_ptr<Client>> &clients);
	void printPOSPacket(sf::Packet);
	Server();
	~Server();
};

