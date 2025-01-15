#include "particle.h"

Particle create_particle(Particle *particle, float x, float y, bool pinned) {
    particle->position = (Coords){x, y};
    particle->prev_position = (Coords){x, y};
    particle->acceleration = (Coords){0, 0};
    particle->is_pinned = pinned;

    return *particle;
}

void apply_force(Particle *particle, Coords force) {
    if (!particle->is_pinned) {
        particle->acceleration.x += force.x;
        particle->acceleration.y += force.y;
    }
}

void update(Particle *particle, float time_step) {
    if (!particle->is_pinned) {
    Coords velocity = {
        particle->position.x - particle->prev_position.x,
        particle->position.y - particle->prev_position.y
    };

    particle->prev_position = particle->position;
    particle->position.x += velocity.x + particle->acceleration.x * time_step * time_step;
    particle->position.y += velocity.y + particle->acceleration.y * time_step * time_step;
    particle->acceleration = (Coords){0, 0};
    }
}

void constrain_to_bounds(Particle *particle, float width, float height) {
    if (particle->position.x < 0) particle->position.x = 0;
    if (particle->position.x > width) particle->position.x = width;
    if (particle->position.y < 0) particle->position.y = 0;
    if (particle->position.y > height) particle->position.y = height;
}