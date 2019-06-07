#pragma once
#include "includes.h"
#include "player.h"
#include "turret.h"
#include "bullet.h"
#include "ship.h"
#include "client.h"
#include <thread>
#include <string>
#include <fstream>
#include <mutex>
#include <atomic>
class Server
{
private:
	struct hitInfo
	{
		sf::Vector2f position;
		unsigned int prey;
		unsigned int predator;
		float damage;
	};


	std::mutex mutex;
	std::mutex TcpMutex;
	std::atomic<bool> endFlag=0;
	bool connectingFlag = 0;
	unsigned short serverUdpPort;
	sf::IpAddress localIP = sf::IpAddress::getLocalAddress();

	std::vector<std::shared_ptr<Ship>> wreckages;
	std::vector<std::shared_ptr<Client>> clients;
	std::vector<std::shared_ptr<Player>> players;
	std::vector<Bullet> bullets;

	std::vector<std::pair<std::string, Bullet>> bulletData;
	std::vector<std::pair<std::string, Barrel>> barrelData;
	std::vector<std::pair<std::string, Turret>> turretData;
	std::vector<std::pair<std::string, Ship>> shipData;

	bool loadBullets();
	bool loadBarrels();
	bool loadTurrets();
	bool loadShips();

	Bullet findBullet(std::string);
	Barrel findBarrel(std::string);
	Turret findTurret(std::string);
	Ship findShip(std::string);

	sf::UdpSocket inUdpSocket;
	sf::TcpSocket inTcpSocket;

	std::shared_ptr<Player> getPlayerById(unsigned int);
	void generateBullet(bulletInfo&);
public:
	void sendingEvent(); //Funkcja obs�uguj�ca wysy�anie, jako sta�y element gry
	void sendTcpToEveryone(sf::Packet);
	void sendUdpToEveryone(sf::Packet);
	void receiveTcpMessages();
	void receiveUdpMessages();
	void serverLoop();
	void joinClients(std::vector<std::shared_ptr<Client>> &clients);
	void printPOSPacket(sf::Packet);

	sf::Packet prepareHITpacket(std::shared_ptr<Player>&,Bullet&);
	sf::Packet prepareKILpacket(unsigned int,unsigned int);
	//void checkColission();
	Server();
	~Server();


};

