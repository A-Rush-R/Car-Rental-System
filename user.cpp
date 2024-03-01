using namespace std;
#include "User.h"
#include "utils.h"
#define PENALTY_CUSTOMER_FRAC 2
#define PENALTY_EMPLOYEE_FRAC 1.5
#define CUSTOMER_BEGIN_ID 200001
#define EMPLOYEE_BEGIN_ID 300001
#define MANAGER_BEGIN_ID 400001
#define REPAIR_REWARD 50


int AVG_CUSTOMER_RECORD = 100;
int AVG_EMPLOYEE_RECORD = 100;

User::User(std::string name, int id, std::string password)
    : name(name), id(id), password(password) {}

Customer:: Customer(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_CUSTOMER_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}

int Customer::show_due() {
    return fine_due;
}

void Customer::clear_due() {
    fine_due = 0;
}

void Customer::show() {
    std::cout << id << " " << name << " " << fine_due << " " << record << std::endl;
    std::cout << "List of Rented Car is ";
    for (auto& it : rented_cars)
        std::cout << it << "\t";
    std::cout << std::endl;
}

void Customer::rent_request(std::vector<Car>& cars) 
{
    if( record < 100){
        cout << "Not eligible to rent a car based on current record";
        return;
    }
    int carID;
    string date;
    int d,m,y;
    int d_,y_,m_;

    cout << "Enter the id of the car to be rented : " ;
    cin >> carID;
    auto carIt = Car :: searchCarById(cars,carID);
    if(carIt == nullptr)
    {
        cout << "Car not found" << endl;
        rent_request(cars);
        return;
    }

    if(carIt->ownerID != 0 )
        cout << "This car is not available for rental" << endl;
    else{
        cout << "Enter the date of rental in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            rent_request(cars);
        }

        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            rent_request(cars);
        }
        rented_cars.push_back(carID);
        carIt->rent_request(id,y,m,d,y_,m_,d_);
    }    
}

void Customer :: return_request(vector<Car>& cars)
{
    int carID;
    Car* carIt;
    string date;
    int condition;
    int d,m,y;
    int fine,duration,payable_rent,late_duration;

    cout << "Enter the id of the car to be returned : " ;
    cin >> carID;
    carIt = Car :: searchCarById(cars,carID);
    if(carIt == nullptr)
    {
        cout << "Car is not Rented by the Customer" << endl;
        begin_session(cars);
        return;
    }

    cout << "Enter the date of return in DD-MM-YYYY format : ";
    cin >> date;
    if (!parse_date(date,&d,&m,&y))
    {
        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
        begin_session(cars);
    }
    while(DateTime(y,m,d) - carIt->due_date <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            begin_session(cars);
        }
    }

    cout << "Enter the condition of the Car\n1 - Heavy Damage\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
    cin >> condition;

    carIt->ownerID = 0;
    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());
    carIt->condition = condition;

    duration = carIt->due_date - carIt->rent_date;
    payable_rent = duration*carIt->rent;
    cout << "Amount to be paid is : " << payable_rent << endl;

    late_duration = DateTime(y,m,d) - carIt->due_date;
    late_duration = late_duration > 0 ? late_duration : 0;

    fine = carIt->rent*late_duration*PENALTY_CUSTOMER_FRAC + carIt->rent*(4 - condition) ; //fine for late return and damage(if any)
    cout << "Fine charged for late return : " << carIt->rent*late_duration*PENALTY_CUSTOMER_FRAC << endl;
    cout << "Fine charged for damages : " << carIt->rent*(4 - condition) << endl;
    cout << "Total fine : " << fine << endl;

    fine_due += fine;
}

void Customer::addCustomer(std::vector<Customer>& customers) 
{
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

Customer* Customer::login(std::vector<Customer>& customers)  {
    string name, password;
    cout << "Enter Username: ";
    cin >> name;

    // Search for the customer with the given name
    auto it = find_if(customers.begin(), customers.end(), [&name](const Customer& customer) {
        return customer.name == name;
    });

    // If customer found, check password
    if (it == customers.end()) {
        cout << "Customer not found!" << endl;
        return nullptr;
    } else 
    {
        cout << "Enter Password: ";
        cin >> password;
        if (it->password == password) {
            cout << "-----------------" << endl;
            cout << "Login Successful!" << endl;
            return &(*it);
        } else {
            cout << "Incorrect Password!" << endl;
            return nullptr;
        }
    }
}

void Customer :: update_record(int late_duration,int condition)
{
    record += 200 - 50 * ( 4 - condition) - late_duration;
}

void Customer::updateCustomer(std::vector<Customer>& customers) 
{
    int id;
    cout << "Enter the id of the Customer to be modified : " ;
    cin >> id;

    auto it = lower_bound(customers.begin(), customers.end(), id, [](const Customer& customer, int id) {
        return customer.id < id;
    });
    if (it != customers.end()) {
        //modify here
        // *it = updatedCustomer;
    } else {
        cout << "Customer with ID " << id << " not found." << endl;
    }
}

void Customer::deleteCustomer(std::vector<Customer>& customers) 
{
    int id;
    cout << "Enter the id of the Customer to be deleted : " ;
    cin >> id;
    auto it = lower_bound(customers.begin(), customers.end(), id, [](const Customer& customer, int id) {
        return customer.id < id;
    });
    // auto it = searchCustomerById(customers,id);
    if (it != customers.end()) {
        customers.erase(it);
    } else {
        cout << "Customer with ID " << id << " not found." << endl;
    }
}

Customer* Customer::searchCustomerById(std::vector<Customer>& customers, int customerId) 
{
    auto it = lower_bound(customers.begin(), customers.end(), customerId, [](const Customer& customer, int id) {
        return customer.id < id;
    });

    if (it != customers.end() && it->id == customerId) {
        return &(*it); 
    } else {
        return nullptr; 
    }
}

void Customer :: show_customers(vector<Customer>& customers) 
{
    for (auto& it : customers)
        it.show();
}

void Customer :: begin_session(std::vector<Car>& cars) 
{
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    int k;
    cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Clear Dues\n5 - Exit" << endl;
    cin >> k;

    char delimiter;

    string date;
    int late_duration;
    int duration;

    int fine,condition;
    int payable_rent;
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
            return_request(cars);
            break;
        case 4 :
            cout << "Current dues : " << show_due() << endl;
            if(!show_due())
                cout << "No dues" << endl;
            else{
                cout << "Do you want to pay dues ?\n1 - Yes\n2 - No" << endl;
                cin >> k;
                if(k == 1)
                    clear_due();
            }
            break;
        case 5 :
            return;
            break;
        default :
            cout << "Enter a valid option" << endl;
            break;
    }
    begin_session(cars);
}

void Customer::saveToFile(const std::vector<Customer>& customers, const std::string& filename) 
{
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

void Customer::loadFromFile(std::vector<Customer>& customers, const std::string& filename) 
{
    ifstream inFile(filename);
    int sum_record = 0;
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
            sum_record += record;
            customers.push_back(Customer(name,id,password,fine_due,record,rented_cars));
        }
        inFile.close();
        if(!customers.size())
            AVG_CUSTOMER_RECORD = sum_record / customers.size();
        cout << "Records loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

Employee :: Employee(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_EMPLOYEE_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}
void Employee :: set_password(string pass)
{
    password = pass;
}

void Employee :: clear_due()
{
    fine_due = 0;
}        

void Employee :: show() 
{
    cout << id << " " << name << " " << fine_due << " " << record << endl;
    cout << "List of Rented Cars is ";
    for( auto& it : rented_cars)
        cout << it << "\t" ;
    cout << endl;
    // Car :: showcars(rented_cars);
}

void Employee :: addEmployee(vector<Employee>& employees) {
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
Employee* Employee :: searchEmployeeById(vector<Employee>& Employees, int EmployeeId) {
    auto it = lower_bound(Employees.begin(), Employees.end(), EmployeeId, [](const Employee& Employee, int id) {
        return Employee.id < id;
    });

    if (it != Employees.end() && it->id == EmployeeId) {
        return &(*it); 
    } else {
        return nullptr; 
    }

}
Employee* Employee :: login(vector<Employee>& employees) {
    string name, password;
    cout << "Enter Username: ";
    cin >> name;

    // Search for the customer with the given name
    auto it = find_if(employees.begin(), employees.end(), [&name](const Employee& employee) {
        return employee.name == name;
    });

    // If customer found, check password
    if (it == employees.end()) {
        cout << "Employee not found!" << endl;
        return nullptr;
    } 
    else 
    {
        cout << "Enter Password: ";
        cin >> password;
        if (it->password == password) {
            cout << "-----------------" << endl;
            cout << "Login Successful!" << endl;
            return &(*it);
        } else {
            cout << "Incorrect Password!" << endl;
            return nullptr;
        }
    }
}

// Function to update an existing Employee in the database
void Employee :: updateEmployee(vector<Employee>& Employees) {
    
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
void Employee :: deleteEmployee(vector<Employee>& Employees) {

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

void Employee :: show_employees(vector<Employee>& Employees)
{
    for(auto& it : Employees)
        it.show();
}

void Employee :: rent_request(vector<Car>& cars)
{
    if( record < 100){
        cout << "Not eligible to rent a car based on current record";
        return;
    }
    int carID;
    string date;
    int d,m,y;
    int d_,y_,m_;

    cout << "Enter the id of the car to be rented : " ;
    cin >> carID;
    auto carIt = Car :: searchCarById(cars,carID);

    if(carIt->ownerID != 0 )
        cout << "This car is not available for rental" << endl;
    else{
        cout << "Enter the date of rental in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            rent_request(cars);
        }

        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            rent_request(cars);
        }
        rented_cars.push_back(carID);
        carIt->rent_request(id,y,m,d,y_,m_,d_);
    }    
}

void Employee :: return_request(vector<Car>& cars)
{
    int carID;
    Car* carIt;
    string date;
    int condition;
    int d,m,y;
    int fine,duration,payable_rent,late_duration;

    cout << "Enter the id of the car to be returned : " ;
    cin >> carID;
    carIt = Car :: searchCarById(cars,carID);
    if(carIt == nullptr)
    {
        cout << "Car is not Rented by the Customer" << endl;
        begin_session(cars);
        return;
    }

    cout << "Enter the date of return in DD-MM-YYYY format : ";
    cin >> date;
    if (!parse_date(date,&d,&m,&y))
    {
        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
        begin_session(cars);
    }
    while(DateTime(y,m,d) - carIt->due_date <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
            begin_session(cars);
        }
    }
    
    cout << "Enter the condition of the Car\n1 - Heavy Damage\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
    cin >> condition;

    // Check if the input matches the pattern
    if (!parse_date(date,&d,&m,&y))
    {
        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
        begin_session(cars);
    }

    carIt->ownerID = 0;
    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());
    carIt->condition = condition;

    duration = carIt->due_date - carIt->rent_date;
    payable_rent = duration*carIt->rent;
    cout << "Amount to be paid is : " << payable_rent << endl;

    late_duration = DateTime(y,m,d) - carIt->due_date;
    late_duration = late_duration > 0 ? late_duration : 0;

    fine = carIt->rent*late_duration*PENALTY_EMPLOYEE_FRAC + carIt->rent*(4 - condition) ; //fine for late return and damage(if any)
    cout << "Fine charged for late return : " << carIt->rent*late_duration*PENALTY_EMPLOYEE_FRAC << endl;
    cout << "Fine charged for damages : " << carIt->rent*(4 - condition) << endl;
    cout << "Total fine : " << fine << endl;

    fine_due += fine;
}

void Employee :: update_record(int late_duration,int condition)
{
    record += 200 - 50 * ( 4 - condition) - late_duration;
}

int Employee :: show_due()
{
    return fine_due;
}

void Employee :: begin_session(vector<Car>& cars)
{
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    int k;
    cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Clear Dues\n5 - Repair Car\n6 - Exit" << endl;
    cin >> k;

    char delimiter;

    string date;
    int late_duration;
    int duration;

    int fine,condition;
    int payable_rent;
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
            return_request(cars);
            break;
        case 4 :
            cout << "Current dues : " << show_due() << endl;
            if(!show_due())
                cout << "No dues" << endl;
            else{
                cout << "Do you want to pay dues ?\n1 - Yes\n2 - No" << endl;
                cin >> k;
                if(k == 1)
                    clear_due();
            }
            break;
        case 5 :
            cout << "Enter the ID of the car to be Repaired : ";
            cin >> carID;

            carIt = Car :: searchCarById(cars,carID);

            record += REPAIR_REWARD * (4 - condition); 
            carIt->repair();
            break;
        case 6 :
            return;
            break;
        default :
            cout << "Enter a valid option" << endl;
            break;
    }
    begin_session(cars);
}

void Employee :: saveToFile(const vector<Employee>& employees, const string& filename) {
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

void Employee :: loadFromFile(vector<Employee>& employees, const string& filename) {
    int sum_record = 0;
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
            sum_record += record;
            employees.push_back(Employee(name,id,password,fine_due,record,rented_cars));
        }
        inFile.close();
        if(!employees.size())
            AVG_EMPLOYEE_RECORD = sum_record / employees.size();
        cout << "Records loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

void Manager :: set_password( string pass) {password = pass;} 
Manager :: Manager(const string& name = "", int id = 0, const string& password = "") : User(name, id, password){}

void Manager :: show_records(vector<Customer>& customers, vector<Employee>& employees, vector<Car>& cars)
{
    int k;
    cout << "Which Records are to be shown?\n1 - Cars\n2 - Customers\n3 - Employees\n4 - Exit" << endl;
    cin >> k;

    switch(k)
    {
        case 1 :
            Car :: showcars(cars,id);
            break;
        case 2 :
            Customer :: show_customers(customers);
            break;
        case 3 :
            Employee :: show_employees(employees);
            break;
        case 4 :
            return;
        default : 
            cout << "Invalid Input" << endl;
            break;
    }
    show_records(customers,employees,cars);
}

void Manager :: addManager(vector<Manager>& managers) {
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
    if(!managers.empty())
        id = managers.back().id + 1;
    else 
        id = MANAGER_BEGIN_ID;
    cout << "ID of the new Employee is : " << id << endl;
    managers.push_back(Manager(name,id,password));
}

// Function to search for a Manager by ID
Manager* Manager :: searchManagerById(vector<Manager>& Managers, int ManagerId) {
    auto it = lower_bound(Managers.begin(), Managers.end(), ManagerId, [](const Manager& Manager, int id) {
        return Manager.id < id;
    });

    if (it != Managers.end() && it->id == ManagerId) {
        return &(*it); 
    } else {
        return nullptr; 
    }
}

Manager* Manager :: login(vector<Manager>& Managers) {
    string name, password;
    cout << "Enter Username: ";
    cin >> name;

    // Search for the Manager with the given name
    auto it = find_if(Managers.begin(), Managers.end(), [&name](const Manager& manager) {
        return manager.name == name;
    });

    // If Manager found, check password
    if (it == Managers.end()) {
        cout << "Manager not found!" << endl;
        return nullptr;
    } else 
    {
        cout << "Enter Password: ";
        cin >> password;
        if (it->password == password) {
            cout << "-----------------" << endl;
            cout << "Login Successful!" << endl;
            return &(*it);
        } else {
            cout << "Incorrect Password!" << endl;
            return nullptr;
        }
    }
}

// Function to update an existing Manager in the database
void Manager :: updateManager(vector<Manager>& Managers) {
    
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
void Manager :: deleteManager(vector<Manager>& Managers) {

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


void Manager :: show_managers(vector<Manager>& Managers)
{
    for(auto& it : Managers)
        it.show();
}

void Manager :: modify_records(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
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

void Manager :: show()
{
    cout << id << " " << name << endl;
}

void Manager :: begin_session(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
{
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    cout << "Choose an option\n1 - View the Records\n2 - Modify the Records\n3 - Exit" << endl;
    int k;
    cin >> k;

    switch(k)
    {
        case 1:
            show_records(customers,employees,cars);
            break;
        case 2:
            modify_records(customers,cars,employees);
            break;
        case 3:
            return;
        default:
            cout << "Invalid Option" << endl;
    }
    begin_session(customers,cars,employees);
}

void Manager :: saveToFile(const vector<Manager>& managers, const string& filename) {
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

void Manager :: loadFromFile(vector<Manager>& managers, const string& filename) {
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
