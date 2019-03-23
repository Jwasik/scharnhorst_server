#include "pch.h"
#include "server.h"

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

void Server::acceptTcpMessage()
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

void Server::acceptUdpMessage()
{
	sf::Packet messagePacket;
	sf::Clock connectionClock;
	connectionClock.restart();
	std::string message = "";
	while (connectionClock.getElapsedTime().asMilliseconds() < 30)
	{
		inTcpSocket.receive(messagePacket);
		if (messagePacket >> message)
		{
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
	while (1)
	{
		//sending

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

		std::shared_ptr<Client> connectingClient = std::make_shared<Client>();
		if (listener.accept((*connectingClient).getTcpSocket()) != sf::Socket::Done)
		{
			std::cout << "client tried to join" << std::endl;
			connectingClient = nullptr;
		}
		else
		{
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
							for (unsigned int i = 1; i < 10000; i++)//Przydziela nowe ID
							{
								if (this->getPlayerById(i) == nullptr);
								{
									newPlayerId = i;
									break;
								}
							}
						}
						std::shared_ptr<Player> newPlayer = std::make_shared<Player>(newPlayerId, newPlayerName);
						helloPacket.clear();

						helloPacket << "PLJ";
						helloPacket << newPlayerId;
						helloPacket << newPlayerName;
						helloPacket << float(120);//pozycja statku
						helloPacket << float(120);
						helloPacket << float(0);//obrót statku

						std::cout << "sending PLJ : " << newPlayerId << ' ' << newPlayerName << std::endl;

						connectingClient->sendTcp(helloPacket);
						helloPacket.clear();

						std::lock_guard<std::mutex> lock(this->mutex);
						players.push_back(newPlayer);
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