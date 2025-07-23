#include <stdio.h>
#include <string.h> // For strcpy, strtok, strcmp
#include <stdlib.h> // For atoi
#include <ctype.h>

#include "user_management.h" // Include its own header file

#define FILENAME "data/users.csv"

void register_user(User users[], int *num_users)
{
    if ((*num_users) >= MAX_USERS)
    {
        printf("User limit reached. Cannot register more users.\n");
        return;
    }

    User new_user;
    printf("Enter your name: ");
    fgets(new_user.name, MAX_NAME_LENGTH, stdin);
    new_user.name[strcspn(new_user.name, "\n")] = '\0';

    printf("Enter your username: ");
    fgets(new_user.username, MAX_USERNAME_LENGTH, stdin);
    new_user.username[strcspn(new_user.username, "\n")] = '\0';

    printf("Enter your password: ");
    fgets(new_user.password, MAX_PASSWORD_LENGTH, stdin);
    new_user.password[strcspn(new_user.password, "\n")] = '\0';

    printf("Enter your phone number (10 digits): ");
    fgets(new_user.number, sizeof(new_user.number), stdin);
    new_user.number[strcspn(new_user.number, "\n")] = '\0';

    if (strlen(new_user.number) != 10 || !isdigit(new_user.number[0]))
    {
        printf("Invalid phone number.\n");
        return;
    }

    printf("Enter your address: ");
    fgets(new_user.address, MAX_ADDRESS_LENGTH, stdin);
    new_user.address[strcspn(new_user.address, "\n")] = '\0';

    printf("Enter your pincode (6 digits): ");
    fgets(new_user.pincode, sizeof(new_user.pincode), stdin);
    new_user.pincode[strcspn(new_user.pincode, "\n")] = '\0';

    if (strlen(new_user.pincode) != 6 || !isdigit(new_user.pincode[0]))
    {
        printf("Invalid pincode.\n");
        return;
    }

    printf("Enter a secret question for password recovery: ");
    fgets(new_user.secret_question, MAX_SECRET_QUESTION_LENGTH, stdin);
    new_user.secret_question[strcspn(new_user.secret_question, "\n")] = '\0';

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Enter the answer to the secret question: ");
    fgets(new_user.secret_answer, MAX_SECRET_ANSWER_LENGTH, stdin);
    new_user.secret_answer[strcspn(new_user.secret_answer, "\n")] = '\0';

    new_user.loyalty_points = 0;
    users[(*num_users)++] = new_user;

    save_user_to_file(new_user);
    printf("Registration successful!\n");
}

int login_user(User users[], int num_users)
{
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter your username: ");
    fgets(username, MAX_USERNAME_LENGTH, stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("Enter your password: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strcspn(password, "\n")] = '\0';

    for (int i = 0; i < num_users; i++)
    {
        // ---- START: Temporary Debug Code ----
        /* printf("\n-- Checking record %d --\n", i);
        printf("File Username: |%s| (Length: %zu)\n", users[i].username, strlen(users[i].username));
        printf("Your Username: |%s| (Length: %zu)\n", username, strlen(username));
        printf("File Password: |%s| (Length: %zu)\n", users[i].password, strlen(users[i].password));
        printf("Your Password: |%s| (Length: %zu)\n", password, strlen(password));
        printf("----------------------\n"); */
        // ---- END: Temporary Debug Code ----
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            // printf("Login successful! Welcome, %s.\n", users[i].name);
            return i;
        }
    }
    // printf("Invalid username or password.\n");
    return -1;
}

void save_user_to_file(User user)
{
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        printf("Error: Unable to open the file for writing.\n");
        return;
    }

    fprintf(file, "%s,%s,%s,%s,%s,%s,%d,%s,%s\n",
            user.name,
            user.username,
            user.password,
            user.number,
            user.address,
            user.pincode,
            user.loyalty_points,
            user.secret_question,
            user.secret_answer);

    fclose(file);
}

void load_users_from_file(User users[], int *num_users)
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        // No file exists, which is fine on first run.
        return;
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file) && (*num_users) < MAX_USERS)
    {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline

        char *token;

        token = strtok(buffer, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].name, token, MAX_NAME_LENGTH);
        users[(*num_users)].name[MAX_NAME_LENGTH - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].username, token, MAX_USERNAME_LENGTH);
        users[(*num_users)].username[MAX_USERNAME_LENGTH - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].password, token, MAX_PASSWORD_LENGTH);
        users[(*num_users)].password[MAX_PASSWORD_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        // The size for 'number' is 11, from its definition in structs.h
        strncpy(users[(*num_users)].number, token, 11);
        users[(*num_users)].number[10] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].address, token, MAX_ADDRESS_LENGTH);
        users[(*num_users)].address[MAX_ADDRESS_LENGTH - 1] = '\0';
        
        token = strtok(NULL, ",");
        if (token == NULL) continue;
        // The size for 'pincode' is 7, from its definition in structs.h
        strncpy(users[(*num_users)].pincode, token, 7);
        users[(*num_users)].pincode[6] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        users[(*num_users)].loyalty_points = atoi(token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].secret_question, token, MAX_SECRET_QUESTION_LENGTH);
        users[(*num_users)].secret_question[MAX_SECRET_QUESTION_LENGTH - 1] = '\0';

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strncpy(users[(*num_users)].secret_answer, token, MAX_SECRET_ANSWER_LENGTH);
        users[(*num_users)].secret_answer[MAX_SECRET_ANSWER_LENGTH - 1] = '\0';

        (*num_users)++;
    }

    fclose(file);
}