#include "Game.h"
//#include "TextureManager.h"
SDL_Texture* splashScreen;
SDL_Texture* nextScreen;
SDL_Texture* gio;

SDL_Rect mouseR;


Game::Game()
{}


Game::~Game()
{}

bool Game::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
	{
		return false;
	}

	if (topA >= bottomB)
	{
		return false;
	}

	if (rightA <= leftB)
	{
		return false;
	}

	if (leftA >= rightB)
	{
		return false;
	}

	//If none of the sides from A are outside B
	return true;
}



void Game::init(const char *title, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
				{
					SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
		isRunning = true;
	}

	/*Loading SplashScreen*/
		SDL_Surface* tmpSurface = IMG_Load("SplashScreen.bmp");
		splashScreen = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		SDL_FreeSurface(tmpSurface);

}

void Game::RenderMilkTea(const char *path, int k) {
	/*Rendering milk tea dropping down*/

	SDL_Surface* tmpSurface = IMG_Load(path);
	TeaWidth = tmpSurface->w;
	TeaHeight = tmpSurface->h;

	MilkTea[k].MilkTeaTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
}

//Generate random milk tea name
std::string Game::RandomPathMilkTea() {
	std::string path = ".png";
	int RandTypeMilkTea = rand() % (maxImage) + 1;
	std::string s = std::to_string(RandTypeMilkTea);
	path.insert(0, s);
	return path;
}

void Game::GenerateMilkTeaVector()
{
	counter = DropMaxLv[level];
	/*Render milk tea with random path*/
	for (int i = 0; i < DropMaxLv[level]; i++) {
		std::string path1 = RandomPathMilkTea();
		MilkTea.push_back(MilkTeaObject()); // Create Milk Tea Object
		MilkTea[i].path = path1.c_str(); // Assign path to milk tea image
		MilkTea[i].PosDrop = rand() % (900 - 300 + 1) + 300; //Generate random position to drop
		MilkTea[i].cnt = -(rand() % (2000 - 100 + 1) + 100); // generate position dropping when scrolling
		MilkTea[i].NotEnd = true;

		RenderMilkTea(MilkTea[i].path, i);
	}
}

void Game::LoadNextScreen() {
	/*Loading game screenplay layer 1 after click start*/
	    SDL_Surface* tmpSurface = IMG_Load("Background.bmp");
		nextScreen = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	    SDL_FreeSurface(tmpSurface);


	/*Render the bag*/
			SDL_Surface* alphaSurface = IMG_Load("gio.png");
			GioWidth = alphaSurface->w;
			GioHeight = alphaSurface->h;
			gio = SDL_CreateTextureFromSurface(renderer, alphaSurface);
			SDL_FreeSurface(alphaSurface);

	GenerateMilkTeaVector();
}


void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			{
				isRunning = false;
				break;
			}
		
		case (SDL_MOUSEBUTTONDOWN):
		{
			if (event.button.x>=410 && event.button.x<=615)
				if (event.button.y >= 410 && event.button.y <= 489)
				{
					StartGame = true;
					LoadNextScreen();
					start = std::clock();
				}
		}

		case (SDL_MOUSEMOTION): {
			posxMouse = event.button.x;
			posyMouse = event.button.y;
			//std::cout << posxMouse << "---" << posyMouse << std::endl;
		}
		default:
			break;
	}
}


void Game::update()
{

	/*Update time*/
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	//std::<< "printf: " << duration << '\n';

	/*Di chuyen coc tra sua*/
	if (StartGame) {
		for (int i = 0; i < DropMaxLv[level]; i++) {
			MilkTea[i].cnt++;
			MilkTea[i].destR.h = (int)(TeaHeight*scaleFactor);
			MilkTea[i].destR.w = (int)(TeaWidth*scaleFactor);
			MilkTea[i].destR.x = MilkTea[i].PosDrop;
			MilkTea[i].destR.y = MilkTea[i].cnt;
		}
	}

	/*Di chuyen cai gio*/
	mouseR.h = GioHeight/2;
	mouseR.w = GioWidth/2;
	if(posxMouse>=295 && posxMouse<=895) mouseR.x = posxMouse; // Avoid object get out of border.
	mouseR.y = 645;
	
	
}
void Game::render()
{
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, splashScreen, NULL, NULL);
	SDL_RenderCopy(renderer, nextScreen, NULL, NULL);
	if (StartGame) {

		for (unsigned int i = 0; i < MilkTea.size(); i++) {
			if (MilkTea[i].NotEnd)
			{
				SDL_RenderCopy(renderer, MilkTea[i].MilkTeaTex, NULL, &MilkTea[i].destR);

				/* if catched milk tea, stop render and add the score*/
				if (checkCollision(MilkTea[i].destR, mouseR))
				{
					MilkTea[i].NotEnd = false;
					score += 10;
					counter--;
					std::cout << "SCORE: " << score << std::endl;
				}

				/*if milk tea touch the stage, stop render it and substract lives*/
				if (MilkTea[i].cnt >= 550) {
					MilkTea[i].NotEnd = false;
					lives--;
					counter--;
					std::cout << "LIVES: " << lives << std::endl;
				}
			}
			if (counter==0 && lives != 0) //go to next level condition
				//@counter==0 means the last milk tea appeared and finish line
				//@lives!=0 means you haven't dead.
			{
				MilkTea.clear();
				level++;
				GenerateMilkTeaVector();

				/*Features in working, when go to next level, change new girl*/
				SDL_Surface* tmpSurface;
				if (level==1) tmpSurface = IMG_Load("Background1.bmp");
				else tmpSurface = IMG_Load("Background2.bmp");
				nextScreen = SDL_CreateTextureFromSurface(renderer, tmpSurface);
				SDL_FreeSurface(tmpSurface);
			}
		}
		SDL_RenderCopy(renderer, gio, NULL, &mouseR);
	}
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}