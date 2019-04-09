#include "pch.h"
#include "server.h"
#include <windows.h>

Server::Server()
{
}


Server::~Server()
{
}


std::shared_ptr<Player> Server::getPlayerById(unsigned int id)
{
	for (const auto & player : players)
	{
		if (player->getPlayerId() == id)
		{
			return player;
		}
	}
	return nullptr;
}

void Server::sendingEvent()
{
	for (auto & player : this->players)
	{
		sendUdpToEveryone(player->preparePOSpacket());
	}
}

void Server::sendTcpToEveryone(sf::Packet &packet)
{
	for (auto & client : clients)
	{
		client->sendTcp(packet);
	}
}

void Server::sendUdpToEveryone(sf::Packet packet)
{
	for (auto & client : clients)
	{
		client->sendTcp(packet);
	}
}

void Server::acceptTcpMessages()
{
	sf::Packet messagePacket;
	sf::Clock connectionClock;
	connectionClock.restart();
	std::string message = "";
	std::lock_guard<std::mutex> lock(this->TcpMutex);
	while (connectionClock.getElapsedTime().asMilliseconds() < 30)
	{
		inTcpSocket.receive(messagePacket);
		sf::Packet copiedPacket = messagePacket;
		if (messagePacket >> message)
		{
			if (message == "BUL")
			{
				sf::Vector2f position;
				float angle;
				std::string type;

				this->sendTcpToEveryone(copiedPacket);

				messagePacket >> position.x;
				messagePacket >> position.y;
				messagePacket >> angle;
				messagePacket >> type;
				this->bullets.push_back(std::make_shared<Bullet>(position,angle,type));
			}

		}
	}
}

void Server::acceptUdpMessages(unsigned short port)
{
	std::cout << "checking for messages" << std::endl;
	sf::Packet messagePacket;
	sf::Clock connectionClock;
	connectionClock.restart();
	std::string message = "";
	while (connectionClock.getElapsedTime().asMilliseconds() < 30)
	{
		inUdpSocket.receive(messagePacket,this->localIP,port);//UWAGA .receive() zeruje wartoœæ portu, przekazujemy go przez wartoœæ
		if (messagePacket >> message)
		{
			std::cout << "got message" << std::endl;
			if (message == "POS")
			{
				unsigned int id;
				sf::Vector2f position;
				float angle;
				float cannonAngle;
				messagePacket >> id;
				messagePacket >> position.x;
				messagePacket >> position.y;
				messagePacket >> angle;
				messagePacket >> cannonAngle;

				std::cout<<"received POS "<<id<< ' ' << position.x << ' ' << position.y << ' ' << angle << ' ' << cannonAngle << std::endl;
				auto player = this->getPlayerById(id);
				if (player == nullptr)continue;
				player->getShip()->setPosition(position);
				player->getShip()->setRotation(angle);
				player->getShip()->setCannonRotation(cannonAngle);
			}
			if (message == "PPS")
			{
				unsigned int count=0;
				unsigned int id;
				sf::Vector2f position;
				float angle;
				float cannonAngle;
				messagePacket >> count;
				for (unsigned int i = 0; i < count; i++)
				{
					messagePacket >> id;
					messagePacket >> position.x;
					messagePacket >> position.y;
					messagePacket >> angle;
					messagePacket >> cannonAngle;

					auto player = this->getPlayerById(id);
					if (player == nullptr)continue;
					player->getShip()->setPosition(position);
					player->getShip()->setRotation(angle);
					player->getShip()->setCannonRotation(cannonAngle);
				}
			}
		}
		messagePacket.clear();
	}
}

void Server::doStuff()
{
	clients.clear();

	inUdpSocket.setBlocking(false);
	inUdpSocket.bind(sf::UdpSocket::AnyPort);
	this->serverUdpPort = inUdpSocket.getLocalPort();

	std::thread listening(&Server::joinClients,this, std::ref(this->clients));

	sf::IpAddress IP = sf::IpAddress::getLocalAddress();

	

	std::cout << "TCP working on " << IP << ':' << "8888" << std::endl;
	std::cout << "UDP working on " << IP << ':' << this->serverUdpPort << std::endl;
	//MAIN LOOP
	while (1)
	{
		sendingEvent();
		acceptUdpMessages(serverUdpPort);
	}
	this->endFlag = 1;
	listening.join();
}

void Server::joinClients(std::vector<std::shared_ptr<Client>> &clients)
{

	sf::TcpListener listener;
	if (listener.listen(8888) != sf::Socket::Done)
	{
		std::cout << "something is blocking incoming connections" << std::endl;
	}

	while (endFlag == 0)
	{
		std::cout << "waiting for incoming connections" << std::endl;
		std::shared_ptr<Client> connectingClient = std::make_shared<Client>();
		if (listener.accept((*connectingClient).getTcpSocket()) != sf::Socket::Done)
		{
			std::cout << "client tried to join" << std::endl;
			connectingClient = nullptr;
		}
		else
		{
			std::cout << "client accepted" << std::endl;
			sf::Clock connectionClock;
			connectionClock.restart();
			sf::Packet helloPacket;
			helloPacket.clear();

			unsigned short clientUdpPort;
			std::string message;
			{
				std::lock_guard<std::mutex> lock(this->TcpMutex);
				while (connectionClock.getElapsedTime().asSeconds() < 2)
				{
	
					if (connectingClient->receiveTcp(helloPacket) == sf::Socket::Status::Done)
					{
						helloPacket >> message;
						if (message == "HI_")
						{
							std::cout << "received HI message" << std::endl;
							helloPacket >> clientUdpPort;
							connectingClient->setOutUdpPort(clientUdpPort);
							std::cout << "new client connected with IP: " << connectingClient->getRemoteAddress() << ':' << clientUdpPort << std::endl;
							break;
						}
					}
				}
				helloPacket.clear();
				
			}
			helloPacket.clear();
			helloPacket << "HI_" << serverUdpPort;
			std::cout << "sending HI_ : " << serverUdpPort << std::endl;
			connectingClient->sendTcp(helloPacket);

			while (connectionClock.getElapsedTime().asSeconds() < 2)
			{
				if (connectingClient->receiveTcp(helloPacket) == sf::Socket::Status::Done)
				{
					helloPacket >> message;
					if (message == "PLA")
					{
						unsigned int newPlayerId;
						std::string newPlayerName, playerShipModel;
						helloPacket >> newPlayerId;
						helloPacket >> newPlayerName;
						helloPacket >> playerShipModel;
						if (newPlayerId == 0)
						{
							newPlayerId = players.size()+1;
						}
						std::shared_ptr<Player> newPlayer = std::make_shared<Player>(newPlayerId, newPlayerName);
						helloPacket.clear();

						helloPacket << "PLJ";
						helloPacket << newPlayerId;
						helloPacket << newPlayerName; 
						helloPacket << float(120);//pozycja statku x
						helloPacket << float(120);//pozycja statku y
						helloPacket << float(0);//obrót statku

						std::cout << "sending PLJ : " << newPlayerId << ' ' << newPlayerName << std::endl;
						std::cout << "sending player position : " << 120 << ' ' << 120 << std::endl;
						connectingClient->sendTcp(helloPacket);
						
						helloPacket.clear();

						std::lock_guard<std::mutex> lock(this->mutex);
						players.push_back(newPlayer);
						break;
					}
				}
			}
			connectingClient->setBlocking(false);

			{
				std::lock_guard<std::mutex> lock(this->mutex);
				clients.push_back(connectingClient);
			}
			
		}

	}
}

void Server::printPOSPacket(sf::Packet packet)
{
	std::cout << packet.getDataSize() << ' ';
	std::string type = "ERR";
	unsigned int id=0;
	float x, y, angle,cannonAngle;
	x = y = angle = cannonAngle = 420;
	packet >> type;
	packet >> id;
	packet >> x, y, angle, cannonAngle;
	std::cout << "packet " << type << ' ' << id << ' ' << x << ' ' << y << ' ' << angle << ' ' << cannonAngle << std::endl;
}
