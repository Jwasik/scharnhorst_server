#include "pch.h"
#include "server.h"
#include <windows.h>

Server::Server()
{
	this->bulletData.push_back(std::pair<std::string,Bullet>("test",Bullet()));
	this->barrelData.push_back(std::pair<std::string, Barrel>("test",Barrel()));
	this->turretData.push_back(std::pair<std::string, Turret>("test",Turret()));	
	this->shipData.push_back(std::pair<std::string, Ship>("test", Ship()));

	if (!loadBullets())std::cout << "error while loading bullets" << std::endl;
	if (!loadBarrels())std::cout << "error while loading barrels" << std::endl;
	if (!loadTurrets())std::cout << "error while loading turrets" << std::endl;
	if (!loadShips())std::cout << "error while loading ships" << std::endl;
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

void Server::generateBullet(bulletInfo &info)
{
	std::shared_ptr<Bullet> newBullet = std::make_shared<Bullet>(this->findBullet(info.name));
	newBullet->setBulletInfo(info);
	this->bullets.push_back(*newBullet);
}

void Server::sendingEvent()
{
	for (auto & player : this->players)
	{
		sendUdpToEveryone(player->preparePOSpacket());
	}
}

void Server::sendTcpToEveryone(sf::Packet packet)
{
	std::lock_guard<std::mutex> lock(this->TcpMutex);
	for (auto & client : clients)
	{
		client->sendTcp(packet);
	}
}

void Server::sendUdpToEveryone(sf::Packet packet)
{
	for (auto & client : clients)
	{
		client->sendUdp(packet);
	}
}

void Server::receiveTcpMessages()
{
	sf::Packet messagePacket;
	sf::Clock connectionClock;
	std::string message = "";
	std::lock_guard<std::mutex> lock(this->TcpMutex);

	connectionClock.restart();
	while (connectionClock.getElapsedTime().asMilliseconds() < 30)
	{

		for (auto & client : clients)
		{
			client->receiveTcp(messagePacket);
			sf::Packet copiedPacket = messagePacket;

			if (messagePacket >> message)
			{
				client->resetConnectionClock();
				if (message == "BUL")
				{
					copiedPacket >> message;
					for (auto & client : clients)
					{
						client->sendTcp(copiedPacket);
					}
					bulletInfo receivedData;
					copiedPacket >> receivedData;
					this->generateBullet(receivedData);
				}
				/*if (message == "AFK")
				{
				//i tak siê resetuje zegar
				}*/
				messagePacket.clear();
				copiedPacket.clear();
			}
		}

	}
}

void Server::receiveUdpMessages()
{
	sf::Packet messagePacket;
	sf::Clock connectionClock;
	sf::IpAddress add(sf::IpAddress::None);
	unsigned short port = 0;
	connectionClock.restart();
	std::string message = "";
	while (connectionClock.getElapsedTime().asMilliseconds() < 30)
	{
			this->inUdpSocket.receive(messagePacket, add, port); 
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
					player->getShip()->setHitboxPosition(position);
					player->getShip()->hitbox[0].rotate(angle - player->getShip()->shape.getRotation());
					player->getShip()->hitbox[1].rotate(angle - player->getShip()->shape.getRotation());
					player->getShip()->setPosition(position);
					player->getShip()->setRotation(angle);
					

					player->setSightAngle(cannonAngle);
				}
				if (message == "PPS")
				{
					unsigned int count = 0;
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

void Server::serverLoop()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML test");
	sf::View view;
	view.setSize(sf::Vector2f(4000, 4000));
	window.setView(view);
	window.setActive();
	sf::Clock delta;
	delta.restart();
	float deltaTime;


	sf::Clock actionClock;
	sf::Clock afkClock;
	sf::Clock deltaTimeClock;
	actionClock.restart();
	afkClock.restart();
	deltaTimeClock.restart();

	clients.clear();

	inUdpSocket.setBlocking(false);
	inUdpSocket.bind(sf::UdpSocket::AnyPort);
	this->serverUdpPort = inUdpSocket.getLocalPort();

	std::thread listening(&Server::joinClients, this, std::ref(this->clients));

	sf::IpAddress IP = sf::IpAddress::getLocalAddress();



	std::cout << "TCP working on " << IP << ':' << "8888" << std::endl;
	std::cout << "UDP working on " << IP << ':' << this->inUdpSocket.getLocalPort() << std::endl;
	

	for (auto & player : players)
	{
		std::cout << player->getShip()->hitbox[0].punkt1.x << " " << player->getShip()->hitbox[0].punkt1.y << std::endl;
		std::cout << player->getShip()->hitbox[0].punkt2.x << " " << player->getShip()->hitbox[0].punkt2.y << std::endl;

		std::cout << player->getShip()->hitbox[1].punkt1.x << " " << player->getShip()->hitbox[1].punkt1.y << std::endl;
		std::cout << player->getShip()->hitbox[1].punkt2.x << " " << player->getShip()->hitbox[1].punkt2.y << std::endl << std::endl;
	}
	//MAIN LOOP
	while (1)
	{
		/*for (auto & player : players)
		{
			std::cout << player->getShip()->hitbox[0].punkt1.x - player->getShip()->shape.getPosition().x << " " << player->getShip()->hitbox[0].punkt1.y - player->getShip()->shape.getPosition().y << std::endl;
			std::cout << player->getShip()->hitbox[0].punkt2.x - player->getShip()->shape.getPosition().x << " " << player->getShip()->hitbox[0].punkt2.y - player->getShip()->shape.getPosition().y << std::endl;

			std::cout << player->getShip()->hitbox[1].punkt1.x - player->getShip()->shape.getPosition().x << " " << player->getShip()->hitbox[1].punkt1.y - player->getShip()->shape.getPosition().y << std::endl;
			std::cout << player->getShip()->hitbox[1].punkt2.x - player->getShip()->shape.getPosition().x << " " << player->getShip()->hitbox[1].punkt2.y - player->getShip()->shape.getPosition().y << std::endl << std::endl;
			break;
		}*/
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		for (auto & bullet : bullets)
		{
			bullet.fly(deltaTime);
		}

		for (auto & bullet : bullets)
		{
			//window.draw(pol);
			bullet.draw(window);
			//std::cout << bullet.tracer.punkt1.x << " " << bullet.tracer.punkt1.y << std::endl;
		} 
		for (auto & player : players)
		{
			//std::cout << player->getShip()->hitbox[0].punkt1.x << " " << player->getShip()->hitbox[0].punkt1.y << std::endl;
			player->draw(window);
			//window.draw(player->getShip()->hitbox[0].line);
			//window.draw(player->getShip()->hitbox[1].line);

		}
		window.display();
		window.clear();

		deltaTime = delta.restart().asSeconds();



		sendingEvent();
		receiveUdpMessages();
		receiveTcpMessages();
		//Kolizja
		for (auto & player : players)
		{
			auto playerShip = player->getShip();
			auto it = bullets.begin();
			for (auto it = bullets.begin(); it!=bullets.end();it++)
			{

				if (it->ownerId == player->getPlayerId())
				{
					continue;
				}
				if (playerShip->hitbox[0].intersects(it->tracer) || playerShip->hitbox[1].intersects(it->tracer))
				{
					std::cout <<it->ownerId << std::endl;
					player->subtractHP(it->getDamage());
					this->sendTcpToEveryone(this->prepareHITpacket(player,*it));
					it=bullets.erase(it);	
					if (it == bullets.end())break;
				}
			}
		}

		if (afkClock.getElapsedTime().asSeconds() > 10)//szuka od³¹czonych klientów
		{
			for (auto it = clients.begin(); it != clients.end(); it++)
			{
				if ((*it)->getTimeFromLastActivity().asSeconds() > 5)
				{
					std::cout << "player " << (*it)->getPlayerId() << " unavaible from " ;
					std::cout << (*it)->getTimeFromLastActivity().asSeconds() <<" seconds"<< std::endl;
					std::cout << "disconnecting AFK player" << std::endl;
					sf::Packet exitPacket;
					exitPacket << "EXT";
					exitPacket << (*it)->getPlayerId();
					this->sendTcpToEveryone(exitPacket);

					for (auto it2 = players.begin(); it2 != players.end(); it2++)
					{
						if ((*it2)->getPlayerId() == (*it)->getPlayerId())
						{
							players.erase(it2);
							break;
						}
					}
					clients.erase(it);
					std::cout<<"lasts " << clients.size() <<" clients"<< std::endl;
					break;
				}
			}
			afkClock.restart();
		}

		if (actionClock.getElapsedTime().asSeconds() > 4)
		{
			sf::Packet afkPacket;
			afkPacket << "AFK";//zapytanie czy klient jest aktywny
			this->sendTcpToEveryone(afkPacket);
			actionClock.restart();
		}
		
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
			std::lock_guard<std::mutex> lock(this->TcpMutex);
			std::cout << "client accepted" << std::endl;
			sf::Clock connectionClock;
			connectionClock.restart();
			sf::Packet helloPacket;
			helloPacket.clear();

			unsigned short clientUdpPort;
			std::string message;

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

			helloPacket.clear();
			helloPacket << "HI_" << serverUdpPort;
			std::cout << "sending HI_ : " << serverUdpPort << std::endl;
			connectingClient->sendTcp(helloPacket);
			sf::Socket::Status connectedInfo;
			while (connectionClock.getElapsedTime().asSeconds() < 2)
			{
				
				connectedInfo = connectingClient->receiveTcp(helloPacket);
				if (connectedInfo == sf::Socket::Status::Done)
				{

					helloPacket >> message;
					if (message == "PLA")
					{
						unsigned int newPlayerId;
						std::string newPlayerName, playerShipModel, playerShipName;
						helloPacket >> newPlayerId;
						helloPacket >> newPlayerName;
						helloPacket >> playerShipModel;//tutaj model
						helloPacket >> playerShipName;
						if (newPlayerId == 0)
						{
							newPlayerId = players.size() + 1;
						}
						std::shared_ptr<Player> newPlayer = std::make_shared<Player>(newPlayerId, newPlayerName);
						newPlayer->getShip()->setName(playerShipName);
						newPlayer->setShip(findShip(playerShipModel));
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

						{
							std::lock_guard<std::mutex> lock(this->mutex);
							players.push_back(newPlayer);
						}

						sf::Packet PLApacket;
						PLApacket.clear();
						PLApacket << "PLA";
						PLApacket << newPlayerId;
						PLApacket << newPlayerName;
						PLApacket << playerShipModel;
						PLApacket << playerShipName;

						for (auto & client : clients)
						{
							client->sendTcp(PLApacket);
						}
						for (auto & player : players)
						{
							connectingClient->sendTcp(player->preparePLApacket());
						}
						connectingClient->setPlayerId(newPlayerId);

						break;
					}
				}
			}
			connectingClient->setBlocking(false);

			if (connectedInfo != sf::Socket::Status::Done)continue;
			{
				std::lock_guard<std::mutex> lock(this->mutex);
				connectingClient->resetConnectionClock();
				clients.push_back(connectingClient);
			}

		}

	}
}

void Server::printPOSPacket(sf::Packet packet)
{
	std::cout << packet.getDataSize() << ' ';
	std::string type = "ERR";
	unsigned int id = 0;
	float x, y, angle, cannonAngle;
	x = y = angle = cannonAngle = 420;
	packet >> type;
	packet >> id;
	packet >> x, y, angle, cannonAngle;
	std::cout << "packet " << type << ' ' << id << ' ' << x << ' ' << y << ' ' << angle << ' ' << cannonAngle << std::endl;
}

sf::Packet Server::prepareHITpacket(std::shared_ptr<Player> &player, Bullet &bullet)
{
	std::string order = "HIT";
	sf::Packet hitPacket;
	hitPacket << order << player->getPlayerId();
	hitPacket << bullet;
	hitPacket<<player->getPlayerHP();
	/*To potem wywaliæ*/
	hitPacket << bullet.getPosition().x;
	hitPacket << bullet.getPosition().y;
	return hitPacket;
}

bool Server::loadBullets()
{
	std::ifstream in("gamedata/bullets.dat");
	if (!in.good())	return 0;

	std::string name, endWord;
	unsigned int pointCount;
	float x, y;
	float speed, damage;

	while (true)
	{
		endWord = "ERR";

		std::getline(in, name);

		in >> pointCount;

		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		sf::ConvexShape bulletShape;
		bulletShape.setPointCount(pointCount);
		bulletShape.setFillColor(sf::Color::Red);

		for (unsigned int i = 0; i < pointCount; i++)
		{
			in >> x;
			in >> y;
			bulletShape.setPoint(i, sf::Vector2f(x, y));
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		//Origin
		in >> x;
		in >> y;
		bulletShape.setOrigin(x, y);
		in >> speed;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> damage;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::getline(in, endWord);

		if (endWord != "END_BULLET")break;
		bulletData.push_back(std::pair<std::string, Bullet>(name, Bullet(name, bulletShape, speed, damage)));
	}
	return 1;
}
bool Server::loadBarrels()
{
	std::ifstream in("gamedata/barrels.dat");
	if (!in.good())	return 0;

	std::string name, mainBulletType, endWord;
	unsigned int point_count, bulletSize;
	while (!in.eof())
	{
		std::shared_ptr<Barrel> newBarrel;

		unsigned int pointCount = 0;
		float x, y;
		sf::ConvexShape barrelShape;
		barrelShape.setFillColor(sf::Color::Blue);

		std::getline(in, name);//nazwa
		std::getline(in, mainBulletType);//nazwa pocisku
		in >> bulletSize;//kaliber
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		in >> pointCount;//iloœæ punktów
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		barrelShape.setPointCount(pointCount);

		float maxx = 0, maxy = 0;//u¿ywane do znalezienia originu

		for (unsigned int i = 0; i < pointCount; i++)//punkty
		{
			in >> x;
			in >> y;

			barrelShape.setPoint(i, sf::Vector2f(x, y));
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (x > maxx)maxx = x;
			if (y > maxy)maxy = y;
		}
		barrelShape.setOrigin(sf::Vector2f(maxx / 2, 0));

		std::getline(in, endWord);//2 razy bo musi przeskoczyæ do nastêpnej lini
		if (endWord != "END_BARREL")return 0;

		barrelData.push_back(std::pair<std::string, Barrel>(name, Barrel(name, sf::Vector2f(0, 0), barrelShape, findBullet(mainBulletType), bulletSize)));
		barrelData.back().second.length = maxy;
	}
	return 1;
}
bool Server::loadTurrets()
{
	std::ifstream in("gamedata/turrets.dat");
	if (!in.good())return 0;

	std::string name, cannonType, endWord;
	unsigned int point_count;
	while (!in.eof())
	{
		endWord = "ERR";
		std::shared_ptr<Turret> newTurret;

		unsigned int pointCount = 0;
		float x, y;
		sf::ConvexShape turretShape;

		turretShape.setFillColor(sf::Color::Red);

		std::getline(in, name);//nazwa

		in >> pointCount;//iloœæ punktów
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		turretShape.setPointCount(pointCount);
		for (unsigned int i = 0; i < pointCount; i++)//punkty
		{
			in >> x;
			in >> y;
			turretShape.setPoint(i, sf::Vector2f(x, y));
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		in >> x;
		in >> y;
		turretShape.setOrigin(sf::Vector2f(x, y));//origin

		float parameters[3];
		in >> parameters[0];
		in >> parameters[1];
		in >> parameters[2];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		newTurret = std::make_shared<Turret>(name, name, turretShape, parameters);

		unsigned int cannonCount = 0;
		in >> cannonCount;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		for (unsigned int i = 0; i < cannonCount; i++)//dzia³a
		{
			std::getline(in, cannonType);
			in >> x;
			in >> y;
			newTurret->addBarrel(findBarrel(cannonType), sf::Vector2f(x, y - findBarrel(cannonType).length) + findBarrel(cannonType).getOrigin());
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		std::getline(in, endWord);
		if (endWord != "END_TURRET")return 0;
		turretData.push_back(std::pair<std::string, Turret>(name, Turret(*newTurret)));
	}
	return 1;
}
bool Server::loadShips()
{
	std::ifstream in("gamedata/ships.dat");
	if (!in.good())return 0;

	std::string name, endWord;
	unsigned int point_count;
	while (true)
	{
		endWord = "ERR";
		std::shared_ptr<Ship> newShip;

		unsigned int pointCount = 0;
		float x, y;
		sf::ConvexShape shipShape;

		shipShape.setFillColor(sf::Color::Red);

		std::getline(in, name);//nazwa
		float parameters[6];//parametry
		in >> parameters[0];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> parameters[1];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> parameters[2];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> parameters[3];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> parameters[4];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		in >> parameters[5];
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		in >> pointCount;//iloœæ punktów
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		shipShape.setPointCount(pointCount);
		float maxx = 0, maxy = 0;
		for (unsigned int i = 0; i < pointCount; i++)//punkty
		{
			in >> x;
			in >> y;
			if (x > maxx)maxx = x;
			if (y > maxy)maxy = y;
			shipShape.setPoint(i, sf::Vector2f(x, y));
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		shipShape.setOrigin(sf::Vector2f(maxx / 2, maxy / 2));//origin

		newShip = std::make_shared<Ship>(name, parameters, shipShape);

		unsigned int turretCount = 0;
		in >> turretCount;
		in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		std::string turretName;
		float turretRestrictedArea[2];
		for (unsigned int i = 0; i < turretCount; i++)
		{
			std::getline(in, turretName);
			in >> x;
			in >> y;
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			in >> turretRestrictedArea[0];
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			in >> turretRestrictedArea[1];
			in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::shared_ptr<Turret> newTurret = std::make_shared<Turret>(this->findTurret(turretName));
			newTurret->setRestrictedArea(parameters);
			newShip->addTurret(newTurret, sf::Vector2f(x, y));
		}

		std::getline(in, endWord);
		if (endWord != "END_SHIP")break;
		shipData.push_back(std::pair<std::string, Ship>(name, Ship(*newShip)));
	}
	return 1;
}
Bullet Server::findBullet(std::string name)
{
	for (auto &bullet : bulletData)
	{
		if (bullet.first == name)return bullet.second;
	}
	return bulletData.front().second;
}
Barrel Server::findBarrel(std::string name)
{
	for (auto &barrel : barrelData)
	{
		if (barrel.first == name)return barrel.second;
	}
	return barrelData.front().second;
}
Turret Server::findTurret(std::string name)
{
	for (auto &turret : turretData)
	{
		if (turret.first == name)return turret.second;
	}
	return turretData.front().second;
}
Ship Server::findShip(std::string name)
{
	for (auto &ship : shipData)
	{
		if (ship.first == name)return ship.second;
	}
	return shipData.front().second;
}