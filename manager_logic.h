#ifndef MANAGER_LOGIC_H
#define MANAGER_LOGIC_H

#include "structs.h"

#define MAX_MANAGERS 50

// This is the main entry point for the manager side of the application
void handleManagerSession();
void manager_menu(Manager* loggedInManager);
int menutofile(const char* restaurantName, MenuItem2 menu[], int menu_size);

#endif