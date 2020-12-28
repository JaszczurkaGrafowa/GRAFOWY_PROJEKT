#include "Graph.hpp"

int losuj(int a, int b) {//wiadomo
	return rand() % (b - a + 1) + a;
}

float getAngleByCoordinates(float x, float y)//zwraca kat miedzy osia y+ zgodnie z ruchem wskazowek zegara do punku (x,y)
{
	if (x == 0) {
		if (y > 0) return 0.f;
		else return 180.f;
	}
	if (y == 0) {
		if (x > 0) return 90.f;
		else return 270.f;
	}
	if (x > 0 && y > 0) return 90.f - atan(fabs(y / x)) * M_RAD;
	if (x > 0 && y < 0) return 90.f + atan(fabs(y / x)) * M_RAD;
	if (x < 0 && y < 0) return 270.f - atan(fabs(y / x)) * M_RAD;
	if (x < 0 && y > 0) return 270.f + atan(fabs(y / x)) * M_RAD;
}

float getLenght(sf::Vector2f p1, sf::Vector2f p2) {//odleglosc miedzy dwoma punktami w przestrzeni
	return sqrtf(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void Graph::draw(sf::RenderWindow& window) {
	for (int i = 1; i <= n; i++) //TO RYSUJE KRAWEDZIE //NIE DOTYKAC //trzeba dodac tryb krawedzi skierowanej xD
		for (int j = 0; j < graf[i].size(); j++) {
			if (isDirected == 0 && i > graf[i][j].first) continue;//zeby dwa razy tej samej nie rysowac w nieskierowanym
			sf::Vector2f pos1 = grafika[i].position, pos2 = grafika[graf[i][j].first].position;
			float len = getLenght(pos1, pos2);
			float angle = getAngleByCoordinates(pos2.x - pos1.x, pos1.y - pos2.y);
			sf::RectangleShape kraw(sf::Vector2f(len, 4));
			kraw.setOrigin(0, 2.f);
			kraw.setPosition(pos1);
			kraw.setFillColor(sf::Color::Black);
			kraw.setRotation(angle - 90);
			window.draw(kraw);
		}
	rep(i, 1, n) { //wierzcholki
		window.draw(grafika[i].circle);
		window.draw(grafika[i].text);
	}
}

void Graph::addEdge(int v, int u, int w, int isDirected) {
	graf[v].push_back({ u,w });
	if (!isDirected) graf[u].push_back({ v,w });
}

void Graph::addVertice() {
	grafika[++n] = Vertex(n, sf::Vector2f((float)losuj(100, 900), (float)losuj(100, 500)), &textures[0], font);
}

void Graph::calculateForces() {
	rep(i, 1, n) force[i] = { 0.f,0.f };
	//przyciaganie do srodka 800 400
	rep(i, 1, n) {
		float distance = getLenght(sf::Vector2f(800, 400), grafika[i].position);
		float forceValue = gravityForce(distance);
		sf::Vector2f forceVector = sf::Vector2f(800, 400) - grafika[i].position;
		float angle = getAngleByCoordinates(grafika[i].position.x - 800, grafika[i].position.y - 400);
		force[i] -= sf::Vector2f(forceValue * sin(angle / M_RAD), forceValue * cos(angle / M_RAD));//- a nie + bo grawitacja przyciaga FIZYKA 
	}

	//odpychanie sie wierzcholkow
	rep(i, 1, n)
		rep(j, 1, n) {
		// i -> j
		float distance = getLenght(grafika[i].position, grafika[j].position);
		float forceValue = repulsionForce(distance);
		sf::Vector2f forceVector_i_to_j = grafika[i].position - grafika[j].position;
		sf::Vector2f forceVector_j_to_i = grafika[j].position - grafika[i].position;
		float angle1 = getAngleByCoordinates(grafika[i].position.x - grafika[j].position.x, grafika[i].position.y - grafika[j].position.y);
		float angle2 = (angle1 + 180);
		if (angle2 > 360) angle2 -= 180;
		force[i] += sf::Vector2f(forceValue * sin(angle1 / M_RAD), forceValue * cos(angle1 / M_RAD));
		force[j] += sf::Vector2f(forceValue * sin(angle2 / M_RAD), forceValue * cos(angle2 / M_RAD));

		//j -> i
	}

	//przyciaganie sie na krawedziach
	rep(i, 1, n)
		rep(j, 1, n) {
		bool czyJest = 0;
		for (auto it : graf[i]) if (it.first == j) czyJest = 1;
		if (czyJest == 0) continue;
		float distance = getLenght(grafika[i].position, grafika[j].position);
		float forceValue = attractionForce(distance);
		sf::Vector2f forceVector_i_to_j = grafika[i].position - grafika[j].position;
		sf::Vector2f forceVector_j_to_i = grafika[j].position - grafika[i].position;
		float angle1 = getAngleByCoordinates(grafika[i].position.x - grafika[j].position.x, grafika[i].position.y - grafika[j].position.y);
		float angle2 = (angle1 + 180);
		if (angle2 > 360) angle2 -= 180;
		force[i] -= sf::Vector2f(forceValue * sin(angle1 / M_RAD), forceValue * cos(angle1 / M_RAD));
		force[j] -= sf::Vector2f(forceValue * sin(angle2 / M_RAD), forceValue * cos(angle2 / M_RAD));
	}

}

void Graph::applyForces() {
	rep(i, 1, n) {
		sf::Vector2f delta = sf::Vector2f(force[i].x * 0.01, force[i].y * 0.01);
		if (delta.x < 0.05)
			delta.x = 0;

		if (delta.y < 0.05)
			delta.y = 0;

		grafika[i].position += delta;
		grafika[i].circle.setPosition(grafika[i].circle.getPosition() + delta);
		grafika[i].text.setPosition(grafika[i].text.getPosition() + delta);
	}
}

float Graph::repulsionForce(float distance) {
	if (distance >= 100) return 0;
	return ((distance - 100) * (distance - 100) / 1 + 50.f);
}

float Graph::attractionForce(float distance) {
	if (distance <= 100) return 0;
	return ((distance - 100) * (distance - 100) / 5 + 50.f);
}

float Graph::gravityForce(float distance) {
	return 100;
}

Graph::Graph() {
	font.loadFromFile("Fonts/ABeeZee-Regular.ttf");
	textures[0].loadFromFile("Textures/vertex.png");//kolory wierzcholków, enum na kolory
	textures[0].setSmooth(true);
	isDirected = 0;//wstepnie na nieskierowany
	isWeighted = 0;//wstepnie na niewazony
	n = 0;
	rep(i, 1, 12)
		addVertice();
	addEdge(1, 2, 1, 0);
	addEdge(2, 3, 1, 0);
	addEdge(3, 4, 1, 0);
	addEdge(1, 3, 1, 0);
}

Vertex::Vertex(int num, sf::Vector2f p, sf::Texture* texturePtr, sf::Font& font) {
	position = p;
	circle.setTexture(*texturePtr);
	circle.setOrigin(circle.getGlobalBounds().width / 2, circle.getGlobalBounds().height / 2);
	circle.setPosition(p);
	text.setFont(font);
	text.setString(std::to_string(num));
	text.setCharacterSize(30);
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(p.x - 3, p.y - 9);
}

Vertex::Vertex() {}