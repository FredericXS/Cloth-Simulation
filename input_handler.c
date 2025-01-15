#include "input_handler.h"

float point_to_segment_distance(float px, float py, float x1, float y1, float x2, float y2) {
    float ABx = x2 - x1;
    float ABy = y2 - y1;
    float APx = px - x1;
    float APy = py - y1;
    float BPx = px - x2;
    float BPy = py - y2;
    float AB_AP = ABx * APx + ABy * APy;
    float AB_AB = ABx * ABx + ABy * ABy;
    float t = AB_AP / AB_AB;

    if (t < 0.0f) {
        return sqrtf(APx * APx + APy * APy);
    } else if (t > 1.0f) {
        return sqrtf(BPx * BPx + BPy * BPy);
    } else {
        float proj_x = x1 + t * ABx;
        float proj_y = y1 + t * ABy;
        return sqrtf((px - proj_x) * (px - proj_x) + (py - proj_y) * (py - proj_y));
    }
}

Constraint* find_nearest_constraint(float mouse_x, float mouse_y, 
                                    Constraint* constraints, int constraints_count) {

    Constraint* nearest_constraint = NULL;
    float min_distance = CLICK_TOLERANCE;
    
    for (int i = 0; i < constraints_count; i++) {
        float distance = point_to_segment_distance(mouse_x, mouse_y,
                            constraints[i].p1->position.x, constraints[i].p1->position.y,
                            constraints[i].p2->position.x, constraints[i].p2->position.y);

        if (distance < min_distance) {
            min_distance = distance;
            nearest_constraint = &constraints[i];
        }
    }
    return nearest_constraint;
}

void tear_cloth(float mouse_x, float mouse_y, 
               Constraint* constraints, int constraints_count) {

    Constraint* nearest = find_nearest_constraint(mouse_x, mouse_y, constraints, constraints_count);
    if (nearest) {
        nearest->active = 0;
    }
}

void handle_mouse_click(const SDL_Event* event, 
                       Constraint* constraints, int constraints_count) {

    if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
        float mouse_x = (float)event->button.x;
        float mouse_y = (float)event->button.y;
        tear_cloth(mouse_x, mouse_y, constraints, constraints_count);
    }
}