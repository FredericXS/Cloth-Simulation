#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#include "particle.h"
#include "constraint.h"
#include "input_handler.h"

#define WIDTH 1080
#define HEIGHT 720
#define GRAVITY 9.80665f // Precisely asf
#define TIME_STEP 0.1f

#define ROW 10
#define COL 10
#define REST_DISTANCE 30.0f

void exit_all(Particle *particles, Constraint *constraints, SDL_Renderer *renderer, SDL_Window *window) {
    free(particles);
    free(constraints);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

int main() {
    // SDL Window stuff
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Could not initialize SDL! %s\n", SDL_GetError());
        return -1;
    }
    SDL_Window *window  = SDL_CreateWindow(
        "Cloth Simulation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("Could not create window! %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Could not create renderer! %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_Event event;

    // Create particles and constraints
    int total_particles = ROW * COL;
    Particle *particles = malloc(ROW * COL * sizeof(Particle));

    int total_constraints = (ROW * (COL - 1)) + ((ROW - 1) * COL);
    Constraint *constraints = malloc(total_constraints * sizeof(Constraint));

    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            float x = col * REST_DISTANCE + WIDTH / 3;
            float y = row * REST_DISTANCE + HEIGHT / 3;
            bool pinned = (row == 0);
            create_particle(&particles[row * COL + col], x, y, pinned);
        }
    }

    int constraint_index = 0;
    for (int row = 0; row < ROW; row++) {
        for (int col = 0; col < COL; col++) {
            if (col < COL - 1) {
                create_constraint(&constraints[constraint_index++],
                                &particles[row * COL + col],
                                &particles[row * COL + col + 1]);
            }
            if (row < ROW - 1) {
                create_constraint(&constraints[constraint_index++],
                                &particles[row * COL + col],
                                &particles[(row + 1) * COL + col]);
            }
        }
    }

    while(1) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            exit_all(particles, constraints, renderer, window);
                    }
                case SDL_QUIT:
                    exit_all(particles, constraints, renderer, window);
            }
            handle_mouse_click(&event, constraints, total_constraints);
        }

        // Applying gravity, checking collisions and drawing particles as points
        for (int i = 0; i < total_particles; i++) {
            Particle *particle = &particles[i];
            apply_force(particle, (Coords){0, GRAVITY});
            update(particle, TIME_STEP);
            constrain_to_bounds(particle, WIDTH, HEIGHT);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawPoint(renderer, particles[i].position.x, particles[i].position.y);
        }

        // Rendering constraints
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < total_constraints; i++) {
                Constraint constraint = constraints[i];
                if (!constraint.active) continue;
                satisfy(&constraints[i]);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawLine(renderer, constraint.p1->position.x, constraint.p1->position.y,
                                constraint.p2->position.x, constraint.p2->position.y);
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // 60 FPS
    }

    exit_all(particles, constraints, renderer, window);

    return 0;
}