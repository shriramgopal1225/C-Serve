#include <stdio.h>
#include <string.h>

#include "manager_logic.h"
#include "structs.h"    // We need this for the Manager struct
#include "cart_logic.h" // For MenuItem2 and other related functions

int load_managers_from_file(Manager managers[])
{
    FILE *file = fopen("data/managers.csv", "r");
    if (file == NULL)
    {
        printf("Error: Could not open managers.csv\n");
        return 0;
    }

    char buffer[BUFFER_SIZE];
    int count = 0;
    fgets(buffer, sizeof(buffer), file); // Skip header line

    while (fgets(buffer, sizeof(buffer), file) && count < MAX_MANAGERS)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        sscanf(buffer, "%49[^,],%49[^,],%49[^,],%49[^,],%19[^\n]",
               managers[count].restaurantName,
               managers[count].managerName,
               managers[count].loginId,
               managers[count].password,
               managers[count].number);
        count++;
    }
    fclose(file);
    return count;
}

int verify_manager_login(Manager managers[], int count, const char *loginId, const char *password)
{
    for (int i = 0; i < count; i++)
    {
        if (strcmp(managers[i].loginId, loginId) == 0 && strcmp(managers[i].password, password) == 0)
        {
            return i; // Return the index of the logged-in manager on success
        }
    }
    return -1; // Return -1 on failure
}

void view_my_menu(const char *restaurantName)
{
    MenuItem2 all_items[MAX_ITEMS];
    int item_count = 0;

    // We can reuse the function from cart_logic.c to load all items
    // Note: For this to work, menufromfile needs to be declared in cart_logic.h
    // and cart_logic.h needs to be included in manager_logic.c
    // For now, let's write a simplified loader here.
    menufromfile(restaurantName, all_items, &item_count);

    display_menu(all_items, item_count);
}

int menutofile(const char *restaurantName, MenuItem2 menu[], int menu_size)
{
    FILE *fin = fopen("data/menuitems.csv", "r");
    FILE *fout = fopen("data/menuitems_temp.csv", "w");
    if (!fin || !fout)
    {
        if (fin)
            fclose(fin);
        if (fout)
            fclose(fout);
        printf("Error opening menuitems.csv for update.\n");
        return 0;
    }

    char buffer[512];

    // Copy header
    if (fgets(buffer, sizeof(buffer), fin))
    {
        fputs(buffer, fout);
    }

    // Copy all items except those from this restaurant
    while (fgets(buffer, sizeof(buffer), fin))
    {
        char rest[MAX_NAME_LENGTH];
        sscanf(buffer, "%[^,]", rest);
        if (strcmp(rest, restaurantName) != 0)
        {
            fputs(buffer, fout);
        }
    }

    // Write all new/updated items for this restaurant
    for (int i = 0; i < menu_size; i++)
    {
        int duplicate = 0;
        for (int j = 0; j < i; j++)
        {
            if (menu[i].item_id == menu[j].item_id)
            {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate)
        {
            fprintf(fout, "%s,%d,%s,%.2f,%s,%d,%.1f\n",
                    menu[i].restaurant,
                    menu[i].item_id,
                    menu[i].item_name,
                    menu[i].price,
                    menu[i].veg_non_veg,
                    menu[i].quantity,
                    menu[i].rating);
        }
    }

    fclose(fin);
    fclose(fout);

    // Replace old file with new
    remove("data/menuitems.csv");
    rename("data/menuitems_temp.csv", "data/menuitems.csv");
    return 1;
}

void add_menu_item(const char *restaurantName)
{
    MenuItem2 all_items[MAX_ITEMS];
    int item_count = 0;

    menufromfile(restaurantName, all_items, &item_count);

    if (item_count >= MAX_ITEMS)
    {
        printf("Menu is full. Cannot add more items.\n");
        return;
    }

    MenuItem2 newItem;
    strncpy(newItem.restaurant, restaurantName, MAX_NAME_LENGTH);
    newItem.restaurant[MAX_NAME_LENGTH - 1] = '\0';

    // Assign a new item_id (find max existing and add 1)
    int max_id = 0;
    for (int i = 0; i < item_count; i++)
    {
        if (all_items[i].item_id > max_id)
        {
            max_id = all_items[i].item_id;
        }
    }
    newItem.item_id = max_id + 1;

    printf("Enter item name: ");
    fgets(newItem.item_name, sizeof(newItem.item_name), stdin);
    newItem.item_name[strcspn(newItem.item_name, "\n")] = '\0';

    printf("Enter item price: ");
    scanf("%f", &newItem.price);
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Enter Veg/Non-Veg: ");
    fgets(newItem.veg_non_veg, sizeof(newItem.veg_non_veg), stdin);
    newItem.veg_non_veg[strcspn(newItem.veg_non_veg, "\n")] = '\0';

    printf("Enter quantity: ");
    scanf("%d", &newItem.quantity);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Enter rating (e.g., 4.5): ");
    scanf("%f", &newItem.rating);
    c = 0;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // Add to array
    all_items[item_count++] = newItem;

    // Save back to file
    if (menutofile(restaurantName, all_items, item_count))
    {
        printf("Menu item added successfully!\n");
    }
    else
    {
        printf("Failed to save menu item.\n");
    }
}

void modify_menu_item(const char *restaurantName) {
    MenuItem2 all_items[MAX_ITEMS];
    int item_count = 0;
    menufromfile(restaurantName, all_items, &item_count);

    if (item_count == 0) {
        printf("No menu items found for this restaurant.\n");
        return;
    }

    printf("\n--- Current Menu ---\n");
    for (int i = 0; i < item_count; i++) {
        printf("%d. [%d] %s - %.2f %s Qty:%d Rating:%.1f\n", 
            i+1, all_items[i].item_id, all_items[i].item_name, all_items[i].price, 
            all_items[i].veg_non_veg, all_items[i].quantity, all_items[i].rating);
    }

    int id;
    printf("Enter the Item ID of the menu item you want to modify: ");
    scanf("%d", &id);
    int c; while ((c = getchar()) != '\n' && c != EOF);

    int found = 0;
    for (int i = 0; i < item_count; i++) {
        if (all_items[i].item_id == id) {
            found = 1;
            printf("Editing item: %s\n", all_items[i].item_name);

            printf("Enter new name (or press Enter to keep '%s'): ", all_items[i].item_name);
            char buf[MAX_NAME_LENGTH];
            fgets(buf, sizeof(buf), stdin);
            if (buf[0] != '\n') {
                buf[strcspn(buf, "\n")] = '\0';
                strncpy(all_items[i].item_name, buf, MAX_NAME_LENGTH);
            }

            printf("Enter new price (or 0 to keep %.2f): ", all_items[i].price);
            float price;
            if (scanf("%f", &price) == 1 && price > 0) {
                all_items[i].price = price;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter new Veg/Non-Veg (or press Enter to keep '%s'): ", all_items[i].veg_non_veg);
            fgets(buf, sizeof(buf), stdin);
            if (buf[0] != '\n') {
                buf[strcspn(buf, "\n")] = '\0';
                strncpy(all_items[i].veg_non_veg, buf, sizeof(all_items[i].veg_non_veg));
            }

            printf("Enter new quantity (or -1 to keep %d): ", all_items[i].quantity);
            int qty;
            if (scanf("%d", &qty) == 1 && qty >= 0) {
                all_items[i].quantity = qty;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            printf("Enter new rating (or 0 to keep %.1f): ", all_items[i].rating);
            float rating;
            if (scanf("%f", &rating) == 1 && rating > 0) {
                all_items[i].rating = rating;
            }
            while ((c = getchar()) != '\n' && c != EOF);

            break;
        }
    }

    if (!found) {
        printf("Item ID not found.\n");
        return;
    }

    if (menutofile(restaurantName, all_items, item_count)) {
        printf("Menu item updated successfully!\n");
    } else {
        printf("Failed to update menu item.\n");
    }
}

void remove_menu_item(const char *restaurantName) {
    MenuItem2 all_items[MAX_ITEMS];
    int item_count = 0;
    menufromfile(restaurantName, all_items, &item_count);

    if (item_count == 0) {
        printf("No menu items found for this restaurant.\n");
        return;
    }

    printf("\n--- Current Menu ---\n");
    for (int i = 0; i < item_count; i++) {
        printf("%d. [%d] %s - %.2f %s Qty:%d Rating:%.1f\n", 
            i+1, all_items[i].item_id, all_items[i].item_name, all_items[i].price, 
            all_items[i].veg_non_veg, all_items[i].quantity, all_items[i].rating);
    }

    int id;
    printf("Enter the Item ID of the menu item you want to remove: ");
    scanf("%d", &id);
    int c; while ((c = getchar()) != '\n' && c != EOF);

    int found = 0;
    for (int i = 0; i < item_count; i++) {
        if (all_items[i].item_id == id) {
            found = 1;
            // Shift all items after i one position left
            for (int j = i; j < item_count - 1; j++) {
                all_items[j] = all_items[j + 1];
            }
            item_count--;
            break;
        }
    }

    if (!found) {
        printf("Item ID not found.\n");
        return;
    }

    if (menutofile(restaurantName, all_items, item_count)) {
        printf("Menu item removed successfully!\n");
    } else {
        printf("Failed to remove menu item.\n");
    }
}

void manager_menu(Manager *loggedInManager)
{
    int choice = 0;
    while (choice != 5)
    {
        printf("\n--- %s Menu Management ---\n", loggedInManager->restaurantName);
        printf("1. View My Menu\n");
        printf("2. Add New Menu Item\n");
        printf("3. Modify Existing Item\n");
        printf("4. Remove Menu Item\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        if (choice == 1)
        {
            view_my_menu(loggedInManager->restaurantName);
            // We will create this function next
        }
        else if (choice == 2)
        {
            // Add new menu item logic here
            add_menu_item(loggedInManager->restaurantName);
        }
        else if (choice == 3)
        {
            // Modify existing item logic here
            modify_menu_item(loggedInManager->restaurantName);
        }
        else if (choice == 4)
        {
            // Remove menu item logic here
            remove_menu_item(loggedInManager->restaurantName);
        }
        else if (choice == 5)
        {
            printf("Logging out...\n");
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }

        // We will add the function calls here in the next steps
    }
}

void handleManagerSession()
{
    Manager managers[MAX_MANAGERS];
    int manager_count = load_managers_from_file(managers);

    if (manager_count == 0)
    {
        printf("Could not load manager data. The portal is unavailable.\n");
        return;
    }

    char loginId[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("\n--- Manager Login ---\n");
    printf("Enter Login ID: ");
    fgets(loginId, sizeof(loginId), stdin);
    loginId[strcspn(loginId, "\n")] = '\0';

    printf("Enter Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    int loggedInIndex = verify_manager_login(managers, manager_count, loginId, password);

    if (loggedInIndex != -1)
    {
        printf("\nLogin successful! Welcome, %s.\n", managers[loggedInIndex].managerName);
        manager_menu(&managers[loggedInIndex]);
        // We will create this manager_menu function next
        // manager_menu(&managers[loggedInIndex]);
    }
    else
    {
        printf("\nLogin failed. Invalid credentials.\n");
    }
}