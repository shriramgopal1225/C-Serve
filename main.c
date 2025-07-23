#include <stdio.h>
#include "structs.h"
#include "user_management.h"
#include "app_logic.h"
#include "manager_logic.h" // Add the new header

void customer_portal() {
    User users[MAX_USERS];
    int num_users = 0;
    load_users_from_file(users, &num_users);

    int choice = 0;
    while (choice != 3) {
        printf("\n--- Customer Portal ---\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        int c; while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 1) {
            register_user(users, &num_users);
        } else if (choice == 2) {
            int loggedInIndex = login_user(users, num_users);
            if (loggedInIndex != -1) {
                printf("\nLogin successful!\n");
                runUserSession(&users[loggedInIndex]);
            } else {
                printf("\nLogin failed. Please check your username and password.\n");
            }
        }
    }
}

int main() {
    int choice = 0;
    while (choice != 3) {
        printf("\nWelcome to C-Serve!\n");
        printf("---------------------\n");
        printf("1. Customer Portal\n");
        printf("2. Manager Portal\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        int c; while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 1) {
            customer_portal();
        } else if (choice == 2) {
            handleManagerSession();
        }
    }
    printf("\nApplication finished.\n");
    return 0;
}