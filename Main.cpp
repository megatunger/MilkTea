#include <SDL.h>
#include <iostream>
#include <Game.h>
#undef main

Game *game = nullptr;

int main(int argc, const char * argv[]) {
	srand(time(NULL));
	const int FPS = 120;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;
	game = new Game();
	game->init("Milk Tea Catcher", 1024, 768, false);

	while (game->running()) {

		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}

	game->clean();
	return 0;
}