#include<iostream>
#include<sstream>  
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include "DateTime.h"
#include<cassert>
#include<regex>
#define PENALTY_CUSTOMER 200
#define HEAVY_DAMAGE 1
#define LIGHT_DAMAGE 2
#define MINOR_SCRATCHES 3
#define FINE 4
#define CUSTOMER_BEGIN_ID 200001
#define EMPLOYEE_BEGIN_ID 300001
#define MANAGER_BEGIN_ID 400001
#define CAR_BEGIN_ID 100001
int AVG_CUSTOMER_RECORD = 100;
int AVG_EMPLOYEE_RECORD=  200;
using namespace std;

//Employee IDs begin with 2
//Customer IDs begin with 1

int operator-(const DateTime& date1, const DateTime& date2) {
    int days1 = date1.getYear() * 365 + (date1.getYear() / 4 - date1.getYear() / 100 + date1.getYear() / 400);
    for (int m = 1; m < date1.getMonth(); ++m) {
        days1 += DateTime :: daysInMonth(date1.getYear(), m);
    }
    days1 += date1.getDay();

    int days2 = date2.getYear() * 365 + (date2.getYear() / 4 - date2.getYear() / 100 + date2.getYear() / 400);
    for (int m = 1; m < date2.getMonth(); ++m) {
        days2 += DateTime :: daysInMonth(date2.getYear(), m);
    }
    days2 += date2.getDay();

    return days1 - days2;
}

bool parse_date(string date, int* d,int* m,int* y)
{
    regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
    char delimiter;
    istringstream iss(date);
    if(regex_match(date,pattern))
    {
        iss >> *d >> delimiter >> *m >> delimiter >> *y;
        return 1;
    }
    else 
        return 0;
}

class Car {

    public:
        DateTime due_date;
        int id;
        string model;
        int condition;
        int ownerID;

        Car(string model = 0, int id = 0, int condition = 0,DateTime due_date = DateTime(0,0,0), int ownerID = 0) : id(id), model(model), condition(condition), due_date(due_date), ownerID(ownerID){}

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
            cout << id << " " << model << " " << condition_name << " " << status << " " << ownerID << " ";
            due_date.display();
        }

        int rent_request(int id,int y,int m,int d) {
            ownerID = id;
            due_date = DateTime(y,m,d);
            return 0;
        }


        DateTime show_due_date() { return due_date; }

        // Function to add a new car to the database
        static void addCar(vector<Car>& cars) {
            int id,condition;
            string model;
            cout << "Enter the model of the Car to be Added : " ;
            cin >> model;

            if(!cars.empty())
                cars.push_back(Car(model, id = cars.back().id+1));
            else 
                cars.push_back(Car(model, id = CAR_BEGIN_ID));

            cout << "The ID of the added car is " << id << endl;
        }

        // Function to update an existing car in the database
        static void updateCar(vector<Car>& cars) {

            int carId;
            cout << "Enter the ID of the car to be modified : " ;
            cin >> carId;

            int k;

            auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
                return car.id < id;
            });
            if (it != cars.end()) {
                
                cout << "Choose the entry to modify\n1 - Condition\n2 - Due-Date" << endl;
                cin >> k;
                string temp;
                int d,m,y;
                regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
                switch(k){
                    case 1:
                        cout << "Choose an option\n1 - Heavy Damaged\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
                        cin >> k;
                        it->condition = k;                
                        break;
                    case 2:
                        cout << "Enter the due-date of the Car (in DD-MM-YYYY) format : " ;
                        cin >> temp;
                    if (!parse_date(temp,&d,&m,&y))
                    {
                        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                        updateCar(cars);
                    }
                    it->due_date = DateTime(y,m,d);

                        break;
                    default:
                        cout << "Please enter a valid number!" << endl;
                        updateCar(cars);
                        break;
                }
                // *it = updatedCar;

            } else {
                cout << "Car with ID " << carId << " not found." << endl;
            }
        }

        // Function to delete a car from the database
        static void deleteCar(vector<Car>& cars) {

            int carId;
            cout << "Enter the ID of the car to be deleted : " ;
            cin >> carId;

            auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
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
            auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
                return car.id < id;
            });

            if (it != cars.end() && it->id == carId) {
                return &(*it); 
            } else {
                return nullptr; 
            }
        }

        static void showcars(vector<Car>& cars, int userID)
        {
            if(userID / 100000 == 4) //for manager
                for(auto& it : cars){
                    it.show();
                }
            else
                for(auto& it : cars)
                {
                    if(it.ownerID == userID || it.ownerID == 0)
                        it.show();
                }
        }
        static void saveToFile(const vector<Car>& cars, const string& filename) {
            ofstream outFile(filename);
            if (outFile.is_open()) {
                for (const auto& car : cars) {
                    outFile << car.id << " " << car.model << " " << car.condition << " " << car.ownerID << " " << car.due_date.getYear() << " " << car.due_date.getMonth() << " " << car.due_date.getDay() << endl;
                    // cout << car.id << " " << car.model << " " << car.condition << endl;
                }
                outFile.close();
                cout << "Records saved to " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }

        static void loadFromFile(vector<Car>& cars, const string& filename) {
            ifstream inFile(filename);
            if (inFile.is_open()) {
                cars.clear(); // Clear existing data
                int id, condition, ownerID;
                string model;
                int y,m,d;

                while (inFile >> id >> model >> condition >> ownerID >> y >> m >> d) {
                    cars.push_back(Car(model,id,condition,DateTime(y,m,d),ownerID));
                }
                inFile.close();
                cout << "Records loaded from " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
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

        virtual void show() = 0; 
};

class Customer : public User 
{
        int fine_due;

    public : 
        vector<int> rented_cars;
        int record ;
        void set_password(string pass) {password = pass;}
        Customer(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_CUSTOMER_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}

        void clear_due()
        {
            fine_due = 0;
        }
        void show()
        {
            cout << id << " " << name << " " << fine_due << " " << record << endl;
            cout << "List of Rented Car is ";
            for (auto& it : rented_cars)
                cout << it << "\t" ;
            cout << endl;
            // Car :: showcars(cars,rented_cars);
        }

        // Function to add a new Customer to the database
        static void addCustomer(vector<Customer>& customers) {
            string name,password,temp;
            int id;

            cout << "Enter Name (no spaces): " ;
            cin >> name ;
            cout << endl;

            auto it = find_if(customers.begin(), customers.end(), [&name](const Customer& customer) {
                return customer.name == name;
            });

            if(it != customers.end())
            {
                cout << "The User already exists!" << endl;
                return ;
            }

            cout << "Enter Password : ";
            cin >> password ;
            cout << endl;

            cout << "Confirm Password : ";
            cin >> temp ;
            cout << endl;

            while( temp != password)
            {
                cout << "Passwords don't match!" << endl;
                cout << "Enter Password : ";
                cin >> password ;
                cout << endl;

                cout << "Confirm Password : ";
                cin >> temp ;
                cout << endl;
            }

            if(!customers.empty())
                id = customers.back().id + 1;
            else
                id = CUSTOMER_BEGIN_ID;
            // cout << "DEBUGGING" << endl;

            cout << "ID of the new Customer is : " << id << endl;
            customers.push_back(Customer(name,id,password));
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
                    cout << "-----------------" << endl;
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

            auto it = lower_bound(Customers.begin(), Customers.end(), id, [](const Customer& Customer, int id) {
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
            auto it = lower_bound(Customers.begin(), Customers.end(), id, [](const Customer& Customer, int id) {
                return Customer.id < id;
            });
            // auto it = searchCustomerById(customers,id);
            if (it != Customers.end()) {
                Customers.erase(it);
            } else {
                cout << "Customer with ID " << id << " not found." << endl;
            }
        }

        // Function to search for a Customer by ID
        static Customer* searchCustomerById(vector<Customer>& Customers, int CustomerId) {
            auto it = lower_bound(Customers.begin(), Customers.end(), CustomerId, [](const Customer& Customer, int id) {
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

        void rent_request(vector<Car>& cars)
        {
            int carID;
            string date;
            int d,m,y;

            cout << "Enter the id of the car to be rented : " ;
            cin >> carID;
            cout << "Enter the date of rental in DD-MM-YYYY format : ";
            cin >> date;
            if (!parse_date(date,&d,&m,&y))
            {
                cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                return;
            }

            auto carIt = Car :: searchCarById(cars,carID);

            if(carIt->ownerID == 0){
                rented_cars.push_back(carID);
                carIt->rent_request(id,y,m,d);
            }
            else
                cout << "This car is not available for rental" << endl;
        }
        void begin_session(vector<Car>& cars)
        {
            cout << "Welcome " << name << endl;
            cout << "-----------------" << endl;
            int k;
            cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Exit" << endl;
            cin >> k;

            char delimiter;

            string date;
            DateTime dt1,dt2;
            int diff;

            Car* carIt;
            int d,m,y;
            int carID;
            switch(k)
            {
                case 1 :
                    Car :: showcars(cars,id);
                    break;
                case 2 :
                    rent_request(cars);
                    break;

                case 3 :
                    cout << "Enter the id of the car to be returned : " ;
                    cin >> carID;
                    cout << "Enter the date of return in DD-MM-YYYY format : ";
                    cin >> date;


                    // Check if the input matches the pattern
                    if (!parse_date(date,&d,&m,&y))
                    {
                        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                        begin_session(cars);
                    }

                    carIt = Car :: searchCarById(cars,carID);
                    carIt->ownerID = 0;
                    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());

                    diff = DateTime(y,m,d) - carIt->due_date;
                    fine_due += diff*PENALTY_CUSTOMER;
                    break;
                case 4 :
                    return;
                    break;
                default :
                    cout << "Enter a valid option" << endl;
                    break;
            }
            begin_session(cars);
        }

        static void saveToFile(const vector<Customer>& customers, const string& filename) {
            ofstream outFile(filename);
            if (outFile.is_open()) {
                for (const auto& customer : customers) {
                    outFile << customer.id << " " << customer.name << " " << customer.password  << " " << customer.fine_due << " " << customer.record ;
                    // cout << Customer.id << " " << Customer.model << " " << Customer.condition << endl;

                    outFile << " " << customer.rented_cars.size(); 
                    for (int carID : customer.rented_cars) {
                        outFile << " " << carID; 
                    }
                    outFile << endl;

                }
                outFile.close();
                cout << "Records saved to " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }

        static void loadFromFile(vector<Customer>& customers, const string& filename) {
            ifstream inFile(filename);
            if (inFile.is_open()) {
                customers.clear(); // Clear existing data
                int id, fine_due, record, numCars,carID;
                string name,password;
                vector<int> rented_cars;

                while (inFile >> id >> name >> password >> fine_due >> record >> numCars) {

                    for(int i=0 ; i<numCars ; i++)
                    {
                        cin >> carID;
                        rented_cars.push_back(carID);
                    }

                    customers.push_back(Customer(name,id,password,fine_due,record,rented_cars));
                }
                inFile.close();
                cout << "Records loaded from " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }
};

class Employee : public User 
{
        vector<int> rented_cars;
        int fine_due;
        int record ;

    public : 
        Employee(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_EMPLOYEE_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}
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
            for( auto& it : rented_cars)
                cout << it << "\t" ;
            cout << endl;
            // Car :: showcars(rented_cars);
        }

        static void addEmployee(vector<Employee>& employees) {
            string name,password,temp;
            int id;

            cout << "Enter Name (no spaces): " ;
            cin >> name ;
            cout << endl;

            auto it = find_if(employees.begin(), employees.end(), [&name](const Employee& employee) {
                return employee.name == name;
            });

            if(it != employees.end())
            {
                cout << "The Employee already exists!" << endl;
                return ;
            }

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

            if(!employees.empty())
                id = employees.back().id + 1;
            else 
                id = EMPLOYEE_BEGIN_ID;
            cout << "ID of the new Employee is : " << id << endl;
            employees.push_back(Employee(name,id,password));
        }

        // Function to search for a Employee by ID
        static Employee* searchEmployeeById(vector<Employee>& Employees, int EmployeeId) {
            auto it = lower_bound(Employees.begin(), Employees.end(), EmployeeId, [](const Employee& Employee, int id) {
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
            auto it = lower_bound(Employees.begin(), Employees.end(), id,[](const Employee& Employee, int id) {
                return Employee.id < id;
            });

            // If Employee found, check password
            if (it != Employees.end()) {
                if (it->password == password) {
                    cout << "-----------------" << endl;
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

            auto it = lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
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

            auto it = lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
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
    
        void rent_request(vector<Car>& cars)
        {
            int carID;
            string date;
            int d,m,y;

            cout << "Enter the id of the car to be rented : " ;
            cin >> carID;
            cout << "Enter the date of rental in DD-MM-YYYY format : ";
            cin >> date;
            if (!parse_date(date,&d,&m,&y))
            {
                cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                return;
            }

            auto carIt = Car :: searchCarById(cars,carID);

            if(carIt->ownerID == 0){
                rented_cars.push_back(carID);
                carIt->rent_request(id,y,m,d);
            }
            else
                cout << "This car is not available for rental" << endl;
        }

        void begin_session(vector<Car>& cars)
        {
            cout << "Welcome " << name << endl;
            cout << "-----------------" << endl;
            int k;
            cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Exit" << endl;
            cin >> k;

            char delimiter;

            string date;
            DateTime dt1,dt2;
            int diff;

            Car* carIt;
            int d,m,y;
            int carID;
            regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
            switch(k)
            {
                case 1 :
                    Car :: showcars(cars,id);
                    break;
                case 2 :
                    rent_request(cars);
                    break;
                case 3 :
                    cout << "Enter the id of the car to be returned : ";
                    cin >> carID;

                    cout << "Enter the date of return in DD-MM-YYYY format : ";
                    cin >> date;


                    carIt = Car :: searchCarById(cars,carID);
                    carIt->ownerID = 0;
                    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());

                    if (!parse_date(date,&d,&m,&y))
                    {
                        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                        begin_session(cars);
                    }

                    diff = DateTime(y,m,d) - carIt->due_date;
                    fine_due += diff*PENALTY_CUSTOMER;

                    break;
                case 4:
                    return;
                default :
                    cout << "Enter a valid option" << endl;
                    begin_session(cars);
                    break;
            }
            begin_session(cars);
        }
        static void saveToFile(const vector<Employee>& employees, const string& filename) {
            ofstream outFile(filename);
            if (outFile.is_open()) {
                for (const auto& employee : employees) {
                    outFile << employee.id << " " << employee.name << " " << employee.password  << " " << employee.fine_due << " " << employee.record;
                    // cout << employee.id << " " << employee.model << " " << employee.condition << endl;

                    outFile << " " << employee.rented_cars.size(); 
                    for (int carID : employee.rented_cars) {
                        outFile << " " << carID; 
                    }
                    outFile << endl;
                }
                outFile.close();
                cout << "Records saved to " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }

        static void loadFromFile(vector<Employee>& employees, const string& filename) {
            ifstream inFile(filename);
            if (inFile.is_open()) {
                employees.clear(); // Clear existing data
                int id, fine_due, record, numCars,carID;
                string name,password;
                vector<int> rented_cars;

                while (inFile >> id >> name >> password >> fine_due >> record >> numCars) {

                    for(int i=0 ; i<numCars ; i++)
                    {
                        cin >> carID;
                        rented_cars.push_back(carID);
                    }

                    employees.push_back(Employee(name,id,password,fine_due,record,rented_cars));
                }
                inFile.close();
                cout << "Records loaded from " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
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

            cout << "Enter Name (no spaces): " ;
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
            auto it = lower_bound(Managers.begin(), Managers.end(), ManagerId, [](const Manager& Manager, int id) {
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
            auto it = lower_bound(Managers.begin(), Managers.end(), id,[](const Manager& Manager, int id) {
                return Manager.id < id;
            });

            // If Manager found, check password
            if (it != Managers.end()) {
                if (it->password == password) {
                    cout << "-----------------" << endl;
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

            auto it = lower_bound(Managers.begin(), Managers.end(), id, [](const Manager& Manager, int id) {
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

            auto it = lower_bound(Managers.begin(), Managers.end(), id, [](const Manager& Manager, int id) {
                return Manager.id < id;
            });
            if (it != Managers.end()) {
                Managers.erase(it);
            } else {
                cout << "Manager with ID " << id << " not found." << endl;
            }
        }


        static void show_managers(vector<Manager>& Managers)
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
                    cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
                    cin >> j;

                    switch (j)
                    {
                    case 1:
                        Car :: addCar(cars);
                        break;
                    case 2:
                        Car :: updateCar(cars);
                        break;
                    case 3:
                        Car :: deleteCar(cars);
                        break;
                    default:
                        cout << "Incorrect Option !" << endl;
                        modify_records(customers,cars,employees);

                        break;
                    }
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

        void begin_session(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
        {
            cout << "Welcome " << name << endl;
            cout << "-----------------" << endl;
            cout << "Choose an option\n1 - View Car Database\n2 - Modify the Records" << endl;
            int k;
            cin >> k;

            switch(k)
            {
                case 1:
                    Car :: showcars(cars,id);
                    break;
                case 2:
                    modify_records(customers,cars,employees);
                    break;
                default:
                    cout << "Invalid Option" << endl;
                    begin_session(customers,cars,employees);
            }
        }
        static void saveToFile(const vector<Manager>& managers, const string& filename) {
            ofstream outFile(filename);
            if (outFile.is_open()) {
                for (const auto& manager : managers) {
                    outFile << manager.id << " " << manager.name << " " << manager.password  << endl;

                }
                outFile.close();
                cout << "Records saved to " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }

        static void loadFromFile(vector<Manager>& managers, const string& filename) {
            ifstream inFile(filename);
            if (inFile.is_open()) {
                managers.clear(); // Clear existing data
                int id;
                string name,password;

                while (inFile >> id >> name >> password) {

                    managers.push_back(Manager(name,id,password));
                }
                inFile.close();
                cout << "Records loaded from " << filename << endl;
            } else {
                cerr << "Unable to open file " << filename << endl;
            }
        }
};

void load(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, vector<Manager> managers)
{
    Car :: loadFromFile(cars,"cars.txt");
    Customer :: loadFromFile(customers,"customers.txt");
    Employee :: loadFromFile(employees,"employees.txt");
    Manager :: loadFromFile(managers,"managers.txt");
}

void save(vector<Car>& cars, vector<Customer>& customers, vector<Employee>& employees, vector<Manager> managers)
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
            cout << "Customer registered successfully, run again to login" << endl;
            break;
        //previous customer/employee/manager
        case 1 :
            cout << "Enter the type of user :\n1 - Customer\n2 - Employee\n3 - Manager" << endl;
            cin >> j;
            Customer* it1;
            Employee* it2;
            Manager* it3;
            switch(j)
            {
                case 1: 
                    it1 = Customer :: login(customers);
                    it1->begin_session(cars);
                    break;
                case 2:
                    it2 = Employee :: login(employees);
                    it2->begin_session(cars);
                    break;
                case 3:
                    it3 = Manager :: login(managers);
                    it3->begin_session(customers,cars,employees);
                    break;
            }
    }
    save(cars,customers,employees,managers);

    return 0;
}

// add functionality of setting up the due date after car is rented
// add renting cost
// how record will decide if car can be rented or not
// add how condition of car is assessed after return
// clearing dues
// keep the classes and header functions separately