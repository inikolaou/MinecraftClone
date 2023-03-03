#ifndef GAME_H
#define GAME_H

#include <utils/Window.h>

class Game
{
public:
	Game();
	~Game();

	void run();
private:
	Window* window;
};

#endif /* GAME_H */