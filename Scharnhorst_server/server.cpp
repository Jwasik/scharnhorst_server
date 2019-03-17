#include "pch.h"
#include "server.h"

Server::Server()
{
}


Server::~Server()
{
}


void Server::sendTcpToEveryone(sf::Packet &packet)
{
	for (auto & client : clients)
	{
		client->sendTcp(packet);
	}
}

void Server::sendUdpToEveryone(sf::Packet &packet)
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
			}
			helloPacket.clear();
			helloPacket << "HI_" << serverUdpPort;
			connectingClient->sendTcp(helloPacket);

			connectingClient->setBlocking(false);

			{
				std::lock_guard<std::mutex> lock(this->mutex);
				clients.push_back(connectingClient);
			}
		}

	}
}