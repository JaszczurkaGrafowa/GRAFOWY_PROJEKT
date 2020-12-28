#ifndef GRAPH_HPP
#define GRAPH_HPP


#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
typedef std::pair<int, int> pii;
#define rep(i,a,b) for(int i = a; i <= b; i++)
#define N 1001
#define M_RAD 57.2957


class Vertex {
public:
	sf::Sprite circle;
	sf::Vector2f position;
	sf::Text text;
	Vertex(int, sf::Vector2f, sf::Texture*, sf::Font&);
	Vertex();
};

class Graph {
public:
	std::vector<pii> graf[N];				//para sasiad - waga //zwykla informacja o grafie
	sf::Vector2f force[N];		//sila dzialajaca na wierzcholki
	Vertex grafika[N];						//cala grafika grafu - kolory wierzcholkow, ich pozycje na ekranie itp
	bool							//
		isDirected,						//chyba wiadomo o co chodzi
		isWeighted;						//
	int n;									//liczba wierzcholkow
	sf::Texture textures[5];				//tekstury wierzcholkow
	sf::Font font;							//czcionka
	std::vector<sf::Vector2f> forces[N];

	float repulsionForce(float);//sila odpychania miedzy wierzcholkami
	float attractionForce(float);//przyciaganie miedzy krawedziami
	float gravityForce(float);//grawitacja na srodek

	void calculateForces();
	void applyForces();
	void draw(sf::RenderWindow&);
	void addEdge(int, int, int, int);
	void addVertice();
	Graph();
};
#endif