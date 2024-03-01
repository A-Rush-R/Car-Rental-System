#include<iostream>
#include<vector>
#include "dateTime.h"
#include "utils.h"
#include "Car.h"
#include "user.h"
using namespace std;

void load(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, vector<Manager>& managers)
{
    Car :: loadFromFile(cars,"cars.txt");
    Customer :: loadFromFile(customers,"customers.txt");
    Employee :: loadFromFile(employees,"employees.txt");
    Manager :: loadFromFile(managers,"managers.txt");
}

void save(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, vector<Manager>& managers)
{
    Car :: saveToFile(cars,"cars.txt");
    Customer :: saveToFile(customers,"customers.txt");
    Employee :: saveToFile(employees,"employees.txt");
    Manager :: saveToFile(managers,"managers.txt");
}

int main()
{  
    // Databases
    vector<Car> cars;
    vector<Customer> customers;
    vector<Employee> employees;
    vector<Manager> managers;
    
    load(cars,customers,employees,managers);

    // Customer :: show_customers(customers);
    // Employee :: show_employees(employees);
    // Manager :: show_managers(managers);
    // Car :: showcars(cars,400001);

    // for(auto& it : cars)
    // {
    //     it.repair();
    // }


    int k;
    cout << "----------------------------" << endl ;
    cout << "Welcome to Car Rental System" << endl ;
    cout << "----------------------------" << endl ;

    cout << "New user or previous user ?" <<  endl;
    cout << "1 - New User \n2 - Previous User\nAny other key - Exit" << endl;

    /// Driver Code
    cin >> k;

    int j;
	Customer* it1 ;
	Employee* it2 ;
	Manager* it3 ;
	it1 = nullptr;
	it2 = nullptr;
	it3 = nullptr;
    switch(k)
    {
        //registering a new customer
        case 1 : 
            Customer :: addCustomer(customers);
            cout << "Customer registered successfully, run again to login" << endl;
            break;
        //previous customer/employee/manager
        case 2 :
            cout << "Enter the type of user :\n1 - Customer\n2 - Employee\n3 - Manager\nAny other key - Exit" << endl;
            cin >> j;
            switch(j)
            {
                case 1: 
					while(it1 == nullptr)
                    	it1 = Customer :: login(customers);	
                    it1->begin_session(cars);

                    break;
                case 2:
					while(it2 == nullptr)
                    	it2 = Employee :: login(employees);
                    it2->begin_session(cars);
                    break;
                case 3:
					while(it3 == nullptr)
                    	it3 = Manager :: login(managers);
                    it3->begin_session(customers,cars,employees);
                    break;
                default:
                    cout << "Thank You !" << endl;
            }
			break;
		default :
			cout << "Thank You !" << endl;
    }
    
    save(cars,customers,employees,managers);

    return 0;
}