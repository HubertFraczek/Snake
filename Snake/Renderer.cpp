#include "Renderer.h"
using namespace std;
Renderer::Renderer(int window_w, int window_h) {
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_w, window_h, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
void Renderer::update_screen() {
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255);
	SDL_RenderClear(renderer);
}
Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}
SDL_Renderer *Renderer::get_renderer() {
	return renderer;
}