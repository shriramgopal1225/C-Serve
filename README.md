# C-Serve: A Console-Based Food Delivery Application

C-Serve is a comprehensive food delivery application that runs entirely in the terminal. Built in C, this project simulates the core functionality of a real-world food delivery service, focusing on a clean modular code structure, data persistence, and fundamental computer science algorithms to create a robust and scalable system.

## Key Features

* **Modular Architecture**: The project is logically separated into modules for user management, location-based logic, cart functionality, and more, promoting clean code and maintainability.
* **User Authentication**: A complete user account system where users can register for a new account and log in. All user data is saved to and loaded from a `users.csv` file.
* **Dynamic Restaurant Discovery**: The application uses **Dijkstra's algorithm** to intelligently find and sort nearby restaurants based on a combined score of distance and user ratings, providing the user with the most relevant options.
* **Full Shopping Cart System**: Users can browse restaurant menus, add items to a shopping cart, view the cart's contents, and proceed to a simulated payment.
* **Data Persistence**: All critical application data, including user accounts and restaurant menus, is managed through external `.csv` files, ensuring data is saved between sessions.

## Project Structure

The codebase is organized into the following modules:
* `main`: The main entry point and application loop.
* `structs.h`: A centralized header for all data structures and constants.
* `user_management`: Handles user registration and login.
* `location_logic`: Implements Dijkstra's algorithm and manages location data.
* `cart_logic`: Manages menu loading, cart operations, and the payment process.

## How to Compile and Run

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/shriramgopal1225/C-Serve
    ```
2.  **Navigate to the project directory:**
    ```bash
    cd C-Serve
    ```
3.  **Compile the project using the provided Makefile:**
    ```bash
    make
    ```
4.  **Run the application:**
    ```bash
    ./c_serve
    ```

## Future Work

This project is actively being developed. The next steps include:
* [ ] Implementing the logic to save the user's updated e-wallet balance and loyalty points after a purchase.
* [ ] Integrating the Restaurant Manager module for adding and updating menu items.
* [ ] Adding advanced cart features like removing items and modifying quantities.
* [ ] Externalizing hardcoded data (like hotel locations) into `.csv` files.