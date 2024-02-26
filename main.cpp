#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include "DateTime.h"
#include<cassert>
#define AVG_CUSTOMER_RECORD 100
#define AVG_EMPLOYEE_RECORD 200
using namespace std;
#define HEAVY_DAMAGE 0
#define LIGHT_DAMAGE 1
#define MINOR_SCRATCHES 2
#define FINE 3

//Employee IDs begin with 2
//Customer IDs begin with 1

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

        bool verify_password(string pass)
        {
            if(pass == password)
                return 1;
            else 
                return 0;
        }
    
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
        static void addCustomer(vector<Customer>& customers) {
            string name,password,temp;
            int id;

            cout << "Enter Name : " ;
            cin >> name ;
            cout << endl;

            cout << "Enter Password : ";
            cin >> password ;
            cout << endl;

            cout << "Confirm Password : ";
            cin >> temp ;
            cout << endl;

            while( temp != password)
            {
                cout << "Passwords don't match!" << endl;
                cout << "Enter your Password : ";
                cin >> password ;
                cout << endl;

                cout << "Confirm Password : ";
                cin >> temp ;
                cout << endl;
            }
            id = customers.back().id + 1;
            cout << "ID of the new Customer is : " << id << endl;
            customers.push_back(Customer(name,id));
            customers.back().set_password(password);
        }
        static Customer* login(vector<Customer>& customers) {
            string name, password;
            cout << "Enter Username: ";
            cin >> name;
            cout << "Enter Password: ";
            cin >> password;

            // Search for the customer with the given name
            auto it = find_if(customers.begin(), customers.end(), [&name](const Customer& customer) {
                return customer.name == name;
            });

            // If customer found, check password
            if (it != customers.end()) {
                if (it->password == password) {
                    cout << "Login Successful!" << endl;
                    return &(*it);
                } else {
                    cout << "Incorrect Password!" << endl;
                    return nullptr;
                }
            } else {
                cout << "Customer not found!" << endl;
                return nullptr;
            }
        }
        // Function to update an existing Customer in the database
        static void updateCustomer(vector<Customer>& Customers) {
            int id;
            cout << "Enter the id of the Customer to be modified : " ;
            cin >> id;

            auto it = std::lower_bound(Customers.begin(), Customers.end(), id, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });
            if (it != Customers.end()) {
                //modify here
                // *it = updatedCustomer;
            } else {
                cout << "Customer with ID " << id << " not found." << endl;
            }
        }

        // Function to delete a Customer from the database
        static void deleteCustomer(vector<Customer>& Customers) {

            int id;
            cout << "Enter the id of the Customer to be deleted : " ;
            cin >> id;
            auto it = std::lower_bound(Customers.begin(), Customers.end(), id, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });
            if (it != Customers.end()) {
                Customers.erase(it);
            } else {
                cout << "Customer with ID " << id << " not found." << endl;
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
        Employee(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_EMPLOYEE_RECORD) : User(name, id, password), fine_due(fine_due), record(record) {}

        void set_password(string pass)
        {
            password = pass;
        }

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

        static void addEmployee(vector<Employee>& employees) {
            string name,password,temp;
            int id;

            cout << "Enter Name : " ;
            cin >> name ;
            cout << endl;

            cout << "Enter Password : ";
            cin >> password ;
            cout << endl;

            cout << "Confirm Password : ";
            cin >> temp ;
            cout << endl;

            while( temp != password)
            {
                cout << "Passwords don't match!" << endl;
                cout << "Enter your Password : ";
                cin >> password ;
                cout << endl;

                cout << "Confirm Password : ";
                cin >> temp ;
                cout << endl;
            }
            id = employees.back().id + 1;
            cout << "ID of the new Employee is : " << id << endl;
            employees.push_back(Employee(name,id));
            employees.back().set_password(password);
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

        static Employee* login(vector<Employee>& Employees) {
            string password;
            int id;
            
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;

            // Search for the Employee with the given name
            auto it = std :: lower_bound(Employees.begin(), Employees.end(), id,[](const Employee& Employee, int id) {
                return Employee.id < id;
            });

            // If Employee found, check password
            if (it != Employees.end()) {
                if (it->password == password) {
                    cout << "Login Successful!" << endl;
                    return &(*it);
                } else {
                    cout << "Incorrect Password!" << endl;
                    return nullptr;
                }
            } else {
                cout << "Employee not found!" << endl;
                return nullptr;
            }
        }

        // Function to update an existing Employee in the database
        static void updateEmployee(vector<Employee>& Employees) {
            
            int id;
            cout << "Enter the id of the Customer to be modified : " ;
            cin >> id;

            auto it = std::lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
                return Employee.id < id;
            });

            if (it != Employees.end()) {
                //modify the employee here
                // *it = updatedEmployee;
            } else {
                cout << "Employee with ID " << id << " not found." << endl;
            }
        }

        // Function to delete a Employee from the database
        static void deleteEmployee(vector<Employee>& Employees) {

            int id;
            cout << "Enter the id of the Customer to be deleted : " ;
            cin >> id;

            auto it = std::lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
                return Employee.id < id;
            });
            if (it != Employees.end()) {
                Employees.erase(it);
            } else {
                cout << "Employee with ID " << id << " not found." << endl;
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
        void set_password( string pass) {password = pass;} 
        Manager(const string& name = "", int id = 0, const string& password = "") : User(name, id, password){}

        void show_user_details(vector<Customer>& customers, vector<Employee>& employees)
        {
            if (id/100000 == 1)
                Customer :: show_customers(customers);
            else
                Employee :: show_employees(employees);
        }

        static void addManager(vector<Manager>& Managers) {
            string name,password,temp;
            int id;

            cout << "Enter Name : " ;
            cin >> name ;
            cout << endl;

            cout << "Enter Password : ";
            cin >> password ;
            cout << endl;

            cout << "Confirm Password : ";
            cin >> temp ;
            cout << endl;

            while( temp != password)
            {
                cout << "Passwords don't match!" << endl;
                cout << "Enter your Password : ";
                cin >> password ;
                cout << endl;

                cout << "Confirm Password : ";
                cin >> temp ;
                cout << endl;
            }
            id = Managers.back().id + 1;
            cout << "ID of the new Manager is : " << id << endl;
            Managers.push_back(Manager(name,id));
            Managers.back().set_password(password);
        }

        // Function to search for a Manager by ID
        static Manager* searchManagerById(vector<Manager>& Managers, int ManagerId) {
            auto it = std::lower_bound(Managers.begin(), Managers.end(), ManagerId, [](const Manager& Manager, int id) {
                return Manager.id < id;
            });

            if (it != Managers.end() && it->id == ManagerId) {
                return &(*it); 
            } else {
                return nullptr; 
            }
        }

        static Manager* login(vector<Manager>& Managers) {
            string password;
            int id;
            
            cout << "Enter ID: ";
            cin >> id;
            cout << "Enter Password: ";
            cin >> password;

            // Search for the Manager with the given name
            auto it = std :: lower_bound(Managers.begin(), Managers.end(), id,[](const Manager& Manager, int id) {
                return Manager.id < id;
            });

            // If Manager found, check password
            if (it != Managers.end()) {
                if (it->password == password) {
                    cout << "Login Successful!" << endl;
                    return &(*it);
                } else {
                    cout << "Incorrect Password!" << endl;
                    return nullptr;
                }
            } else {
                cout << "Manager not found!" << endl;
                return nullptr;
            }
        }

        // Function to update an existing Manager in the database
        static void updateManager(vector<Manager>& Managers) {
            
            int id;
            cout << "Enter the id of the Customer to be modified : " ;
            cin >> id;

            auto it = std::lower_bound(Managers.begin(), Managers.end(), id, [](const Manager& Manager, int id) {
                return Manager.id < id;
            });

            if (it != Managers.end()) {
                //modify the Manager here
                // *it = updatedManager;
            } else {
                cout << "Manager with ID " << id << " not found." << endl;
            }
        }

        // Function to delete a Manager from the database
        static void deleteManager(vector<Manager>& Managers) {

            int id;
            cout << "Enter the id of the Customer to be deleted : " ;
            cin >> id;

            auto it = std::lower_bound(Managers.begin(), Managers.end(), id, [](const Manager& Manager, int id) {
                return Manager.id < id;
            });
            if (it != Managers.end()) {
                Managers.erase(it);
            } else {
                cout << "Manager with ID " << id << " not found." << endl;
            }
        }


        static void show_Managers(vector<Manager>& Managers)
        {
            for(auto& it : Managers)
                it.show();
        }

        void modify_records(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
        {
            cout << "Which database is to be modified?\n1 - Customers\n2 - Employees\n3 - Cars" << endl;
            int k,j;

            cin >> k;

            string name,password;
            switch(k){
                case 1 : 
                    cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
                    cin >> j;

                    switch (j)
                    {
                    case 1:
                        Customer :: addCustomer(customers);
                        break;
                    case 2:
                        Customer :: updateCustomer(customers);
                        break;
                    case 3:
                        Customer :: deleteCustomer(customers);
                        break;
                    default:
                        cout << "Incorrect Option !" << endl;
                        modify_records(customers,cars,employees);

                        break;
                    }
                    break;
                case 2 : 
                    cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
                    cin >> j;

                    switch (j)
                    {
                    case 1:
                        Employee :: addEmployee(employees);
                        break;
                    case 2:
                        Employee :: updateEmployee(employees);
                        break;
                    case 3:
                        Employee :: deleteEmployee(employees);
                        break;
                    default:
                        cout << "Incorrect Option !" << endl;
                        modify_records(customers,cars,employees);

                        break;
                    }
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
    vector<Manager> managers;

    // loadFromFile(cars, "cars.txt");
    // loadFromFile(customers, "customers.txt")
    // loadFromFile(employees, "employees.txt")

    // Car :: addCar(cars,Car(100001,82,2,DateTime(2004,02,05)));
    // Car :: addCar(cars,Car(100003,77,3,DateTime(2005,6,21)));
    // Car :: addCar(cars,Car(100002,82,1,DateTime(2007,1,12)));

    // saveToFile(cars,"cars.txt");

    // loadFromFile(cars,"cars.txt");

    // for(auto& it : cars){
    //     cout << it.id << " " << it.model << " " << it.condition << " The Due date is ";
    //     it.due_date.display() ;
    // }
    
    // auto ptr = Car :: searchCarById(cars, 100007);
    // cout << ptr->id << " " << ptr->model << " " << ptr->condition << " The Due date is ";
    // ptr->due_date.display();

    int k;
    cout << "Welcome to Car Rental System" << endl ;
    cout << "----------------------------" << endl ;

    cout << "New user or previous user ?" <<  endl;
    cout << "0 - New User \n1 - Previous User\n" << endl;

    /// Driver Code
    cin >> k;

    int j;
    switch(k)
    {
        //registering a new customer
        case 0 : 
            Customer :: addCustomer(customers);
            break;
        //previous customer/employee/manager
        case 1 :
            cout << "Enter the type of user :\n1 - Customer\n2 - Employee\n3 - Manager" << endl;
            cin >> j;
            switch(j)
            {
                case 1: 
                    auto it = Customer :: login(customers);
                    break;
                case 2:
                    auto it = Employee :: login(employees);
                    break;
                case 3:
                    auto it = Manager :: login(managers);
                    break;
            }
    }
    return 0;
}