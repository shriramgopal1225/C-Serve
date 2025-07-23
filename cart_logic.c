#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "cart_logic.h" // Assuming this includes definitions for Cart, MenuItem2, MAX_ITEMS, BUFFER_SIZE, User

// Custom implementation of strcasestr for platforms where it's not available
char *my_strcasestr(const char *haystack, const char *needle) {
    if (!*needle) { // Empty needle, return haystack
        return (char *)haystack;
    }

    for (; *haystack; ++haystack) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            const char *h, *n;
            for (h = haystack, n = needle; *h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n); ++h, ++n);
            if (!*n) { // Found entire needle
                return (char *)haystack;
            }
        }
    }
    return NULL; // Not found
}

void initialize_cart(Cart *cart) {
    cart->num_items = 0;
    cart->total = 0.0;
}

void add_item_to_cart(Cart *cart, MenuItem2 item, int quantity) {
    if (cart->num_items >= MAX_ITEMS) {
        printf("Cart is full.\n");
        return;
    }
    cart->items[cart->num_items] = item;
    cart->quantity[cart->num_items] = quantity;
    cart->num_items++;
    cart->total += item.price * quantity;
    printf("Item added successfully.\n\n");
}

void remove_item_from_cart(Cart *cart, int item_id) {
    int i, j, found = 0;
    for (i = 0; i < cart->num_items; i++) {
        if (cart->items[i].item_id == item_id) {
            cart->total -= cart->items[i].price * cart->quantity[i];
            
            // Shift remaining items to the left
            for (j = i; j < cart->num_items - 1; j++) {
                cart->items[j] = cart->items[j + 1];
                cart->quantity[j] = cart->quantity[j + 1];
            }
            cart->num_items--;
            found = 1;
            printf("Item removed successfully.\n\n");
            break;
        }
    }
    if (!found) {
        printf("Item with ID %d not found in cart.\n", item_id);
    }
}

void modify_quantity(Cart *cart, int item_id, int new_quantity) {
    int found = 0;
    for (int i = 0; i < cart->num_items; i++) {
        if (cart->items[i].item_id == item_id) {
            // In a real app, you'd check stock here.
            // For now, we just update.
            cart->total -= cart->items[i].price * cart->quantity[i]; // remove old total
            cart->total += cart->items[i].price * new_quantity; // add new total
            cart->quantity[i] = new_quantity;
            found = 1;
            printf("Quantity has been modified.\n\n");
            break;
        }
    }
    if (!found) {
        printf("Item with ID %d not found in cart.\n", item_id);
    }
}

void display_menu(MenuItem2 menu[], int menu_size) {
    printf("\n--- Menu --- \n");
    printf("+--------+-----------------------------------------+-----------+\n");
    printf("| %-6s | %-39s | %-9s |\n", "ID", "Item", "Price");
    printf("+--------+-----------------------------------------+-----------+\n");

    if (menu_size == 0) {
        printf("| %-55s |\n", "No items found for this restaurant.");
    } else {
        for (int i = 0; i < menu_size; i++) {
            printf("| %-6d | %-39s | %-9.2f |\n", 
                    menu[i].item_id, menu[i].item_name, menu[i].price);
        }
    }
    printf("+--------+-----------------------------------------+-----------+\n");
}

void display_cart(Cart *cart) {
    printf("\n--- YOUR CART ---\n");
    printf("+--------+-----------------------------------------+-----------+----------+----------+\n");
    printf("| %-6s | %-39s | %-9s | %-8s | %-8s |\n", "ID", "Item", "Price", "Quantity", "Total");
    printf("+--------+-----------------------------------------+-----------+----------+----------+\n");
    if (cart->num_items == 0) {
        printf("| %-82s |\n", "Cart is empty.");
    } else {
        for (int i = 0; i < cart->num_items; i++) {
            printf("| %-6d | %-39s | %-9.2f | %-8d | %-8.2f |\n", 
                    cart->items[i].item_id, cart->items[i].item_name, cart->items[i].price, 
                    cart->quantity[i], cart->items[i].price * cart->quantity[i]);
        }
    }
    printf("+--------+-----------------------------------------+-----------+----------+----------+\n");
    printf("Total Value: %.2f\n", cart->total);
    printf("-----------------\n");
}

void loyalty_points(Cart *cart, User* loggedInUser) {
    int points_earned = cart->total / 10; // 1 point per 10 currency units
    loggedInUser->loyalty_points += points_earned;
    printf("You have earned %d loyalty points! Your new total is: %d\n", points_earned, loggedInUser->loyalty_points);
}

void payment(Cart *cart, User* loggedInUser) {
    if (cart->num_items == 0) {
        printf("Cannot proceed to payment, your cart is empty.\n");
        return;
    }

    double total_payable = cart->total * 1.18; // Assuming a flat 18% tax

    printf("\n--- PAYMENT ---\n");
    printf("Cart Total:           %.2f\n", cart->total);
    printf("Tax (18%%):            +%.2f\n", cart->total * 0.18);
    printf("---------------------------\n");
    printf("Total Payable:        %.2f\n", total_payable);
    printf("\nYour E-Wallet Balance:  %.2f\n", loggedInUser->ewallet_balance);

    if (loggedInUser->ewallet_balance >= total_payable) {
        loggedInUser->ewallet_balance -= total_payable;
        printf("Payment Successful! New E-Wallet Balance: %.2f\n", loggedInUser->ewallet_balance);
        loyalty_points(cart, loggedInUser);
    } else {
        printf("Payment Failed: Insufficient E-Wallet balance.\n");
    }
}

int menufromfile(char *search_restaurant, MenuItem2 menu[], int *menu_size) {
    FILE *file = fopen("data/menuitems.csv", "r");
    if (file == NULL) {
        printf("Error: Could not open menu file 'data/menuitems.csv'.\n");
        return 0;
    }
    
    int count = 0;
    char buffer[BUFFER_SIZE];
    fgets(buffer, sizeof(buffer), file); // Skip header

    while (fgets(buffer, sizeof(buffer), file) && count < MAX_ITEMS) {
        // *** CHANGE THIS LINE ***
        if (my_strcasestr(buffer, search_restaurant)) { // Case-insensitive search using your custom function
            sscanf(buffer, "%99[^,],%d,%99[^,],%f,%7[^,],%d,%f",
                menu[count].restaurant, &menu[count].item_id,
                menu[count].item_name, &menu[count].price,
                menu[count].veg_non_veg, &menu[count].quantity,
                &menu[count].rating);
            count++;
        }
    }
    fclose(file);
    *menu_size = count;
    
    return count; // Return the number of items found
}
