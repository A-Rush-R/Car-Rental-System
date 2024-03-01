#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include<vector>
#include <algorithm>
#include "Car.h"
using namespace std;
#define CUSTOMER_BEGIN_ID 200001

class User {
protected:
    std::string password;

public:
    std::string name;
    int id;

    User(std::string name, int id, std::string password);
    virtual void show() = 0;
};

class Customer : public User 
{
        int fine_due;
        vector<int> rented_cars;
        int record ;
    public : 

        void set_password(string pass);
        Customer(const string& name, int id, const string& password, int fine_due, int record,const vector<int>& rented_cars);
		int show_due();
        void clear_due();
        void show();
        void update_record(int late_duration,int condition);
        static void addCustomer(vector<Customer>& customers);
        static Customer* login(vector<Customer>& customers);
        static void updateCustomer(vector<Customer>& Customers);
        static void deleteCustomer(vector<Customer>& Customers);
        static void show_customers(vector<Customer>& customers);
        static Customer* searchCustomerById(vector<Customer>& Customers, int CustomerId);
        void rent_request(vector<Car>& cars);
        void return_request(vector<Car>& cars);
        void begin_session(vector<Car>& cars);
        static void saveToFile(const vector<Customer>& customers, const string& filename);
        static void loadFromFile(vector<Customer>& customers, const string& filename);
};
class Employee : public User 
{
        int fine_due;
        vector<int> rented_cars;
        int record ;
    public : 

        void set_password(string pass);
        Employee(const string& name, int id, const string& password, int fine_due, int record,const vector<int>& rented_cars);
		int show_due();
        void clear_due();
        void show();
        void update_record(int late_duration,int condition);
        static void addEmployee(vector<Employee>& customers);
        static Employee* login(vector<Employee>& customers);
        static void updateEmployee(vector<Employee>& employees);
        static void deleteEmployee(vector<Employee>& employees);
        static void show_employees(vector<Employee>& employees);
        static Employee* searchEmployeeById(vector<Employee>& employees, int EmployeeId);
        void rent_request(vector<Car>& cars);
        void return_request(vector<Car>& cars);
        void begin_session(vector<Car>& cars);
        static void saveToFile(const vector<Employee>& employees, const string& filename);
        static void loadFromFile(vector<Employee>& employees, const string& filename);
};

class Manager : public User 
{
    public : 
        void set_password( string pass);
        Manager(const string& name, int id, const string& password);
        void show_records(vector<Customer>& customers, vector<Employee>& employees, vector<Car>& cars);
        static void addManager(vector<Manager>& Managers);
        static Manager* searchManagerById(vector<Manager>& Managers, int ManagerId);
        static Manager* login(vector<Manager>& Managers);
        static void updateManager(vector<Manager>& Managers);
        static void deleteManager(vector<Manager>& Managers);
        static void show_managers(vector<Manager>& Managers);
        void modify_records(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees);
        void show();
        void begin_session(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees);
        static void saveToFile(const vector<Manager>& managers, const string& filename);
        static void loadFromFile(vector<Manager>& managers, const string& filename);
};

#endif // USER_H
