# Car Rental System

## Overview

This Car Rental System is developed using Object-Oriented Programming (OOP) concepts in C++ and utilizes MySQL for database management.

## Setup Instructions

### Setting Up the Environment

1. **Setup Microsoft Visual Studio and MySQL Connector for C++:**
   Follow the instructions provided [here](https://www.geeksforgeeks.org/how-to-setup-mysql-database-in-visual-studio-2022-for-a-cpp-application/) to set up MySQL and the C++ connector in Microsoft Visual Studio.

### Running the Program

1. **Open the Project:**
   Use Microsoft Visual Studio to open the project folder as guided in the link above.

2. **Add the Code:**
   Copy the `.cpp` file into the project files.

3. **Build the Code:**
   Press `Ctrl + Shift + B` to build the code.

4. **Run the Code:**
   Press `Ctrl + F5` to run the program.

5. **Navigate the Program:**
   Enter the corresponding option in the terminal and press Enter to navigate through the program.

## Credentials

Initially, you can only log in using the following credentials:

### Customers
| Username  | Password |
|-----------|----------|
| customer1 | password1 |
| customer2 | password2 |
| customer3 | password3 |
| customer4 | password4 |
| customer5 | password5 |

### Employees
| Username  | Password |
|-----------|----------|
| employee1 | password1 |
| employee2 | password2 |
| employee3 | password3 |
| employee4 | password4 |
| employee5 | password5 |

### Manager
| Username | Password |
|----------|----------|
| manager  | password |

After logging in, you can add new IDs if logged in as the manager.

**Note:** Each ID must be unique.

## Functionality

### Customer Functions
- **rentCar**: Rent a car.
- **returnCar**: Return a rented car.
- **clearDue**: Clear any outstanding dues.
- **showAllCars**: Display all available cars.
- **showMyCars**: Show the cars rented by the customer.
- **logout**: Log out of the system.

### Employee Functions
- **rentCar**: Rent a car.
- **returnCar**: Return a rented car.
- **clearDue**: Clear any outstanding dues.
- **showAllCars**: Display all available cars.
- **showMyCars**: Show the cars rented by the employee.
- **logout**: Log out of the system.

### Manager Functions
- **addCustomer**: Add a new customer.
- **updateCustomer**: Update customer details.
- **deleteCustomer**: Delete a customer.
- **addEmployee**: Add a new employee.
- **updateEmployee**: Update employee details.
- **deleteEmployee**: Delete an employee.
- **addCar**: Add a new car.
- **updateCar**: Update car details.
- **deleteCar**: Delete a car.
- **carDatabase**: View the car database.
- **customerDatabase**: View the customer database.
- **employeeDatabase**: View the employee database.
- **logout**: Log out of the system.

## Notes
- There is only one manager, and this role cannot be replaced.
- Ensure that all IDs (username) are unique across customers, employees, and the manager.

Feel free to modify and extend the functionalities as needed.

