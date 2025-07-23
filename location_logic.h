#ifndef LOCATION_LOGIC_H
#define LOCATION_LOGIC_H

#include "structs.h"

// --- Public Function Declarations ---

// Functions for validating coordinates
int road(int x, int y);
int restaurant(int x, int y);

// The main function for finding and sorting nearby hotels
void dijkstra(int location, int src, struct Hotel hotels[], int n);

#endif