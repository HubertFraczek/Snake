#ifndef RENDERER_H
#define RENDERER_H
#include <iostream>
#include <SDL.h>
#include <math.h>
#include <string.h>
using namespace std;
class Renderer {
	SDL_Window *window;
	SDL_Renderer *renderer;
public:
	Renderer(int window_w, int window_h);
	void update_screen();
	~Renderer();
	SDL_Renderer *get_renderer();
};
#endif //RENDERER_H