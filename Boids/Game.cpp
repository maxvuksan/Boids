#include "Game.h"
#include "Boid.h"


Game::Game() :
	W_WIDTH(700),
	W_HEIGHT(700),
	D_WIDTH(700),
	D_HEIGHT(700),
	DELTA_TIME(0),
	BOID_COUNT(30),
	MAX_FLOCK_COUNT(5),
	FLOCK_COUNT(2),
	SHOW_TRAILS(true),
	rect_count(10),
	SIMULATION_SPEED(1)
{
	WINDOW = new sf::RenderWindow(sf::VideoMode(W_WIDTH, W_HEIGHT), "Boid Simulation");
	ImGui::SFML::Init(*WINDOW);

	DISPLAY = new sf::RenderTexture;
	DISPLAY->create(D_WIDTH, D_HEIGHT);
	DISPLAY->setSmooth(true);

	if (!WOBBLE.loadFromFile("wobble_shader.frag", sf::Shader::Fragment)) {
		throw "failed loading";
	}

	srand(time(NULL));
	int random_background = rand() % 5;
	switch (random_background) {

		case 0:
			BACKGROUND_COLOUR = sf::Color(65,45, 51);
			break;
		case 1:
			BACKGROUND_COLOUR = sf::Color(60, 45, 51);
			break;
		case 2:
			BACKGROUND_COLOUR = sf::Color(51, 45, 60);
			break;
		case 3:
			BACKGROUND_COLOUR = sf::Color(54, 64, 56);
			break;
		case 4:
			BACKGROUND_COLOUR = sf::Color(53, 54, 56);
			break;
	}



	//defining flock colours
	FLOCK_COLOURS.resize(MAX_FLOCK_COUNT);
	for (int i = 0; i < MAX_FLOCK_COUNT; i++) {
		sf::Color c;
		switch (i) {
			case 0:
				c =  sf::Color(145, 223, 218);
				break;
			case 1:
				c = sf::Color(200, 158, 221);
				break;
			case 2:
				c = sf::Color(168, 223, 150);
				break;
			case 3:
				c = sf::Color(232, 190, 144);
				break;
			case 4:
				c = sf::Color(232, 119, 119);
				break;
			default:
				c = sf::Color::White;
		}
		FLOCK_COLOURS[i] = c;
	}

	background_rect.setFillColor(sf::Color(0, 0, 0, 20));
	background_rect.setSize(sf::Vector2f(D_WIDTH / (float)rect_count, D_HEIGHT / (float)rect_count));
}

void Game::start() {
	reset_boids();
}

sf::Color Game::get_colour_by_flock(int flock) {
	return FLOCK_COLOURS[flock];
}

void Game::reset_boids() {
	for (int i = 0; i < ENTITIES.size(); i++) {
		delete ENTITIES[i];
	}
	ENTITIES.clear();
	for (int i = 0; i < BOID_COUNT; i++) {
		add_entity(new Boid);
	}
}

void Game::run() {
	start();

	while (WINDOW->isOpen()) {

		DISPLAY->clear(BACKGROUND_COLOUR);

		sf::Time td = CLOCK.restart();
		ImGui::SFML::Update(*WINDOW, td);
		DELTA_TIME = td.asSeconds() * SIMULATION_SPEED;
		TIME += DELTA_TIME;

		this->update();
		this->poll_events();
		this->render_ui();

		DISPLAY->display();

		sf::Sprite display_sprite(DISPLAY->getTexture());
		//by drawing it with an offset it prevents the shader from screwing up the edges
		short shader_edge_buffer = 10;
		display_sprite.setPosition(-shader_edge_buffer / 2, -shader_edge_buffer / 2);
		display_sprite.setScale(W_WIDTH / ((float)D_WIDTH - shader_edge_buffer), W_HEIGHT / ((float)D_HEIGHT - shader_edge_buffer));
		
		if (TIME > 99999) {
			TIME = 0; //preventing overflow of value
		}

		WOBBLE.setUniform("u_time", TIME);
		WOBBLE.setUniform("u_texture", DISPLAY->getTexture());
		WINDOW->draw(display_sprite, &WOBBLE);

		ImGui::SFML::Render(*WINDOW);

		WINDOW->display();
	}
	ImGui::SFML::Shutdown();
}

void Game::poll_events() {
	sf::Event event;
	while (WINDOW->pollEvent(event))
	{
		ImGui::SFML::ProcessEvent(event);
		if (event.type == sf::Event::Closed) {
			WINDOW->close();
		}

		for (int i = 0; i < ENTITIES.size(); i++) {
			ENTITIES[i]->catch_event(event);
		}
	}

}

void Game::update() {

	//drawing background
	for (int x = 0; x < rect_count; x++) {
		for (int y = 0; y < rect_count; y++) {
			if ((x + y) % 2 == 0) {
				background_rect.setPosition(sf::Vector2f(x * D_WIDTH / (float)rect_count, y * D_HEIGHT / (float)rect_count));
				DISPLAY->draw(background_rect);
			}
		}
	}

	for (int i = 0; i < ENTITIES.size(); i++) {
		ENTITIES[i]->update();
		ENTITIES[i]->update(*DISPLAY);
	}
}

void Game::render_ui() {
	ImGui::Begin("##", NULL, ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowPos(ImVec2(5, 5));
	ImGui::SetWindowSize(ImVec2(0, 0));
	ImGui::Checkbox("SHOW TRAILS", &SHOW_TRAILS);
	ImGui::Checkbox("SHOW RADIUS", &SHOW_RADIUS);
	ImGui::SliderFloat("SPEED", &SIMULATION_SPEED, 1, 4);

	ImGui::Separator();

	ImGui::SliderInt("COUNT", &BOID_COUNT, 1, 200);
	
	if (ImGui::Button("-")) {
		if (FLOCK_COUNT > 1) {
			FLOCK_COUNT--;
		}
	}
	ImGui::SameLine();
	ImGui::Text("FLOCKS");
	ImGui::SameLine();
	if (ImGui::Button("+")) {
		if (FLOCK_COUNT < MAX_FLOCK_COUNT) {
			FLOCK_COUNT++;
		}
	}

	for (int i = 0; i < FLOCK_COUNT; i++) {

		if (i % 5 != 0) {
			ImGui::SameLine();
		}

		//preview for each flocks colour
		float colour[3] = { FLOCK_COLOURS[i].r/(float)255, FLOCK_COLOURS[i].g/ (float)255, FLOCK_COLOURS[i].b/ (float)255 };
		ImGui::ColorEdit3("##", colour, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip);
	}

	if (ImGui::Button("RESET")) {
		reset_boids();
	}

	ImGui::End();

}

void Game::add_entity(Entity* entity) {
	ENTITIES.push_back(entity);
	entity->link_game(this);
	entity->start();
}

std::vector<Entity*> Game::get_ENTITIES() {
	return ENTITIES;
}

sf::Vector2i Game::get_W_SIZE() {
	return sf::Vector2i(W_WIDTH, W_HEIGHT);
}
sf::Vector2i Game::get_D_SIZE() {
	return sf::Vector2i(D_WIDTH, D_HEIGHT);
}

Game::~Game() {
	delete WINDOW;
	delete DISPLAY;
	for (int i = 0; i < ENTITIES.size(); i++) {
		delete ENTITIES[i];
	}
}