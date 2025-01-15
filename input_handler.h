#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <math.h>
#include "particle.h"
#include "constraint.h"

#define CLICK_TOLERANCE 5.0f

float point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2);
Constraint* find_nearest_constraint(float mouse_x, float mouse_y, Constraint* constraints, int constraints_count);
void tear_cloth(float mouse_x, float mouse_y, Constraint* constraints, int constraints_count);
void handle_mouse_click(const SDL_Event* event, Constraint* constraints, int constraints_count);

#endif // INPUT_HANDLER_H