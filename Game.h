#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include <cstdlib>
#include <ctime>
#include <SDL_ttf.h>
#include <cstdio>
#include <vector>


struct MilkTeaObject {
	const char *path;
	int type;
	int PosDrop;
	int cnt;
	int velocity;
	int Pos;
	bool NotEnd;
	SDL_Rect destR;
	int x, y;
	SDL_Texture* MilkTeaTex;
};

class Game {
public:
	Game();
	~Game();

	bool checkCollision(SDL_Rect a, SDL_Rect b);
	void RenderMilkTea(const char *path, int k);
	std::string RandomPathMilkTea();
	void GenerateMilkTeaVector();
	void init(const char *title, int width, int height, bool fullscreen);
	void handleEvents();
	void LoadNextScreen();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

private:
	int DropMaxLv[11] = { 10, 15, 15, 20, 30, 35, 40, 50, 70, 80, 100};
	int maxImage = 10;
	bool isRunning = false;
	bool StartGame = false;
	int TeaWidth, TeaHeight;
	int GioWidth, GioHeight;
	std::vector<MilkTeaObject> MilkTea;
	std::clock_t start; // timer for counting
	double duration;
	const double scaleFactor = 0.3;//Scale factor milk tea
	int posxMouse, posyMouse;
	unsigned long long int score=0;
	int lives = 6;
	int level = 0;
	int counter;
	SDL_Window *window;
	SDL_Renderer *renderer;
};
