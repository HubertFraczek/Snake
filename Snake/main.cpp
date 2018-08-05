#include <iostream>
#include <SDL.h>
#include "Renderer.h"
#include <time.h>
#include <stdlib.h>
#include <list>
//#define fps 10
#define screen_width 500
#define screen_height 500
#define block_size 20
void DrawRect(Renderer &renderer, int x, int  y, int w) {
	SDL_Rect temp = { x, y, w, w };
	SDL_RenderFillRect(renderer.get_renderer(), &temp);
}
struct Wonsz {
	int x;
	int y;
};
struct Food {
	int foodX;
	int foodY;
	bool reSpawn = true;
};
void move(bool dead, int direction, list<Wonsz> &wonsz) {
	if (!dead) {
		switch (direction) {
		case 0:
			wonsz.push_front({ (wonsz.front().x / block_size) * block_size, (wonsz.front().y / block_size) * block_size - block_size });
			wonsz.pop_back();
			break;
		case 1:
			wonsz.push_front({ (wonsz.front().x / block_size) * block_size + block_size, (wonsz.front().y / block_size) * block_size });
			wonsz.pop_back();
			break;
		case 2:
			wonsz.push_front({ (wonsz.front().x / block_size) * block_size, (wonsz.front().y / block_size) * block_size + block_size });
			wonsz.pop_back();
			break;
		case 3:
			wonsz.push_front({ (wonsz.front().x / block_size) * block_size - block_size, (wonsz.front().y / block_size) * block_size });
			wonsz.pop_back();
			break;
		}
	}
}
void spawnFood(list<Wonsz> &wonsz, list<Food> &food) {
	bool dontTouchPython = true;
	int foodX = ((rand() % screen_width) / block_size) * block_size;
	int foodY = ((rand() % screen_height) / block_size) * block_size;
	while (1) {
		if (!dontTouchPython) {
			foodX = ((rand() % screen_width) / block_size) * block_size;
			foodY = ((rand() % screen_height) / block_size) * block_size;
			dontTouchPython = true;
		}
		for (auto &j : wonsz) {
			if (j.x == foodX && j.y == foodY) dontTouchPython = false;
		}
		if (dontTouchPython) {
			food.push_back( {foodX, foodY} );
			break;
		}
	}
}
void spawnFoods(list<Wonsz> &wonsz, list<Wonsz> &wonsz2, list<Food> &food) {
	bool dontTouchPython = true;
	int foodX = ((rand() % screen_width) / block_size) * block_size;
	int foodY = ((rand() % screen_height) / block_size) * block_size;
	int x = 0;
	while (x < 2) {
		if (!dontTouchPython) {
			foodX = ((rand() % screen_width) / block_size) * block_size;
			foodY = ((rand() % screen_height) / block_size) * block_size;
			dontTouchPython = true;
		}
		for (auto &j : wonsz) {
			if (j.x == foodX && j.y == foodY) dontTouchPython = false;
		}
		for (auto &j : wonsz2) {
			if (j.x == foodX && j.y == foodY) dontTouchPython = false;
		}
		if (dontTouchPython) {
			food.push_back({ foodX, foodY });
			x++;
			dontTouchPython = false;
		}
	}
}
void collisionWithFood(list<Wonsz> &wonsz, double &fps, list<Food> &food) {
	for (list<Food>::iterator i = food.begin(); i != food.end(); i++) {
		if (i->reSpawn) {
			if (wonsz.front().x == i->foodX && wonsz.front().y == i->foodY) {
				bool dontTouchPython = true;
				while (1) {
					if (!dontTouchPython) {
						i->foodX = ((rand() % screen_width) / block_size) * block_size;
						i->foodY = ((rand() % screen_height) / block_size) * block_size;
						dontTouchPython = true;
					}
					for (auto &j : wonsz) {
						if (j.x == i->foodX && j.y == i->foodY) dontTouchPython = false;
					}
					if (dontTouchPython) break;
				}
				wonsz.push_back({ wonsz.back().x, wonsz.back().y });
				fps += 0.33;
			}
		}
		else {
			if (wonsz.front().x == i->foodX && wonsz.front().y == i->foodY) {
				i = food.erase(i);
				wonsz.push_back({ wonsz.back().x, wonsz.back().y });
				fps += 0.33;
			}
		}
	}
}
void collisionWithMap(list<Wonsz> &wonsz, bool &dead) {
	if (wonsz.front().x < 0 || wonsz.front().x >= screen_width)
		dead = true;
	if (wonsz.front().y < 3 || wonsz.front().y >= screen_height)
		dead = true;
}
void collisionWithSnake(list<Wonsz> &wonsz, bool &dead) {
	for (list<Wonsz>::iterator i = wonsz.begin(); i != wonsz.end(); i++)
		if (i != wonsz.begin() && i->x == wonsz.front().x && i->y == wonsz.front().y) dead = true;
}
void drawSnake(list<Wonsz> &wonsz, bool &dead, Renderer &renderer) {
	if (!dead) {
		int x = 0;
		for (auto &i : wonsz) {
			if (x == 0) {
				SDL_SetRenderDrawColor(renderer.get_renderer(), 255, 0, 0, 255);
				DrawRect(renderer, (i.x / block_size) * block_size, (i.y / block_size) * block_size, block_size);
			}
			else {
				SDL_SetRenderDrawColor(renderer.get_renderer(), 255, 255, 255, 255);
				DrawRect(renderer, i.x, i.y, block_size);
			}
			x++;
		}
	}
	else {
		for (auto &i : wonsz) {
			SDL_SetRenderDrawColor(renderer.get_renderer(), 255, 0, 0, 255);
			DrawRect(renderer, (i.x / block_size) * block_size, (i.y / block_size) * block_size, block_size);
		}
	}
}
void collisionBeetweenSnakes(list<Wonsz> &wonsz, list<Wonsz> &wonsz2, bool &dead, bool &dead2) {
	for (auto &i : wonsz) {
		if (wonsz2.front().x == i.x && wonsz2.front().y == i.y) dead2 = true;
	}
	for (auto &i : wonsz2) {
		if (wonsz.front().x == i.x && wonsz.front().y == i.y) dead = true;
	}
}
void drawFood(Renderer &renderer, list<Food> &food) {
	SDL_SetRenderDrawColor(renderer.get_renderer(), 255, 255, 0, 255);
	for (auto &i : food) {
		DrawRect(renderer, i.foodX, i.foodY, block_size);
	}
}
void ai(list<Wonsz> &wonsz, int &foodX, int &foodY, int &direction) {
	if (wonsz.front().x <= foodX && wonsz.front().y <= foodY) {

	}
	if (wonsz.front().x >= foodX && wonsz.front().y <= foodY) {

	}
	if (wonsz.front().x <= foodX && wonsz.front().y >= foodY) {

	}
	if (wonsz.front().x >= foodX && wonsz.front().y >= foodY) {

	}
}
void controls(list<Wonsz> &wonsz, list<Wonsz> &wonsz2,list<Food> &food, int &direction, int &direction2, bool &dead, bool &dead2, bool &running, bool &twoPlayers, double &fps, SDL_Event &event) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			running = false;
			break;
		}
		if (event.type == SDL_KEYDOWN) {
			if (twoPlayers) {
				if (event.key.keysym.sym == SDLK_UP) {
					if (direction2 != 2) direction2 = 0;
				}
				if (event.key.keysym.sym == SDLK_LEFT) {
					if (direction2 != 1) direction2 = 3;
				}
				if (event.key.keysym.sym == SDLK_DOWN) {
					if (direction2 != 0) direction2 = 2;
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					if (direction2 != 3) direction2 = 1;
				}
			}
			if (event.key.keysym.sym == SDLK_w) {
				if (direction != 2) direction = 0;
			}
			if (event.key.keysym.sym == SDLK_a) {
				if (direction != 1) direction = 3;
			}
			if (event.key.keysym.sym == SDLK_s) {
				if (direction != 0) direction = 2;
			}
			if (event.key.keysym.sym == SDLK_d) {
				if (direction != 3) direction = 1;
			}
			if (event.key.keysym.sym == SDLK_q && dead) {
				wonsz.clear();
				food.clear();
				wonsz = { { ((screen_width / 2) / block_size) * block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + 2 * block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + 3 * block_size, ((screen_height / 2) / block_size) * block_size } };
				direction = 3;
				dead = false;
				fps = 10.0;
				bool dontTouchPython = true;
				if (!twoPlayers) spawnFood(wonsz, food);
				if (twoPlayers) {
					wonsz2.clear();
					wonsz2 = { { ((screen_width / 2) / block_size) * block_size, ((screen_height / 4) / block_size) * block_size },
							   { ((screen_width / 2) / block_size) * block_size + block_size, ((screen_height / 4) / block_size) * block_size },
							   { ((screen_width / 2) / block_size) * block_size + 2 * block_size, ((screen_height / 4) / block_size) * block_size },
						   	   { ((screen_width / 2) / block_size) * block_size + 3 * block_size, ((screen_height / 4) / block_size) * block_size } };
					direction2 = 3;
					dead2 = false;
					spawnFoods(wonsz, wonsz2, food);
				}
				
			}
		}
	}
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	Renderer renderer(screen_width, screen_height);
	Uint32 starting_tick;
	SDL_Event event;

	SDL_Color white = { 255, 255, 255 };
	//initialize
	bool running = true, dead = false, dead2 = false, twoPlayers = true;
	double fps = 10.0;
	srand(time(NULL));
	int direction = 3, direction2 = 3;
	list<Wonsz> wonsz = { { ((screen_width / 2) / block_size) * block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + 2 * block_size, ((screen_height / 2) / block_size) * block_size },
						  { ((screen_width / 2) / block_size) * block_size + 3 * block_size, ((screen_height / 2) / block_size) * block_size } };
	list<Wonsz> wonsz2 = { { ((screen_width / 2) / block_size) * block_size, ((screen_height / 4) / block_size) * block_size },
						   { ((screen_width / 2) / block_size) * block_size + block_size, ((screen_height / 4) / block_size) * block_size },
						   { ((screen_width / 2) / block_size) * block_size + 2 * block_size, ((screen_height / 4) / block_size) * block_size },
						   { ((screen_width / 2) / block_size) * block_size + 3 * block_size, ((screen_height / 4) / block_size) * block_size } };
	list<Food> food;
	if (!twoPlayers) spawnFood(wonsz, food);
	else spawnFoods(wonsz, wonsz2, food);
	//initialize

	while (running) {
		starting_tick = SDL_GetTicks();
		controls(wonsz, wonsz2, food, direction, direction2, dead, dead2, running, twoPlayers, fps, event);

		//ai(wonsz, foodX, foodY, direction);
		
		move(dead, direction, wonsz);
		collisionWithFood(wonsz, fps, food);
		collisionWithMap(wonsz, dead);
		collisionWithSnake(wonsz, dead);
		drawSnake(wonsz, dead, renderer);
		drawFood(renderer, food);

		if (twoPlayers) {
			move(dead2, direction2, wonsz2);
			collisionWithFood(wonsz2, fps, food);
			collisionWithMap(wonsz2, dead2);
			collisionWithSnake(wonsz2, dead2);
			drawSnake(wonsz2, dead2, renderer);
			collisionBeetweenSnakes(wonsz, wonsz2, dead, dead2);
		}

		if (dead) {
			for (auto &i : wonsz) {
				food.push_back( { i.x, i.y, false } );
			}
			wonsz.clear();
		}
		if (dead2) {
			for (auto &i : wonsz2) {
				food.push_back({ i.x, i.y, false });
			}
			wonsz2.clear();
		}

		renderer.update_screen();
		if ((1000 / fps) > SDL_GetTicks() - starting_tick) {
			SDL_Delay(1000 / fps - (SDL_GetTicks() - starting_tick));
		}
	}
	return 0;
}