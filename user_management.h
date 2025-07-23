#ifndef USER_MANAGEMENT_H
#define USER_MANAGEMENT_H

#include "structs.h" // We need this to know what a 'User' is

void register_user(User users[], int* num_users);
int login_user(User users[], int num_users);
void save_user_to_file(User user);
void load_users_from_file(User users[], int* num_users);

#endif