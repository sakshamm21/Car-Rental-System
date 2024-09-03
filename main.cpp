#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include<string>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <iomanip>
#include<iterator>
#include <chrono>
#include<ctime>
#include<sstream>

using namespace std;
using namespace sql;
using namespace std::chrono;

class DatabaseConnector {
private:
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    void createDatabase(const string& dbName) {
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("CREATE DATABASE IF NOT EXISTS " + dbName);
            delete stmt;
            std::cout << "Database created successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

public:
    DatabaseConnector(const string& host, const string& user, const string& password, const string& database) {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect(host, user, password);
            if (database != "") {
                con->setSchema(database);
            }
            else {
                // If the database name is empty, create a default database
                const string defaultDatabaseName = "yo";
                createDatabase(defaultDatabaseName);
                con->setSchema(defaultDatabaseName);
            }
        }
        catch (sql::SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    ~DatabaseConnector() {
        delete con;
    }

    sql::Connection* getConnection() const {
        return con;
    }

};

class User {
public:
    int id;
    string username;
    string password;

    User(int id, const string& username, const string& password): id(id), username(username), password(password) {}
    int daysBetweenDates(const std::string& date1, const std::string& date2);
    string getCurrentDate();
    int isValidDateString(const std::string& dateStr);
};

string User::getCurrentDate() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << localTime->tm_mday << "-"
        << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "-"
        << (localTime->tm_year + 1900);

    return oss.str();
}

class Car {
private:
    int id;
    string model;
    string car_condition;
    int rent;
    bool isAvailable;
    int user_id=0;

public:
    Car(int id, const string& model, const string& condition, int rent, bool isAvailable) : id(id), model(model), car_condition(condition), rent(rent), isAvailable(isAvailable) {}
    int days=0;
    int getrent() { return rent; }
    int getid() { return id; }
    void setuserid(int userloid) { user_id = userloid; }
    string getcondition() { return car_condition; }
    string getmodel() { return model; }
    bool available() { return isAvailable; }
    void notavailable() { isAvailable = 0; }
    void yesavailable() { isAvailable = 1; }
    void addToDatabase(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            string available = isAvailable ? "true" : "false";
            ResultSet* res = stmt->executeQuery("SELECT COUNT(*) FROM cars WHERE id = " + to_string(id));
            res->next();
            int count = res->getInt(1);
            delete res;
            if(count==0) stmt->execute("INSERT INTO cars (id, model, car_condition, rent, isAvailable) VALUES (" + to_string(id) + ", '" + model + "', '" + car_condition + "', " + to_string(rent) + ", " + available + ")");
            delete stmt;
            if(count==0) std::cout << "Car added successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
    void addToDatabase1(Connection* con) {
        try {
            string available = isAvailable ? "true" : "false";
            Statement* stmt = con->createStatement();
            stmt->execute("INSERT INTO cars (id, model, car_condition, rent, isAvailable) VALUES (" + to_string(id) + ", '" + model + "', '" + car_condition + "', " + to_string(rent) + ", " + available + ")");
            delete stmt;
            std::cout << "Car added successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    bool operator==(const Car& other) const {
        return id == other.id;
    }

    void Update(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            string available = isAvailable ? "true" : "false";
            stmt->execute("UPDATE cars SET model = '" + model + "', car_condition = '" + car_condition + "', rent = " + to_string(rent) + ", isAvailable = " + available + " WHERE id = " + to_string(id));
            delete stmt;
            std::cout << "Car updated successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    /*void Delete(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("DELETE FROM cars WHERE id = " + to_string(id));
            delete stmt;
            std::cout << "Car deleted successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void Search(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE id = " + to_string(id));
            if (res->next()) {
                std::cout << "Car found:" << endl;
                std::cout << "ID: " << res->getInt("id") << endl;
                std::cout << "Model: " << res->getString("model") << endl;
                std::cout << "Condition: " << res->getString("car_condition") << endl;
                std::cout << "Rent: " << res->getInt("rent") << endl;
                std::cout << "Available: " << (res->getBoolean("isAvailable") ? "Yes" : "No") << endl;
            }
            else {
                std::cout << "Car with ID " << id << " not found." << endl;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }*/
};

class Customer : public User{
private:
    /*int id;
    string username;
    string password;*/
    int customer_record;
    double fineDue;

public:
    //Customer(int id, const string& username, const string& password) : id(id), username(username), password(password), customer_record(8), fineDue(0) {}
    Customer(int id, const string& username, const string& password): User(id, username, password), customer_record(8), fineDue(0) {}
    int getID() const { return id; }
    const string& getUsername() const { return username; }
    int getCustomerrecord() { return customer_record; }
    double getfineDue() const { return fineDue; }

    void addCustomer(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT COUNT(*) FROM customers WHERE id = " + to_string(id));
            res->next();
            int count = res->getInt(1);
            delete res; // Clean up the ResultSet object
            delete stmt; // Clean up the Statement object
            if (count == 0) {
                stmt = con->createStatement();
                stmt->execute("INSERT INTO customers (id, username, password) VALUES (" + to_string(id) + ", '" + username + "', '" + password + "')");
                delete stmt; // Clean up the Statement object
                std::cout << "Customer added successfully." << endl;
            }
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
    void addCustomer1(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("INSERT INTO customers (id, username, password) VALUES (" + to_string(id) + ", '" + username + "', '" + password + "')");
            delete stmt; // Clean up the Statement object
            std::cout << "Customer added successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    /*void Search(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE id = " + to_string(id));
            if (res->next()) {
                std::cout << "Customer found:" << endl;
                std::cout << "ID: " << res->getInt("id") << endl;
                std::cout << "Username: " << res->getString("username") << endl;
            }
            else {
                std::cout << "Customer with ID " << id << " not found." << endl;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }*/

    void rentCar(Car& car, int days, Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers");
            int b = 8;
            while (res->next()) {
                int id1 = res->getInt("id");
                if (id1 == id)
                {
                    b = res->getInt("customer_record");
                    if (b < 5)
                    {
                        cout << "Customer_record too low!" << endl;
                    }
                    break;
                }
            }
            delete res;
            delete stmt;
            if (b < 5) return;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        bool a = 1;
        bool hihi = 1;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                int id = res->getInt("id");
                if (id == car.getid())
                {
                    bool b = res->getBoolean("isAvailable");
                    if (b == 0) a = 0;
                    hihi = 0;
                    break;
                }
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        if (hihi)
        {
            cout << "id not found" << endl;
            return;
        }
        if (a==1)
        {
            car.notavailable();
            car.days = days;
            fineDue = car.getrent() * days;
            string date = getCurrentDate();
            cout << date << endl;
            try {
                Statement* stmt = con->createStatement();
                // Update fineDue in the database for the respective customer or employee
                int customerId = id; // Assuming you have a method to get the customer's ID
                stmt->execute("UPDATE cars SET user_id = " + to_string(customerId) + " WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE customers SET fineDue = fineDue + " + to_string(fineDue) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET isAvailable = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_time = '" + date + "' WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_days = " + to_string(days) + " WHERE id = " + to_string(car.getid()));
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            std::cout << "Car rented successfully" << endl;
        }
        else {
            std::cout << "Car not available at the moment" << endl;
        }
    }

    void returnCar(Car& car, int day, vector<Car>& rentedCars, Connection* con) {
        int days;
        bool hihi = 1;
        if (find(rentedCars.begin(), rentedCars.end(), car) != rentedCars.end())
        {
            rentedCars.erase(find(rentedCars.begin(), rentedCars.end(), car));
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
                while (res->next()) {
                    int id = res->getInt("id");
                    if (id == car.getid())
                    {
                        days = res->getInt("rent_days");
                        hihi = 0;
                        break;
                    }
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            if (hihi) {
                cout << "id not found" << endl;
                return;
            }
            if (day > days) {
                fineDue = (day - days) * 0.1;
                customer_record = -1 * (day - days) * 0.1;
            }
            else {
                fineDue = 0;
                customer_record = 0;
            }
            try {
                Statement* stmt = con->createStatement();
                // Update fineDue in the database for the respective customer or employee
                stmt->execute("UPDATE cars SET user_id = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE customers SET fineDue = fineDue + " + to_string(fineDue) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET isAvailable = 1 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE customers SET customer_record = customer_record + " + to_string(customer_record) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET rent_days = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_time = '01-01-00' WHERE id = " + to_string(car.getid()));

                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            car.yesavailable();
        }
    }

    void clearDue(Connection* con) {
        fineDue = 0;
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("UPDATE customers SET fineDue = 0 WHERE id = " + to_string(id));
            std::cout << "Due Cleared" << endl;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    void showAllCars(Connection* con) {
        vector<Car> allcars;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                int id = res->getInt("id");
                string model = res->getString("model");
                string condition = res->getString("car_condition");
                int rent = res->getInt("rent");
                bool isAvailable = res->getBoolean("isAvailable");
                allcars.push_back(Car(id, model, condition, rent, isAvailable));
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        int i = 0;
        while (i < allcars.size()) {
            cout << "Serial no. " << i + 1 << " | id: " << allcars[i].getid() << " | model: " << allcars[i].getmodel() << " | condition: " << allcars[i].getcondition() << " | rent: " << allcars[i].getrent() << " | availability: " << allcars[i].available() << endl;
            i++;
        }
    }
    void showMyCars(Connection* con) {
        int p = 1;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                if (id == res->getInt("user_id"))
                {
                    cout << "Serial no.: " << p <<" ";
                    int id = res->getInt("id");
                    std::cout << "| id: " << id << " ";
                    string model = res->getString("model");
                    std::cout << "| model: " << model << " ";
                    string car_condition = res->getString("car_condition");
                    std::cout << "| car_conditioncar: " << car_condition << " ";
                    int rent = res->getInt("rent");
                    std::cout << "| rent: " << rent << " ";
                    bool isAvailable = res->getBoolean("isAvailable");
                    std::cout << "| isAvailable: " << isAvailable << " ";
                    int user_id = res->getInt("user_id");
                    std::cout << "| user_id" << user_id << endl;
                    p++;
                }
            }
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
};

class Employee : public User{
private:
    /*int id;
    string username;
    string password;*/
    int employee_record;
    double fineDue;
    

public:
    Employee(int id, const string& username, const string& password) : User(id, username, password), employee_record(8), fineDue(0) {}

    int getID() const { return id; }
    const string& getUsername() const { return username; }
    int getEmployeerecord() { return employee_record; }
    double getfineDue() const { return fineDue; }

    void addEmployee(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT COUNT(*) FROM employees WHERE id = " + to_string(id));
            res->next();
            int count = res->getInt(1);
            delete res; // Clean up the ResultSet object
            delete stmt; // Clean up the Statement object
            if (count == 0) {
                stmt = con->createStatement();
                stmt->execute("INSERT INTO employees (id, username, password) VALUES (" + to_string(id) + ", '" + username + "', '" + password + "')");
                delete stmt; // Clean up the Statement object
                std::cout << "Employee added successfully." << endl;
            }
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
    void addEmployee1(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("INSERT INTO employees (id, username, password) VALUES (" + to_string(id) + ", '" + username + "', '" + password + "')");
            delete stmt; // Clean up the Statement object
            std::cout << "Employee added successfully." << endl;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }

    /*void Search(Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE id = " + to_string(id));
            if (res->next()) {
                std::cout << "Employee found:" << endl;
                std::cout << "ID: " << res->getInt("id") << endl;
                std::cout << "Username: " << res->getString("username") << endl;
            }
            else {
                std::cout << "Employee with ID " << id << " not found." << endl;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }*/

    void rentCar(Car& car, int days, Connection* con) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees");
            int b = 8;
            while (res->next()) {
                int id1 = res->getInt("id");
                if (id1 == id)
                {
                    b = res->getInt("employee_record");
                    if (b < 5)
                    {
                        cout << "Employee_record too low!" << endl;
                    }
                    break;
                }
            }
            delete res;
            delete stmt;
            if (b < 5) return;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        bool a = 1;
        bool hihi = 1;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                int id = res->getInt("id");
                if (id == car.getid())
                {
                    bool b = res->getBoolean("isAvailable");
                    if (b == 0) a = 0;
                    hihi = 0;
                    break;
                }
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        if (hihi)
        {
            cout << "id not found" << endl;
            return;
        }
        if (a == 1)
        {
            car.notavailable();
            car.days = days;
            fineDue = car.getrent() * days * 0.85;
            string date = getCurrentDate();
            cout << date << endl;

            try {
                Statement* stmt = con->createStatement();
                // Update fineDue in the database for the respective customer or employee
                int employeeId = id; // Assuming you have a method to get the customer's ID
                stmt->execute("UPDATE cars SET user_id = " + to_string(employeeId) + " WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE employees SET fineDue = fineDue + " + to_string(fineDue) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET isAvailable = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_time = '" + date + "' WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_days = " + to_string(days) + " WHERE id = " + to_string(car.getid()));
                
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            std::cout << "Car rented successfully" << endl;
        }
        else {
            std::cout << "Car not available at the moment" << endl;
        }
    }

    void returnCar(Car& car, int day, vector<Car>& rentedCars, Connection* con) {
        int days;
        bool hihi = 1;
        if (find(rentedCars.begin(), rentedCars.end(), car) != rentedCars.end())
        {
            rentedCars.erase(find(rentedCars.begin(), rentedCars.end(), car));
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
                while (res->next()) {
                    int id = res->getInt("id");
                    if (id == car.getid())
                    {
                        days = res->getInt("rent_days");
                        hihi = 0;
                        break;
                    }
                }
                delete res;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            if (hihi)
            {
                cout << "id not found" << endl;
                return;
            }
            if (day > days) {
                fineDue = (day - days) * 0.1;
                employee_record = -1 * (day - days) * 0.1;
            }
            else {
                fineDue = 0;
                employee_record = 0;
            }
            try {
                Statement* stmt = con->createStatement();
                // Update fineDue in the database for the respective customer or employee
                stmt->execute("UPDATE cars SET user_id = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE employees SET fineDue = fineDue + " + to_string(fineDue) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET isAvailable = 1 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE employees SET employee_record = employee_record + " + to_string(employee_record) + " WHERE id = " + to_string(id));
                stmt->execute("UPDATE cars SET rent_days = 0 WHERE id = " + to_string(car.getid()));
                stmt->execute("UPDATE cars SET rent_time = '01-01-00' WHERE id = " + to_string(car.getid()));
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
            car.yesavailable();
        }
    }

    void clearDue(Connection* con) {
        fineDue = 0;
        try {
            Statement* stmt = con->createStatement();
            stmt->execute("UPDATE employees SET fineDue = 0 WHERE id = " + to_string(id));
            std::cout << "Due Cleared" << endl;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
    }
    void showAllCars(Connection* con) {
        vector<Car> allcars;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                int id = res->getInt("id");
                string model = res->getString("model");
                string condition = res->getString("car_condition");
                int rent = res->getInt("rent");
                bool isAvailable = res->getBoolean("isAvailable");
                allcars.push_back(Car(id, model, condition, rent, isAvailable));
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        int i = 0;
        while (i < allcars.size()) {
            cout << "Serial no. "<< i+1<<" | id: " << allcars[i].getid() << " | model: " << allcars[i].getmodel() << " | condition: " << allcars[i].getcondition() << " | rent: " << allcars[i].getrent() << " | availability: " << allcars[i].available() << endl;
            i++;
        }
    }
    void showMyCars(Connection* con) {
        int p = 1;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                if (id == res->getInt("user_id"))
                {
                    cout << "Serial no.: " << p << " ";
                    int id = res->getInt("id");
                    std::cout << "| id: " << id << " ";
                    string model = res->getString("model");
                    std::cout << "| model: " << model << " ";
                    string car_condition = res->getString("car_condition");
                    std::cout << "| car_conditioncar: " << car_condition << " ";
                    int rent = res->getInt("rent");
                    std::cout << "| rent: " << rent << " ";
                    bool isAvailable = res->getBoolean("isAvailable");
                    std::cout << "| isAvailable: " << isAvailable << " ";
                    int user_id = res->getInt("user_id");
                    std::cout << "| user_id" << user_id << endl;
                    p++;
                }
            }
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
};

class Manager : public User{
private:
    /*int id;
    string username;
    string password;*/
    // employee_record
    // Other details...

public:
    Manager(int id, const string& username, const string& password) : User(id, username, password) {}

    int getID() const { return id; }
    const string& getUsername() const { return username; }

    void addCustomer(Connection* con, Customer& customer) {
        if (con) {
            customer.addCustomer(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }

    void addCustomer1(Connection* con, Customer& customer) {
        if (con) {
            customer.addCustomer1(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }
   
    void updateCustomer(Connection* con, int id, int newid, string newusername, int newcustomer_record, double newfineDue) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE id = " + std::to_string(id));

            if (res->next()) {
                std::string username = res->getString("username");
                int customer_record = res->getBoolean("customer_record");
                double fineDue = res->getDouble("fineDue");
                stmt->executeUpdate("UPDATE customers SET id = " + std::to_string(newid) +
                    ", username = '" + newusername +
                    "', customer_record = " + std::to_string(newcustomer_record) +
                    ", fineDue = " + std::to_string(newfineDue) +
                    " WHERE id = " + std::to_string(id));
            }
            else {
                std::cerr << "Customer with ID " << id << " not found." << std::endl;
            }

            // Clean up
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    void deleteCustomer(Connection* con, int customerId) {
        try {
            Statement* stmt = con->createStatement();
            stmt->executeUpdate("DELETE FROM customers WHERE id = " + std::to_string(customerId));
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    //void searchCustomer(Connection* con, Customer& customer) {
    //    // Implementation
    //    if (con) {
    //        customer.Search(con);
    //    }
    //    else {
    //        cerr << "Failed to connect to the database." << endl;
    //    }
    //}

    void addEmployee(Connection* con, Employee& employee) {
        if (con) {
            employee.addEmployee(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }
    void addEmployee1(Connection* con, Employee& employee) {
        if (con) {
            employee.addEmployee1(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }
    void updateEmployee(Connection* con, int id, int newid, string newusername, int newemployee_record, double newfineDue) {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE id = " + std::to_string(id));

            if (res->next()) {
                std::string username = res->getString("username");
                int employee_record = res->getBoolean("employee_record");
                double fineDue = res->getDouble("fineDue");
                stmt->executeUpdate("UPDATE employees SET id = " + std::to_string(newid) +
                    ", username = '" + newusername +
                    "', employee_record = " + std::to_string(newemployee_record) +
                    ", fineDue = " + std::to_string(newfineDue) +
                    " WHERE id = " + std::to_string(id));
            }
            else {
                std::cerr << "Employee with ID " << id << " not found." << std::endl;
            }

            // Clean up
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    void deleteEmployee(Connection* con, int employeeId) {
        try {
            Statement* stmt = con->createStatement();
            stmt->executeUpdate("DELETE FROM employees WHERE id = " + std::to_string(employeeId));
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    //void searchEmployee(Connection* con, Employee& employee) {
    //    // Implementation
    //    if (con) {
    //        employee.Search(con);
    //    }
    //    else {
    //        cerr << "Failed to connect to the database." << endl;
    //    }
    //}

    void addCar(Connection* con, Car& car) {
        // Implementation
        if (con) {
            car.addToDatabase(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }
    void addCar1(Connection* con, Car& car) {
        // Implementation
        if (con) {
            car.addToDatabase1(con);
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }
    }
    void updateCar(Connection* con, int id, int newid, string newmodel, string newcondition, int newrent, bool newisAvailable) {
        // Implementation
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE id = " + std::to_string(id));

            if (res->next()) {
                std::string model = res->getString("model");
                string condition = res->getString("car_condition");
                int rent = res->getInt("rent");
                bool isAvailable = res->getBoolean("isAvailable");
                stmt->executeUpdate("UPDATE cars SET "
                    "id = " + std::to_string(newid) +
                    ", model = '" + newmodel +
                    "', car_condition = '" + newcondition +
                    "', rent = " + std::to_string(newrent) +
                    ", isAvailable = " + std::to_string(newisAvailable ? 1 : 0) +
                    " WHERE id = " + std::to_string(id));
            }
            else {
                std::cerr << "Car with ID " << id << " not found." << std::endl;
            }

            // Clean up
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    void deleteCar(Connection* con, int carId) {
        try {
            Statement* stmt = con->createStatement();
            stmt->executeUpdate("DELETE FROM cars WHERE id = " + std::to_string(carId));
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    //void searchCar(Connection* con, Car& car) {
    //    // Implementation
    //    if (con) {
    //        car.Search(con);
    //    }
    //    else {
    //        cerr << "Failed to connect to the database." << endl;
    //    }
    //}
    
};

vector<Car> getAllCars(Connection* con) {
    vector<Car> cars;
    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
        while (res->next()) {
            int id = res->getInt("id");
            string model = res->getString("model");
            string condition = res->getString("car_condition");
            int rent = res->getInt("rent");
            bool isAvailable = res->getBoolean("isAvailable");
            cars.push_back(Car(id, model, condition, rent, isAvailable));
        }
        delete res;
        delete stmt;
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
    }
    return cars;
}
vector<Customer> getAllCustomers(Connection* con) {
    vector<Customer> customers;
    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT * FROM customers");
        while (res->next()) {
            int id = res->getInt("id");
            string username = res->getString("username");
            string password = res->getString("password");
            customers.push_back(Customer(id, username, password));
        }
        delete res;
        delete stmt;
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
    }
    return customers;
}
vector<Employee> getAllEmployees(Connection* con) {
    vector<Employee> employees;
    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT * FROM employees");
        while (res->next()) {
            int id = res->getInt("id");
            string username = res->getString("username");
            string password = res->getString("password");
            employees.push_back(Employee(id, username, password));
        }
        delete res;
        delete stmt;
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
    }
    return employees;
}
vector<Manager> getManager(Connection* con) {
    vector<Manager> managers;
    try {
        Statement* stmt = con->createStatement();
        ResultSet* res = stmt->executeQuery("SELECT * FROM managers");
        while (res->next()) {
            int id = res->getInt("id");
            string username = res->getString("username");
            string password = res->getString("password");
            managers.push_back(Manager(id, username, password));
        }
        delete res;
        delete stmt;
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
    }
    return managers;
}
int Customer_functions(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, int i, vector<Car>&v, Connection*& con)
{
    cout << "-----------------------------------------------------------------------------------------" << endl;
    std::cout << "Functions you could perform: " << endl;
    std::cout << "rentCar" << endl;
    std::cout << "returnCar" << endl;
    std::cout << "clearDue" << endl;
    std::cout << "showAllCars" << endl;
    std::cout << "showMyCars" << endl;
    std::cout << "logout" << endl;
    string s;
    cin >> s;
    if (s == "showAllCars")
    {
        customers[i].showAllCars(con);
        if (Customer_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "showMyCars")
    {
        customers[i].showMyCars(con);
        if (Customer_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "rentCar")
    {
        customers[i].showAllCars(con);
        std::cout << "Which serial no. car?" << endl;
        int x;
        cin >> x;
        /*if (x > cars.size() || x < 1) {
            cout << "Invalid serial no." << endl;
            if(Customer_functions(cars, customers, employees, i, v, con)) return 1;
        }*/
        std::cout << "How many days?" << endl;
        int y;
        cin >> y;
        customers[i].rentCar(cars[x-1], y, con);
        if(Customer_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "returnCar")
    {
        int p = 1;
        v.clear();
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                if (customers[i].getID() == res->getInt("user_id"))
                {
                    cout << "Serial no.: " << p << " ";
                    int id = res->getInt("id");
                    std::cout << "| id: " << id << " ";
                    string model = res->getString("model");
                    std::cout << "| model: " << model << " ";
                    string car_condition = res->getString("car_condition");
                    std::cout << "| car_conditioncar: " << car_condition << " ";
                    int rent = res->getInt("rent");
                    std::cout << "| rent: " << rent << " ";
                    bool isAvailable = res->getBoolean("isAvailable");
                    std::cout << "| isAvailable: " << isAvailable << " ";
                    int user_id = res->getInt("user_id");
                    std::cout << "| user_id" << user_id << endl;
                    Car cario(id, model, car_condition, rent, isAvailable);
                    cario.setuserid(user_id);
                    v.push_back(cario);
                    p++;
                }
            }
            delete res;
            delete stmt;
            std::cout << "Choose which serial no. car?" << endl;
            int x;
            cin >> x;
            /*if (x > cars.size() || x < 1) {
                cout << "Invalid serial no." << endl;
                if(Customer_functions(cars, customers, employees, i, v, con)) return 1;
            }*/
            std::cout << "How many days?" << endl;
            int y;
            cin >> y;
            //vector<Car>yo = v;
            if (x >= 1) {
                customers[i].returnCar(v[x - 1], y, v, con);
            }
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
        if(Customer_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "clearDue")
    {
        customers[i].clearDue(con);
        if(Customer_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "logout")
    {
        return 1;
    }
    else if (Customer_functions(cars, customers, employees, i, v, con)) return 1;
    return 1;
}
int Employee_functions(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, int i, vector<Car>&v, Connection*& con)
{
    cout << "-----------------------------------------------------------------------------------------" << endl;
    std::cout << "Functions you could perform: " << endl;
    std::cout << "rentCar" << endl;
    std::cout << "returnCar" << endl;
    std::cout << "clearDue" << endl;
    std::cout << "showAllCars" << endl;
    std::cout << "showMyCars" << endl;
    std::cout << "logout" << endl;
    string s;
    cin >> s;
    if (s == "showAllCars")
    {
        employees[i].showAllCars(con);
        if (Employee_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "showMyCars")
    {
        employees[i].showMyCars(con);
        if (Employee_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "rentCar")
    {
        employees[i].showAllCars(con);
        std::cout << "Which serial no. car?" << endl;
        int x;
        cin >> x;
        /*if (x > cars.size() || x<1) {
            cout << "Invalid serial no." << endl;
            if(Employee_functions(cars, customers, employees, i, v, con)) return 1;
        }*/
        std::cout << "How many days?" << endl;
        int y;
        cin >> y;
        employees[i].rentCar(cars[x-1], y, con);
        if(Employee_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "returnCar")
    {
        int p = 1;
        v.clear();
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            while (res->next()) {
                if (employees[i].getID() == res->getInt("user_id"))
                {
                    cout << "Serial no.: " << p << " ";
                    int id = res->getInt("id");
                    std::cout << "| id: " << id << " ";
                    string model = res->getString("model");
                    std::cout << "| model: " << model << " ";
                    string car_condition = res->getString("car_condition");
                    std::cout << "| car_conditioncar: " << car_condition << " ";
                    int rent = res->getInt("rent");
                    std::cout << "| rent: " << rent << " ";
                    bool isAvailable = res->getBoolean("isAvailable");
                    std::cout << "| isAvailable: " << isAvailable << " ";
                    int user_id = res->getInt("user_id");
                    std::cout << "| user_id" << user_id << endl;
                    Car cario(id, model, car_condition, rent, isAvailable);
                    cario.setuserid(user_id);
                    v.push_back(cario);
                    p++;
                }
            }
            delete res;
            delete stmt;
            std::cout << "Choose which serial no. car?" << endl;
            int x;
            cin >> x;
            /*if (x > cars.size() || x < 1) {
                cout << "Invalid serial no." << endl;
                int t = Employee_functions(cars, customers, employees, i, v, con);
            }*/
            std::cout << "How many days?" << endl;
            int y;
            cin >> y;
            //vector<Car>yo = v;
            if (x >= 1) {
                employees[i].returnCar(v[x - 1], y, v, con);
            }
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
        if(Employee_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "clearDue")
    {
        employees[i].clearDue(con);
        if(Employee_functions(cars, customers, employees, i, v, con)) return 1;
    }
    else if (s == "logout")
    {
        return 1;
    }
    else if (Employee_functions(cars, customers, employees, i, v, con)) return 1;
    return 1;
}
int Manager_functions(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, Manager& manager, Connection*& con) {
    cout << "-----------------------------------------------------------------------------------------" << endl;
    std::cout << "Functions available:" << endl;
    std::cout << "add/update/deleteCustomer" << endl;
    std::cout << "add/update/deleteEmployee" << endl;
    std::cout << "add/update/deleteCar" << endl;
    std::cout << "carDatabase" << endl;
    std::cout << "customerDatabase" << endl;
    std::cout << "employeeDatabase" << endl;
    cout << "logout" << endl;
    string s;
    cin >> s;
    if (s == "carDatabase") {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
            int l = 0;
            while (res->next()) {
                cout << "Serial No.: " << l << " ";
                int id = res->getInt("id");
                std::cout << "id: " << id << " ";
                string model = res->getString("model");
                std::cout << "model: " << model << " ";
                string car_condition = res->getString("car_condition");
                std::cout << "car_conditioncar: " << car_condition << " ";
                int rent = res->getInt("rent");
                std::cout << "rent: " << rent << " ";
                bool isAvailable = res->getBoolean("isAvailable");
                std::cout << "isAvailable: " << isAvailable << " ";
                int user_id = res->getInt("user_id");
                std::cout << "user_id: " << user_id << endl;
                l++;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
        if (Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "employeeDatabase") {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees");
            int l = 0;
            while (res->next()) {
                cout << "Serial No.: " << l << " ";
                int id = res->getInt("id");
                std::cout << "id: " << id << " ";
                string username = res->getString("username");
                std::cout << "username: " << username << " ";
                int employee_record = res->getInt("employee_record");
                std::cout << "employee_record: " << employee_record << " ";
                double fineDue = res->getDouble("fineDue");
                std::cout << "fineDue: " << fineDue << endl;
                l++;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
        if (Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "customerDatabase") {
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers");
            int l = 0;
            while (res->next()) {
                cout << "Serial No.: " << l << " ";
                int id = res->getInt("id");
                std::cout << "id: " << id << " ";
                string username = res->getString("username");
                std::cout << "username: " << username << " ";
                int customer_record = res->getInt("customer_record");
                std::cout << "customer_record: " << customer_record << " ";
                double fineDue = res->getDouble("fineDue");
                std::cout << "fineDue: " << fineDue << endl;
                l++;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
        if (Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "logout")
    {
        return 1;
    }
    else if (s == "addCustomer") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        cout << "username: " << endl;
        string username;
        cin >> username;
        cout << "password: " << endl;
        string password;
        cin >> password;
        Customer C(id, username, password);
        manager.addCustomer1(con, C);
        if(Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "deleteCustomer") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE id = " + std::to_string(id));
            if (res->next()) {
                manager.deleteCustomer(con, id);
            }
            else {
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
            if(Manager_functions(cars, customers, employees, manager, con)) return 1;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else if (s == "updateCustomer") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        string username;
        int customer_record=8;
        double fineDue=0;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE id = " + std::to_string(id));
            if (res->next()) {
                username = res->getString("username");
                customer_record = res->getInt("customer_record");
                fineDue = res->getDouble("fineDue");
                cout << "id: " << id << " username: " << username << " customer_record: " << customer_record << " fineDue: " << fineDue << endl;
                int newid;
                string newusername;
                int newcustomer_record;
                double newfineDue;
                cout << "new_id: " << endl;
                cin >> newid;
                cout << "new_username" << endl;
                cin >> newusername;
                cout << "new_customer_record" << endl;
                cin >> newcustomer_record;
                cout << "new_fineDue" << endl;
                cin >> newfineDue;
                manager.updateCustomer(con, id, newid, newusername, newcustomer_record, newfineDue);
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            else{
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else if (s == "addEmployee") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        cout << "username: " << endl;
        string username;
        cin >> username;
        cout << "password: " << endl;
        string password;
        cin >> password;
        Employee C(id, username, password);
        manager.addEmployee1(con, C);
        if(Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "deleteEmployee") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE id = " + std::to_string(id));
            if (res->next()) {
                manager.deleteEmployee(con, id);
            }
            else {
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
            if (Manager_functions(cars, customers, employees, manager, con)) return 1;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else if (s == "updateEmployee") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        string username;
        int employee_record=8;
        double fineDue=0;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE id = " + std::to_string(id));
            if (res->next()) {
                username = res->getString("username");
                employee_record = res->getInt("employee_record");
                fineDue = res->getDouble("fineDue");
                cout << "id: " << id << " username: " << username << " employee_record: " << employee_record << " fineDue: " << fineDue << endl;
                int newid;
                string newusername;
                int newemployee_record;
                double newfineDue;
                cout << "new_id: " << endl;
                cin >> newid;
                cout << "new_username: " << endl;
                cin >> newusername;
                cout << "new_employee_record: " << endl;
                cin >> newemployee_record;
                cout << "new_fineDue: " << endl;
                cin >> newfineDue;
                manager.updateEmployee(con, id, newid, newusername, newemployee_record, newfineDue);
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            else {
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if(Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else if (s == "addCar") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        cout << "model: " << endl;
        string model;
        cin >> model;
        cout << "car_condition: " << endl;
        string car_condition;
        cin >> car_condition;
        cout << "rent: " << endl;
        int rent;
        cin >> rent;
        cout << "isAvailable: " << endl;
        bool isAvailable;
        cin >> isAvailable;
        Car D(id, model, car_condition, rent, isAvailable);
        manager.addCar1(con, D);
        if (Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    else if (s == "deleteCar") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE id = " + std::to_string(id));
            if (res->next()) {
                manager.deleteCar(con, id);
            }
            else {
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if (Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
            if (Manager_functions(cars, customers, employees, manager, con)) return 1;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else if (s == "updateCar") {
        cout << "id: " << endl;
        int id;
        cin >> id;
        string model;
        string condition;
        int rent;
        bool isAvailable;
        try {
            Statement* stmt = con->createStatement();
            ResultSet* res = stmt->executeQuery("SELECT * FROM cars WHERE id = " + std::to_string(id));
            if (res->next()) {
                model = res->getString("model");
                condition = res->getString("car_condition");
                rent = res->getInt("rent");
                isAvailable = res->getDouble("isAvailable");
                cout << "id: " << id << " model: " << model << " cat_condition: " << condition << " rent: " << rent << " isAvailable: " << isAvailable << endl;
                int newid;
                string newmodel;
                string newcondition;
                int newrent;
                bool newisAvailable;
                cout << "new_id: " << endl;
                cin >> newid;
                cout << "new_model: " << endl;
                cin >> newmodel;
                cout << "new_condition: " << endl;
                cin >> newcondition;
                cout << "new_rent" << endl;
                cin >> newrent;
                cout << "new_isAvailable" << endl;
                cin >> newisAvailable;
                manager.updateCar(con, id, newid, newmodel, newcondition, newrent, newisAvailable);
                if (Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            else {
                delete res;
                delete stmt;
                cout << "id not found" << endl;
                if (Manager_functions(cars, customers, employees, manager, con)) return 1;
            }
            delete res;
            delete stmt;
        }
        catch (SQLException& e) {
            std::cerr << "SQL Error: " << e.what() << std::endl;
        }
    }
    else {
        cout << "No functions found" << endl;
        if (Manager_functions(cars, customers, employees, manager, con)) return 1;
    }
    return 1;
}
void logging(Connection* con, vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, Manager& manager)
{
    cars = getAllCars(con);
    customers = getAllCustomers(con);
    employees = getAllEmployees(con);
    cout << "-----------------------------------------------------------------------------------------" << endl;
    string username;
    string password;
    std::cout << "Enter username: ";
    cin >> username;
    std::cout << "Enter password: ";
    cin >> password;
    try {
        bool isCustomer = false;
        if (con && !isCustomer) {
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM customers WHERE username = '" + username + "' AND password = '" + password + "'");
                if (res->next()) {
                    // Username-password pair found in customers table
                    isCustomer = true;
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
        }
        else {
            cerr << "Failed to connect to the database." << endl;
        }

        // Query employees table
        bool isEmployee = false;
        if (con && !isEmployee && !isCustomer) {
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM employees WHERE username = '" + username + "' AND password = '" + password + "'");
                if (res->next()) {
                    // Username-password pair found in employees table
                    isEmployee = true;
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
        }

        // Query managers table
        bool isManager = false;
        if (con && !isManager && !isEmployee && !isCustomer) {
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM managers WHERE username = '" + username + "' AND password = '" + password + "'");
                if (res->next()) {
                    // Username-password pair found in managers table
                    isManager = true;
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                cerr << "SQL Error: " << e.what() << endl;
            }
        }

        if (con && !isManager && !isEmployee && !isCustomer) {
            cout << "Invalid username or password" << endl;
            logging(con, cars, customers, employees, manager);
        }

        // Determine user category based on query results
        if (isCustomer) {
            int i = 0;
            while (i < customers.size())
            {
                if (customers[i].getUsername() == username) break;
                i++;
            }
            std::cout << "Hello customer!" << endl;
            std::cout << "ID: " << customers[i].getID() << endl;
            std::cout << "Rented Cars: " << endl;
            int j = 0;
            vector<Car>caro;
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
                int l = 1;
                while (res->next()) {
                    if (customers[i].getID()==res->getInt("user_id"))
                    {
                        cout << "Serial No.: " << l << " " << "| ";
                        int id = res->getInt("id");
                        std::cout << "id: " << id << " " << "| ";
                        string model = res->getString("model");
                        std::cout << "model: " << model << " " << "| ";
                        string car_condition = res->getString("car_condition");
                        std::cout << "car_condition: " << car_condition << " " << "| ";
                        int rent = res->getInt("rent");
                        std::cout << "rent: " << rent << " " << "| ";
                        bool isAvailable = res->getBoolean("isAvailable");
                        std::cout << "isAvailable: " << isAvailable << " | ";
                        int user_id = res->getInt("user_id");
                        std::cout << "user_id: " << user_id << endl;
                        Car cario(id, model, car_condition, rent, isAvailable);
                        cario.setuserid(user_id);
                        caro.push_back(cario);
                        l++;
                    }
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                std::cerr << "SQL Error: " << e.what() << std::endl;
            }
            std::cout << "Customer Record: " << customers[i].getCustomerrecord() << endl;
            std::cout << "Fine Due: " << customers[i].getfineDue() << endl;
            if(Customer_functions(cars, customers, employees, i, caro, con)) logging(con, cars, customers, employees, manager);
        }
        else if (isEmployee) {
            int i = 0;
            while (i < employees.size())
            {
                if (employees[i].getUsername() == username) break;
                i++;
            }
            std::cout << "Hello employee!" << endl;
            std::cout << "ID: " << employees[i].getID() << endl;
            std::cout << "Rented Cars: " << endl;
            int j = 0;
            vector<Car>caro;
            try {
                Statement* stmt = con->createStatement();
                ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
                int l = 0;
                while (res->next()) {
                    if (employees[i].getID() == res->getInt("user_id"))
                    {
                        cout << "Serial No.: " << l << " ";
                        int id = res->getInt("id");
                        std::cout << "id: " << id << " ";
                        string model = res->getString("model");
                        std::cout << "model: " << model << " ";
                        string car_condition = res->getString("car_condition");
                        std::cout << "car_conditioncar: " << car_condition << " ";
                        int rent = res->getInt("rent");
                        std::cout << "rent: " << rent << " ";
                        bool isAvailable = res->getBoolean("isAvailable");
                        std::cout << "isAvailable: " << isAvailable << " ";
                        int user_id = res->getInt("user_id");
                        std::cout << "user_id: " << user_id << endl;
                        Car cario(id, model, car_condition, rent, isAvailable);
                        cario.setuserid(user_id);
                        caro.push_back(cario);
                    }
                    l++;
                }
                delete res;
                delete stmt;
            }
            catch (SQLException& e) {
                std::cerr << "SQL Error: " << e.what() << std::endl;
            }
            std::cout << "Employee Record: " << employees[i].getEmployeerecord() << endl;
            std::cout << "Fine Due: " << employees[i].getfineDue() << endl;
            if (Employee_functions(cars, customers, employees, i, caro, con)) logging(con, cars, customers, employees, manager);
        }
        else if (isManager) {
            std::cout << "Hello manager!" << endl;
            if (Manager_functions(cars, customers, employees, manager, con)) logging(con, cars, customers, employees, manager);
        }
        else {
            std::cout << "Invalid username or password." << endl;
            logging(con, cars, customers, employees, manager);
        }
    }
    catch (SQLException& e) {
        cerr << "SQL Error: " << e.what() << endl;
    }

}
//vector<Car> getAllCars(Connection* con) {
//    vector<Car> cars;
//    try {
//        Statement* stmt = con->createStatement();
//        ResultSet* res = stmt->executeQuery("SELECT * FROM cars");
//        while (res->next()) {
//            int id = res->getInt("id");
//            string model = res->getString("model");
//            string condition = res->getString("car_condition");
//            int rent = res->getInt("rent");
//            bool isAvailable = res->getBoolean("isAvailable");
//            cars.push_back(Car(id, model, condition, rent, isAvailable));
//        }
//        delete res;
//        delete stmt;
//    }
//    catch (SQLException& e) {
//        cerr << "SQL Error: " << e.what() << endl;
//    }
//    return cars;
//}
//vector<Customer> getAllCustomers(Connection* con) {
//    vector<Customer> customers;
//    try {
//        Statement* stmt = con->createStatement();
//        ResultSet* res = stmt->executeQuery("SELECT * FROM customers");
//        while (res->next()) {
//            int id = res->getInt("id");
//            string username = res->getString("username");
//            string password = res->getString("password");
//            customers.push_back(Customer(id, username, password));
//        }
//        delete res;
//        delete stmt;
//    }
//    catch (SQLException& e) {
//        cerr << "SQL Error: " << e.what() << endl;
//    }
//    return customers;
//}
//vector<Employee> getAllEmployees(Connection* con) {
//    vector<Employee> employees;
//    try {
//        Statement* stmt = con->createStatement();
//        ResultSet* res = stmt->executeQuery("SELECT * FROM employees");
//        while (res->next()) {
//            int id = res->getInt("id");
//            string username = res->getString("username");
//            string password = res->getString("password");
//            employees.push_back(Employee(id, username, password));
//        }
//        delete res;
//        delete stmt;
//    }
//    catch (SQLException& e) {
//        cerr << "SQL Error: " << e.what() << endl;
//    }
//    return employees;
//}
//vector<Manager> getManager(Connection* con) {
//    vector<Manager> managers;
//    try {
//        Statement* stmt = con->createStatement();
//        ResultSet* res = stmt->executeQuery("SELECT * FROM managers");
//        while (res->next()) {
//            int id = res->getInt("id");
//            string username = res->getString("username");
//            string password = res->getString("password");
//            managers.push_back(Manager(id, username, password));
//        }
//        delete res;
//        delete stmt;
//    }
//    catch (SQLException& e) {
//        cerr << "SQL Error: " << e.what() << endl;
//    }
//    return managers;
//}

int main() {
    //string database;
    //std::cout << "Enter the database: ";
    //cin >> database;
    DatabaseConnector dbConnector("tcp://localhost:3306", "root", "armk", ""); // Empty string for the database name
    Connection* con = dbConnector.getConnection();
    Manager manager(1, "manager", "password"); // Instantiate the manager

    string username;
    string password;
    static vector<Car> cars;
    static vector<Customer> customers;
    static vector<Employee> employees;
    static vector<Manager> managers;

    if (con) {
        try {
            //// Create tables if not exist
            Statement* stmt = con->createStatement();
            stmt->execute("CREATE TABLE IF NOT EXISTS customers(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL, customer_record INT DEFAULT 8 NOT NULL, fineDue DOUBLE DEFAULT 0 NOT NULL)");
            stmt->execute("CREATE TABLE IF NOT EXISTS employees (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL, employee_record INT DEFAULT 8 NOT NULL, fineDue DOUBLE DEFAULT 0 NOT NULL)");
            stmt->execute("CREATE TABLE IF NOT EXISTS cars (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, model VARCHAR(255) NOT NULL, car_condition VARCHAR(255) NOT NULL, rent INT NOT NULL, isAvailable BOOLEAN NOT NULL, user_id INT DEFAULT 0 NOT NULL, rent_time VARCHAR(255) DEFAULT '01-01-00' NOT NULL, rent_days INT DEFAULT 0 NOT NULL)");
            stmt->execute("CREATE TABLE IF NOT EXISTS managers(id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, username VARCHAR(255) NOT NULL, password VARCHAR(255) NOT NULL)");
            delete stmt;
            //create_tables_if_not_exist(con);
            cars = getAllCars(con);
            customers = getAllCustomers(con);
            employees = getAllEmployees(con);
            managers = getManager(con);

            if (!managers.size())
            {
                int id = 1;
                string username = "manager";
                string password = "password";
                try {
                    Statement* stmt = con->createStatement();
                    ResultSet* res = stmt->executeQuery("SELECT COUNT(*) FROM managers WHERE id = " + to_string(id));
                    res->next();
                    int count = res->getInt(1);
                    delete res; // Clean up the ResultSet object
                    delete stmt; // Clean up the Statement object
                    if (count == 0) {
                        stmt = con->createStatement();
                        stmt->execute("INSERT INTO managers (id, username, password) VALUES (" + to_string(id) + ", '" + username + "', '" + password + "')");
                        delete stmt; // Clean up the Statement object
                        std::cout << "Manager added successfully." << endl;
                    }
                }
                catch (SQLException& e) {
                    cerr << "SQL Error: " << e.what() << endl;
                }
            }

            if (!cars.size()) {
                cars = {
                    Car(123, "Toyota Camry", "Good", 50, true),
                    Car(234, "Honda Civic", "Excellent", 60, true),
                    Car(345, "Ford Mustang", "Fair", 70, true),
                    Car(456, "Chevrolet Corvette", "Excellent", 80, true),
                    Car(567, "BMW 3 Series", "Good", 90, true)
                };
                for (auto& car : cars) {
                    manager.addCar(con, car);
                }
            }

            // Add some sample customers
            if (!customers.size()) {
                customers = {
                    Customer(135, "customer1", "password1"),
                    Customer(246, "customer2", "password2"),
                    Customer(357, "customer3", "password3"),
                    Customer(468, "customer4", "password4"),
                    Customer(579, "customer5", "password5")
                };
                for (auto& customer : customers) {
                    manager.addCustomer(con, customer);
                }
            }

            // Add some sample employees
            if (!employees.size()) {
                employees = {
                    Employee(147, "employee1", "password1"),
                    Employee(258, "employee2", "password2"),
                    Employee(369, "employee3", "password3"),
                    Employee(4710, "employee4", "password4"),
                    Employee(5811, "employee5", "password5")
                };
                for (auto& employee : employees) {
                    manager.addEmployee(con, employee);
                }
            }
            logging(con, cars, customers, employees, manager);
        }
        catch (SQLException& e) {
            cerr << "SQL Error: " << e.what() << endl;
        }
        
        // Close connection
        delete con;
    }
    else {
        cerr << "Failed to connect to the database." << endl;
    }

    return 0;
}

