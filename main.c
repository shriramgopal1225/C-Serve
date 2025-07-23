#include <stdio.h>
#include "structs.h"         // Our data dictionary
#include "user_management.h" // Our NEW module for user functions
#include "app_logic.h"      // Our NEW module for application logic

int main()
{
    User users[MAX_USERS];
    int num_users = 0;

    load_users_from_file(users, &num_users);

    int choice = 0;
    printf("Welcome to C-Serve!\n");
    printf("---------------------\n");

    while (choice != 3)
    {
        printf("\n1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        scanf("%d", &choice);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

        if (choice == 1)
        {
            register_user(users, &num_users); // Calling a function from another file!
        }
        else if (choice == 2)
        {
            int loggedIndex = login_user(users, num_users);

            if (loggedIndex >= 0)
            {
                printf("Login successful!\n");
                runUserSession(&users[loggedIndex]); // Assuming the first user is logged in
            }
            else
            {
                printf("Login failed. Please check your username and password.\n");
            } // Calling a function from another file!
        }
    }

    printf("\nApplication finished.\n");

    return 0;
}