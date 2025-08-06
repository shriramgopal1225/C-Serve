#ifndef CART_LOGIC_H
#define CART_LOGIC_H

#include "structs.h"

// Function declarations
void initialize_cart(Cart* cart);
void add_item_to_cart(Cart* cart, MenuItem2 item, int quantity);
void display_cart(Cart* cart);
void payment(Cart* cart, User* loggedInUser);
int menufromfile(const char* search_restaurant, MenuItem2 menu[], int* menu_size);
void display_menu(MenuItem2 menu[], int menu_size);

#endif