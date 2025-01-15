#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "particle.h"
#include <math.h>

typedef struct {
    Particle *p1;
    Particle *p2;
    float initial_length;
    bool active;
} Constraint;

Constraint create_constraint(Constraint *constraint, Particle *p1, Particle *p2);
void satisfy(Constraint *constraint);
void deactivate(Constraint *constraint);

#endif // CONSTRAINT_H