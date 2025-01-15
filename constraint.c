#include "constraint.h"

Constraint create_constraint(Constraint *constraint, Particle *p1, Particle *p2) {
    constraint->p1 = p1;
    constraint->p2 = p2;
    constraint->initial_length = sqrtf(
        powf(p2->position.x - p1->position.x, 2) +
        powf(p2->position.y - p1->position.y, 2)
    );
    constraint->active = true;

    return *constraint;
}

void satisfy(Constraint *constraint) {
    if (!constraint->active) return;

    Coords delta = {constraint->p2->position.x - constraint->p1->position.x,
                    constraint->p2->position.y - constraint->p1->position.y};
    
    float current_length = sqrtf(delta.x * delta.x + delta.y * delta.y);
    float difference = (current_length - constraint->initial_length) / current_length;
    Coords correction = {delta.x * 0.5f * difference, delta.y * 0.5f * difference};

    if (!constraint->p1->is_pinned) {
        constraint->p1->position.x += correction.x;
        constraint->p1->position.y += correction.y;
    }
    if (!constraint->p2->is_pinned) {
        constraint->p2->position.x -= correction.x;
        constraint->p2->position.y -= correction.y;
    }
}

void deactivate(Constraint *constraint) {
    constraint->active = false;
}