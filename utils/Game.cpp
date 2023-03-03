#include "Game.h"

Game::Game() {
	window = new Window(1024, 768, "Minecraft");
}

void Game::run() {
	window->run();
}

Game::~Game() {
	delete window;
}