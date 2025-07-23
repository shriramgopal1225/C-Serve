#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "app_logic.h"
#include "cart_logic.h"
#include "location_logic.h"

// Note: All #defines and struct definitions have been removed from this file.
// They now live in structs.h.


// --- All functions from the old app.c, now modified for our new structure ---

void restaurant_locations(char grid[36][36])
{
    grid[2][3] = 'H';
    grid[33][28] = 'H';
    // ... (rest of the function is unchanged)
}

void not_restaurant_locations(char grid[36][36])
{
    grid[2][3] = '|';
    grid[33][28] = '|';
    // ... (rest of the function is unchanged)
}

void update_review(const char *restaurant_name, double new_rating)
{
    // Note: FILENAME3 is not defined. We should change this to a relative path.
    FILE *file = fopen("data/restaurants_ratings.csv", "r+");
    if (file == NULL)
    {
        // This is not an error, the file might not exist yet.
        // Let's open in "a+" mode to create it if it doesn't exist.
        file = fopen("data/restaurants_ratings.csv", "a+");
        if (file == NULL) {
            perror("Failed to open or create ratings file");
            return;
        }
    }
    // ... (rest of the function is largely unchanged but would need review for robustness)
    char buffer[BUFFER_SIZE];
    long pos = 0; 
    int found = 0;

    while (fgets(buffer, BUFFER_SIZE, file))
    {
        char name[100];
        double avg_rating;
        int num_reviews;
        long line_start = pos;
        sscanf(buffer, "%99[^,],%lf,%d", name, &avg_rating, &num_reviews);
        pos = ftell(file);

        if (strcasecmp(name, restaurant_name) == 0)
        {
            num_reviews++;
            avg_rating = ((avg_rating * (num_reviews - 1)) + new_rating) / num_reviews;
            found = 1;
            fseek(file, line_start, SEEK_SET);
            fprintf(file, "%s,%.1f,%d\n", name, avg_rating, num_reviews);
            long new_pos = ftell(file);
            int padding = pos - new_pos;
            for(int i = 0; i < padding; i++) fputc(' ', file);
            break;
        }
    }

    if (!found) {
        fprintf(file, "%s,%.1f,1\n", restaurant_name, new_rating);
    }
    fclose(file);
}


void readCSV(Driver drivers[])
{
    FILE *fp = fopen("data/drivers.csv", "r");
    if (!fp) {
        printf("Failed to open drivers file.\n");
        return;
    }
    char line[256];
    int count = 0;
    fgets(line, sizeof(line), fp); // Skip header line
    while (fgets(line, sizeof(line), fp) && count < MAX_DRIVERS) {
        // Corrected sscanf
        sscanf(line, "%49[^,],%14[^,],%d,%d,%9[^,],%d", 
               drivers[count].name, drivers[count].phone_number,
               &drivers[count].x, &drivers[count].y, drivers[count].status, &drivers[count].rating);
        count++;
    }
    fclose(fp);
}

void writeCSV(Driver drivers[])
{
    FILE *fp = fopen("data/drivers.csv", "w");
    if (!fp) {
        printf("Failed to open the file for writing.\n");
        return;
    }
    fprintf(fp, "Name,Phone,CoordinateX,CoordinateY,Status,Rating\n");
    for (int i = 0; i < MAX_DRIVERS; i++) {
        // Corrected fprintf
        fprintf(fp, "%s,%s,%d,%d,%s,%d\n", drivers[i].name, drivers[i].phone_number, drivers[i].x, drivers[i].y, drivers[i].status, drivers[i].rating);
    }
    fclose(fp);
}

double calculateDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}


void loadMenuItems(const char *filename, MenuItem2 menu[], int *num_menu_items)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Unable to open file %s\n", filename);
        return;
    }
    char line[BUFFER_SIZE];
    *num_menu_items = 0; // Reset count
    fgets(line, sizeof(line), file);  // Skip header

    while (fgets(line, sizeof(line), file) && *num_menu_items < MAX_ITEMS) {
        sscanf(line, "%99[^,],%d,%99[^,],%f,%7[^,],%d,%f",
               menu[*num_menu_items].restaurant,
               &menu[*num_menu_items].item_id,
               menu[*num_menu_items].item_name,
               &menu[*num_menu_items].price,
               menu[*num_menu_items].veg_non_veg,
               &menu[*num_menu_items].quantity,
               &menu[*num_menu_items].rating);
        (*num_menu_items)++;
    }
    fclose(file);
}


// Main function for the application logic after user logs in
void runUserSession(User* loggedInUser)
{
    printf("\nHello, %s! Welcome to the C-Serve App.\n", loggedInUser->name);
    printf("Your E-Wallet Balance: %.2f\n", loggedInUser->ewallet_balance);

    char locations[MAX_LOCATIONS][50] = {
        "Medavakkam",
        "Besant Nagar",
        "T Nagar",
        "Anna Nagar",
        "Thiruporur"
        // Add other locations here...
    };

    struct Hotel hotels[MAX_LOCATIONS][MAX_HOTELS] = {
        // Hotels nearby Medavakkam
        {
            {"Flower Drum", 8.5},
            {"Thalapakatti", 9.2},
            {"KFC", 7.8},
            {"McDonalds", 7.5},
            {"Dominos", 8.0}
        },
        // Hotels nearby Besant Nagar
        {
            {"Bilal", 8.0},
            {"Murugan Idli Shop", 9.0},
            {"Farzi Cafe", 8.5},
            {"Pizza Hut", 7.7},
            {"A2B", 8.8}
        },
        // Hotels nearby T Nagar
        {
            {"Barbeque Nation", 8.9},
            {"Steam & Whistles", 9.5},
            {"Bazaar", 7.6},
            {"J Hind", 8.2},
            {"Absolute Barbeque", 9.0}
        },
        // Hotels nearby Anna Nagar
        {
            {"Taco Bell", 7.9},
            {"Pind", 8.7},
            {"Burger King", 7.5},
            {"Krispy Kreme", 8.3},
            {"Nescafe", 8.0}
        },
        // Hotels nearby Thiruporur
        {
            {"Asia7", 8.6},
            {"Pakwan", 8.2},
            {"Udupi Hotel", 7.8},
            {"Kaithi", 8.4},
            {"Popeyes", 8.1}
        },

        // Add hotels for other locations similarly...
    };

    int n = MAX_HOTELS;
    int location_choice;

    // Prompt the user to select a location
    printf("\nPlease select your delivery area:\n");
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        printf("%d. %s\n", i + 1, locations[i]);
    }
    printf("Enter your choice: ");
    scanf("%d", &location_choice);
    // Add a buffer clear after scanf
    int c; while ((c = getchar()) != '\n' && c != EOF); 

    // Validate choice and call Dijkstra
    if (location_choice > 0 && location_choice <= MAX_LOCATIONS) {
        // We subtract 1 because arrays are 0-indexed
        dijkstra(location_choice - 1, 0, hotels[location_choice - 1], n);
    } else {
        printf("Invalid location choice.\n");
        return; // Exit the session if choice is invalid
    }
    
    char search_restaurant[MAX_NAME_LENGTH];
    MenuItem2 menu[MAX_ITEMS];
    int menu_size = 0;
    Cart cart;
    initialize_cart(&cart);

    printf("\nEnter the name of the restaurant you want to order from: ");
    fgets(search_restaurant, sizeof(search_restaurant), stdin);
    search_restaurant[strcspn(search_restaurant, "\n")] = '\0';
    
    menufromfile(search_restaurant, menu, &menu_size);

    display_menu(menu, menu_size);

    if (menu_size == 0) {
        printf("Exiting session.\n");
        return;
    }

    int choice = 0;
    while(choice != 3) {
        printf("\n1. Add item to cart\n");
        printf("2. View cart\n");
        printf("3. Proceed to payment\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        int c; while ((c = getchar()) != '\n' && c != EOF); // Clear buffer

        if (choice == 1) {
            int item_id, quantity;
            printf("Enter item ID to add: ");
            scanf("%d", &item_id);
            printf("Enter quantity: ");
            scanf("%d", &quantity);
            while ((c = getchar()) != '\n' && c != EOF); // Clear buffer

            int found = 0;
            for(int i = 0; i < menu_size; i++) {
                if (menu[i].item_id == item_id) {
                    add_item_to_cart(&cart, menu[i], quantity);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Invalid item ID.\n");
            }
        } else if (choice == 2) {
            display_cart(&cart);
        } else if (choice == 3) {
            if (cart.num_items > 0) {
                payment(&cart, loggedInUser);
            } else {
                printf("Your cart is empty.\n");
            }
        }
    }
    
    printf("\nThank you for using C-Serve!\n");
    return;
}