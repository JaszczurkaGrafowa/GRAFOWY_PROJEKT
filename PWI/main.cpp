#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

#include"Graph.hpp"

int main()
{
	/*sf::Image image;
	if (image.loadFromFile("Textures/vertex.png"))
		std::cout << "mam plik!\n";
	sf::Vector2u size = image.getSize();
	for(int i = 0; i < size.y; i++)
		for (int j = 0; j < size.x; j++) {
			sf::Color pixel = image.getPixel(j, i);
			if (image.getPixel(j, i) == sf::Color((unsigned int)255, (unsigned int)255, (unsigned int)255, (unsigned int)255))
				image.setPixel(j,i,sf::Color(255, 255,255,0));
		}
	image.saveToFile("Textures/vertex.png");
	return 0;*/
	Graph G;
	srand(time(NULL));
	if (!G.font.loadFromFile("Fonts/ABeeZee-Regular.ttf"))
		throw("NIE MA CZCIONKI\n");
	sf::RenderWindow window(sf::VideoMode(1600, 800), "Projekt PWI");
	window.setFramerateLimit(165);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		G.calculateForces();
		G.applyForces();
		window.clear(sf::Color::White);
		G.draw(window);
		window.display();
	}

	return 0;
	return 1;
}
//ssagssdagsdadgasdg
