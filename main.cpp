#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include "DateTime.h"
#include<cassert>
#define AVG_CUSTOMER_RECORD 100
using namespace std;
#define HEAVY_DAMAGE 0
#define LIGHT_DAMAGE 1
#define MINOR_SCRATCHES 2
#define FINE 3

// class Car
// {
//     private :
//         DateTime due_date;
//     public :
//         int id;
//         int model;
//         int condition;

//         int rent_request()
//         {
//             //code here
//             return 0;
//         }

//         DateTime show_due_date(){ return due_date; }

// };

class Car {

public:
    DateTime due_date;
    int id;
    int model;
    int condition;
    int ownerID;

    Car(int id =0, int model = 0, int condition = 0,DateTime due_date = DateTime(0,0,0), int ownerID = 0)
    {
        this->id = id;
        this->model = model;
        this->condition = condition;
        this->due_date = due_date; 
        this->ownerID = ownerID;
    }

    void show() const
    {
        string status = ownerID == 0 ? "Available" : "Rented"; 
        cout << "ID   Model    Condition    Status" << endl;
        string condition_name;

        assert(condition <= FINE && condition >= HEAVY_DAMAGE);

        switch(condition) {
            case HEAVY_DAMAGE:
                condition_name = "Heavy Damage";
                break;
            case LIGHT_DAMAGE:
                condition_name = "Light Damage";
                break;
            case MINOR_SCRATCHES:
                condition_name = "Minor Scratches";
                break;
            case FINE:
                condition_name = "Fine";
                break;
        }
        cout << id << " " << model << " " << condition_name << " " << status << " ";
        due_date.display();
    }

    int rent_request() {
        // code here
        return 0;
    }


    DateTime show_due_date() { return due_date; }

    // Function to add a new car to the database
    static void addCar(vector<Car>& cars, const Car& newCar) {
        cars.push_back(newCar);
    }

    // Function to update an existing car in the database
    static void updateCar(vector<Car>& cars, int carId, const Car& updatedCar) {
        auto it = std::lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
            return car.id < id;
        });
        if (it != cars.end()) {
            *it = updatedCar;
        } else {
            cout << "Car with ID " << carId << " not found." << endl;
        }
    }

    // Function to delete a car from the database
    static void deleteCar(vector<Car>& cars, int carId) {
        auto it = std::lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
            return car.id < id;
        });
        if (it != cars.end()) {
            cars.erase(it);
        } else {
            cout << "Car with ID " << carId << " not found." << endl;
        }
    }

    // Function to search for a car by ID
    static Car* searchCarById(vector<Car>& cars, int carId) {
        auto it = std::lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
            return car.id < id;
        });

        if (it != cars.end() && it->id == carId) {
            return &(*it); 
        } else {
            return nullptr; 
        }
    }

    static void showcars(vector<Car>& cars, int userID = 0)
    {
        if(userID == 0)
            for(auto& it : cars)
                it.show();
        else
            for(auto& it : cars)
            {
                if(it.ownerID == userID || it.ownerID == 0)
                    it.show();
            }
    }
};

class User
{
    protected :
        string password;
    public :
        string name;
        int id;
        User(string name, int id, string password)
        {
            this->name = name;
            this->id = id;
            this->password = password;
        }
        virtual void show() = 0; //virtual function
    
};

class Customer : public User 
{
        vector<Car> rented_cars;
        int fine_due;
        int record ;

    public : 
        void set_password(string pass) {password = pass;}
        Customer(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_CUSTOMER_RECORD) : User(name, id, password), fine_due(fine_due), record(record) {}

        void clear_due()
        {
            fine_due = 0;
        }
        void show()
        {
            cout << id << " " << name << " " << fine_due << " " << record << endl;
            cout << "List of Rented Car is ";
            Car :: showcars(rented_cars);
        }

        // Function to add a new Customer to the database
        static void addCustomer(vector<Customer>& Customers, const Customer& newCustomer) {
            Customers.push_back(newCustomer);
        }

        // Function to update an existing Customer in the database
        static void updateCustomer(vector<Customer>& Customers, int CustomerId, const Customer& updatedCustomer) {
            auto it = std::lower_bound(Customers.begin(), Customers.end(), CustomerId, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });
            if (it != Customers.end()) {
                *it = updatedCustomer;
            } else {
                cout << "Customer with ID " << CustomerId << " not found." << endl;
            }
        }

        // Function to delete a Customer from the database
        static void deleteCustomer(vector<Customer>& Customers, int CustomerId) {
            auto it = std::lower_bound(Customers.begin(), Customers.end(), CustomerId, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });
            if (it != Customers.end()) {
                Customers.erase(it);
            } else {
                cout << "Customer with ID " << CustomerId << " not found." << endl;
            }
        }

        // Function to search for a Customer by ID
        static Customer* searchCustomerById(vector<Customer>& Customers, int CustomerId) {
            auto it = std::lower_bound(Customers.begin(), Customers.end(), CustomerId, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });

            if (it != Customers.end() && it->id == CustomerId) {
                return &(*it); 
            } else {
                return nullptr; 
            }
        }
        static void show_customers(vector<Customer>& customers) 
        {
            for (auto& it : customers)
                it.show();
        }
        
};

class Employee : public User 
{
        vector<Car> rented_cars;
        int fine_due;
        int record ;

    public : 

        void clear_due()
        {
            fine_due = 0;
        }        

        void show() 
        {
            cout << id << " " << name << " " << fine_due << " " << record << endl;
            cout << "List of Rented Cars is ";
            Car :: showcars(rented_cars);
        }

        static void addEmployee(vector<Employee>& Employees, const Employee& newEmployee) {
            Employees.push_back(newEmployee);
        }

        // Function to update an existing Employee in the database
        static void updateEmployee(vector<Employee>& Employees, int EmployeeId, const Employee& updatedEmployee) {
            auto it = std::lower_bound(Employees.begin(), Employees.end(), EmployeeId, [](const Employee& Employee, int id) {
                return Employee.id < id;
            });

            if (it != Employees.end()) {
                *it = updatedEmployee;
            } else {
                cout << "Employee with ID " << EmployeeId << " not found." << endl;
            }
        }

        // Function to delete a Employee from the database
        static void deleteEmployee(vector<Employee>& Employees, int EmployeeId) {
            auto it = std::lower_bound(Employees.begin(), Employees.end(), EmployeeId, [](const Employee& Employee, int id) {
                return Employee.id < id;
            });
            if (it != Employees.end()) {
                Employees.erase(it);
            } else {
                cout << "Employee with ID " << EmployeeId << " not found." << endl;
            }
        }

        // Function to search for a Employee by ID
        static Employee* searchEmployeeById(vector<Employee>& Employees, int EmployeeId) {
            auto it = std::lower_bound(Employees.begin(), Employees.end(), EmployeeId, [](const Employee& Employee, int id) {
                return Employee.id < id;
            });

            if (it != Employees.end() && it->id == EmployeeId) {
                return &(*it); 
            } else {
                return nullptr; 
            }
        }

        static void show_employees(vector<Employee>& Employees)
        {
            for(auto& it : Employees)
                it.show();
        }
};

class Manager : public User 
{
    public : 
        void modify_records(vector<Customer>& customers, vector<Car>* cars, vector<Employee>& employees)
        {
            cout << "Which database is to be modified?\n1 - Customers\n2 - Employees\n3 - Cars" << endl;
            int k,j;

            cin >> k;

            string name,password;
            switch(k){
                case 1 : 
                    cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
                    cin >> j;
                    cout << "Choose the id you want to modify/add" << endl;
                    cin >> id;

                    switch (j)
                    {
                    case 1:
                        cout << "Enter the details\n Name of the customer : " ;
                        cin >> name;
                        cout << "Enter the password : ";
                        cin >> password;

                        Customer :: addCustomer(customers, Customer(name,id));
                        customers.back().set_password(password);
                        break;
                    case 2:
                        cout << "Enter the details\n Name of the customer : " ;
                        cin >> name;
                        cout << "Enter the password : ";
                        cin >> password;

                        Customer :: addCustomer(customers, Customer(name,id));
                        customers.back().set_password(password);
                        break;
                    case 3:
                        /* code */
                        break;
                    default:
                        break;
                    }
                    break;
                case 2 : 
                    break ;
                case 3 : 
                    break;
                default : cout << "Please enter a valid number!" << endl;
                    modify_records(customers,cars,employees);
                    break;
            }
        }
        void show()
        {
            cout << id << " " << name << endl;
        }
};

void saveToFile(const vector<Car>& cars, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& car : cars) {
            outFile << car.id << " " << car.model << " " << car.condition << " " << car.due_date.getYear() << " " << car.due_date.getMonth() << " " << car.due_date.getDay() << endl;
            // cout << car.id << " " << car.model << " " << car.condition << endl;
        }
        outFile.close();
        cout << "Records saved to " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

void loadFromFile(vector<Car>& cars, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        cars.clear(); // Clear existing data
        int id, model, condition;
        int y,m,d;

        while (inFile >> id >> model >> condition >> y >> m >> d) {
            DateTime due_date(y,m,d);
            Car car(id,model,condition,due_date);
            cars.push_back(car);
        }
        inFile.close();
        cout << "Records loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

int main()
{  
    // Databases
    vector<Car> cars;
    vector<Customer> customers;
    vector<Employee> employees;

    // loadFromFile(cars, "cars.txt");
    // loadFromFile(customers, "customers.txt")
    // loadFromFile(employees, "employees.txt")

    int k;
    cout << "Welcome to Car Rental System" << endl ;
    cout << "----------------------------" << endl ;

    cout << "New user or previous user ?" <<  endl;
    cout << "0 - New User \n1 - Previous User\n" << endl;

    // Car :: addCar(cars,Car(100001,82,2,DateTime(2004,02,05)));
    // Car :: addCar(cars,Car(100003,77,3,DateTime(2005,6,21)));
    // Car :: addCar(cars,Car(100002,82,1,DateTime(2007,1,12)));

    // saveToFile(cars,"cars.txt");

    loadFromFile(cars,"cars.txt");

    // for(auto& it : cars){
    //     cout << it.id << " " << it.model << " " << it.condition << " The Due date is ";
    //     it.due_date.display() ;
    // }
    
    auto ptr = Car :: searchCarById(cars, 100007);
    cout << ptr->id << " " << ptr->model << " " << ptr->condition << " The Due date is ";
    ptr->due_date.display();

    /// Driver Code
    // cin >> k;
    // string name,password,temp;
    // int id;

    // switch(k)
    // {
    //     //registering a new customer
    //     case 0 : 
    //         cout << "Enter your Name : " ;
    //         cin >> name ;
    //         cout << endl;

    //         cout << "Enter your Password : ";
    //         cin >> password ;
    //         cout << endl;

    //         cout << "Confirm Password : ";
    //         cin >> temp ;
    //         cout << endl;

    //         while( temp != password)
    //         {
    //             cout << "Passwords don't match!" << endl;
    //             cout << "Enter your Password : ";
    //             cin >> password ;
    //             cout << endl;

    //             cout << "Confirm Password : ";
    //             cin >> temp ;
    //             cout << endl;
    //         }

    //         customers.push_back(Customer(name,id));
    //         customers.back().set_password(password);
    //         break;
    //     //previous customer/employee/manager
    //     case 1 :
    //         cout << "Enter your Name : " ;
    //         cin >> name ;
    //         cout << endl;

    //         cout << "Enter your Password : ";
    //         cin >> password ;
    //         cout << endl;
    //         break;
    // }


    //         Car Rental
    return 0;
}