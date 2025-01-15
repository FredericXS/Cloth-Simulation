#ifndef PARTICLE_H
#define PARTICLE_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
} Coords;

typedef struct {
    Coords position;
    Coords prev_position;
    Coords acceleration;
    bool is_pinned;
} Particle;

Particle create_particle(Particle *particle, float x, float y, bool pinned);
void apply_force(Particle *particle, Coords force);
void update(Particle *particle, float time_step);
void constrain_to_bounds(Particle *particle, float width, float height);

#endif // PARTICLE_H