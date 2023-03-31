#include <SDL2/SDL.h>
#include "matrix.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define CELL_SIZE 10
#define NUM_CELL_ROW SCREEN_HEIGHT/CELL_SIZE
#define NUM_CELL_COLS SCREEN_WIDTH/CELL_SIZE
#define UPDATE_RATE 800

int main(int argc, char* argv[]) {
    Mat* matrix = get_random_matrix(NUM_CELL_ROW, NUM_CELL_COLS);

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    int quit = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Griglia in bianco e nero", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    Uint32 last_update_time = 0;

    for (int i = 0; i < NUM_CELL_ROW; ++i) {
        for (int j = 0; j < NUM_CELL_COLS; ++j) {
            if (matrix -> matrix[i][j] == 1) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &rect);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &rect);
            }
            rect.x = CELL_SIZE * i;
            rect.y = CELL_SIZE * j;
        }
    }
    SDL_RenderPresent(renderer);

    while (1) {
        matrix = game_of_life(matrix);
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed_time = current_time - last_update_time;

        if (elapsed_time > UPDATE_RATE) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < NUM_CELL_ROW; i++) {
                for (int j = 0; j < NUM_CELL_COLS; j++) {
                    if (matrix ->matrix[i][j] == 1) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    } else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    }
                    rect.x = i * CELL_SIZE;
                    rect.y = j * CELL_SIZE;
                    SDL_RenderFillRect(renderer, &rect);
                }
            }

            SDL_RenderPresent(renderer);

            last_update_time = current_time;
        }

        SDL_Delay(10);

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }
    }
}
