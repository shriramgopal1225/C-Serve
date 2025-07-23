#ifndef STRUCTS_H
#define STRUCTS_H

#include <limits.h>

// --- Constants ---
#define MAX_USERS 100
#define MAX_NAME_LENGTH 100
#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_ADDRESS_LENGTH 200
#define MAX_SECRET_QUESTION_LENGTH 200
#define MAX_SECRET_ANSWER_LENGTH 100

#define MAX_LOCATIONS 5
#define MAX_HOTELS 5
#define MAX_MENU_ITEMS 600
#define MAX_ITEMS 500
#define MAX_DRIVERS 100

#define MAX_HOTEL_NAME_LENGTH 100
#define MAX_LOCATION_LENGTH 50
#define MAX_DRIVER_NAME_LENGTH 50
#define MAX_PHONE_NUMBER_LENGTH 15
#define MAX_STATUS_LENGTH 10
#define MAXCHAR 100

#define BUFFER_SIZE 1024
#define INF INT_MAX
#define MIN_SUPPORT 2
#define PRICE_THRESHOLD 20
#define MAX_ASSOCIATIONS 100


// --- Data Structures ---

typedef struct {
    char name[MAX_NAME_LENGTH];
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH]; 
    char number[MAX_PHONE_NUMBER_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char pincode[7];
    int loyalty_points;
    char secret_question[MAX_SECRET_QUESTION_LENGTH];
    char secret_answer[MAX_SECRET_ANSWER_LENGTH];
    double ewallet_balance;
} User;

typedef struct {
    char location[MAX_LOCATION_LENGTH];
    char name[MAX_DRIVER_NAME_LENGTH];
    char status[MAX_STATUS_LENGTH];
    char phone_number[MAX_PHONE_NUMBER_LENGTH];
    int x;
    int y;
    int rating;
} Driver;

struct Hotel {
    char name[MAX_HOTEL_NAME_LENGTH];
    float rating;
};

typedef struct MenuItem {
    char name[MAXCHAR];
    float price;
    char type[MAXCHAR];
    char restaurant[MAXCHAR];
    int quantity;
    float rating;
} MenuItem;

typedef struct {
    int items[2];
    int count;
} Itemset;

typedef struct {
    char items[MAX_ITEMS][MAX_NAME_LENGTH];
    int count;
} ItemSet;

typedef struct {
    char restaurant[MAX_NAME_LENGTH];
    int item_id;
    char item_name[MAX_NAME_LENGTH];
    float price;
    char veg_non_veg[8]; 
    int quantity;
    float rating;
} MenuItem2;

typedef struct {
    MenuItem2 items[MAX_ITEMS];
    int quantity[MAX_ITEMS];
    int num_items;
    float total;
} Cart;

#endif