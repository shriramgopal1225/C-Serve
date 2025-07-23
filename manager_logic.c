#include <stdio.h>
#include <string.h>

#include "manager_logic.h"
#include "structs.h" // We need this for the Manager struct

int load_managers_from_file(Manager managers[]) {
    FILE* file = fopen("data/managers.csv", "r");
    if (file == NULL) {
        printf("Error: Could not open managers.csv\n");
        return 0;
    }

    char buffer[BUFFER_SIZE];
    int count = 0;
    fgets(buffer, sizeof(buffer), file); // Skip header line

    while (fgets(buffer, sizeof(buffer), file) && count < MAX_MANAGERS) {
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

int verify_manager_login(Manager managers[], int count, const char* loginId, const char* password) {
    for (int i = 0; i < count; i++) {
        if (strcmp(managers[i].loginId, loginId) == 0 && strcmp(managers[i].password, password) == 0) {
            return i; // Return the index of the logged-in manager on success
        }
    }
    return -1; // Return -1 on failure
}

void handleManagerSession() {
    Manager managers[MAX_MANAGERS];
    int manager_count = load_managers_from_file(managers);

    if (manager_count == 0) {
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

    if (loggedInIndex != -1) {
        printf("\nLogin successful! Welcome, %s.\n", managers[loggedInIndex].managerName);
        // We will create this manager_menu function next
        // manager_menu(&managers[loggedInIndex]); 
    } else {
        printf("\nLogin failed. Invalid credentials.\n");
    }
}