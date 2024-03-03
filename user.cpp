#include "user.h"
#include "utils.h"
#include<iomanip>
#include "constants.h"
using namespace std;


int AVG_CUSTOMER_RECORD = CUSTOMER_RECORD;
int AVG_EMPLOYEE_RECORD = EMPLOYEE_RECORD; 

User::User(std::string name, int id, std::string password)
    : name(name), id(id), password(password) {}

Customer:: Customer(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_CUSTOMER_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}
Employee :: Employee(const string& name = "", int id = 0, const string& password = "", int fine_due = 0, int record = AVG_EMPLOYEE_RECORD,const vector<int>& rented_cars = {}) : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}

int Customer::show_due() {
    return fine_due;
}

void Customer::clear_due() {
    fine_due = 0;
}

void Customer::show() {
    cout << left << setw(8) << id << setw(NAME_CAP) << name << setw(10) << fine_due << setw(18) << record ;
    for (auto& it : rented_cars)
        cout << setw(8) << it ;
    cout << endl;
}

void Employee :: show() 
{
    cout << left << setw(8) << id << setw(NAME_CAP) << name << setw(10) << fine_due << setw(18) << record ;
    for (auto& it : rented_cars)
        cout << setw(8) << it ;
    cout << endl;
}

void Customer::rent_request(std::vector<Car>& cars) 
{
    int carID;
    string date;
    int d,m,y;
    int d_,y_,m_;

    cout << "Enter the id of the car to be rented : " ;
    if(!(cin >> carID)){
        non_int();
        exit();
        return;
    }
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
            rent_request(cars);
            return;
        }

        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            rent_request(cars);
            return;
        }
    while(DateTime(y_,m_,d_) - DateTime(y,m,d) <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            rent_request(cars);
            return;
        }
    }
        record -= THRESHOLD_RECORD;
        rented_cars.push_back(carID);
        carIt->rent_request(id,y_,m_,d_,y,m,d);
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
    if(!(cin >> carID)){
        non_int();
        exit();
        return;
    }
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
        begin_session(cars);
        return;
    }
    while(DateTime(y,m,d) - carIt->rent_date <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            begin_session(cars);
            return;
        }
    }

    cout << "Enter the condition of the Car\n1 - Heavy Damage\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
    if(!(cin >> condition)){
        non_int();
        exit();
        return;
    }

    carIt->ownerID = 0;
    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());
    int delta = carIt->condition - condition;
    carIt->condition = condition;

    duration = carIt->due_date - carIt->rent_date;
    payable_rent = duration*carIt->rent;
    cout << "Amount to be paid is : " << payable_rent << endl;

    late_duration = DateTime(y,m,d) - carIt->due_date;
    late_duration = late_duration > 0 ? late_duration : 0;
    update_record(late_duration,delta);

    fine = carIt->rent*late_duration*PENALTY_CUSTOMER_FRAC + carIt->rent*(FINE - condition) ; //fine for late return and damage(if any)
    cout << "Fine charged for late return : " << carIt->rent*late_duration*PENALTY_CUSTOMER_FRAC << endl;
    cout << "Fine charged for damages : " << carIt->rent*(FINE - condition) << endl;
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
    cout << "Customer with ID " << id << " added successfully." << endl;
    customers.push_back(Customer(name,id,password,0,AVG_CUSTOMER_RECORD));
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

void Customer :: update_record(int late_duration,int delta)
{
    record = record + RETURN_REWARD - DAMAGE_PENALTY *delta - LATE_RECORD_CHARGE*late_duration;
    // cout << "record is " << record << endl;
}

void Customer::updateCustomer(std::vector<Customer>& customers) 
{
    int id;
    cout << "Enter the id of the Customer to be modified : " ;
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }
    auto it = lower_bound(customers.begin(), customers.end(), id, [](const Customer& customer, int id) {
        return customer.id < id;
    });
    if (it != customers.end()) {
        string name,password;
        int k;

        cout << "Choose the entry to modify" << endl;
        cout << "1 - Name\n2 - Password\n3 - Fine due\n4 - Record" <<endl;
        if(!(cin >> k)){
            non_int();
            exit();
            return;
        }
        switch(k)
        {
            case 1 :
                cout << "Enter the new name : " ;
                cin >> name;
                it->name = name;
                break;
            case 2 :
                cout << "Enter the new password : ";
                cin >> password;
                it->set_password(password);
                break;
            case 3 :
                cout << "Enter the new fine-due : ";
                if(!(cin >> it->fine_due)){
                    non_int();
                    exit();
                    return;
                }
                break;
            case 4 :
                cout << "Enter the new record : ";
                if(!(cin >> it->record)){
                    non_int();
                    exit();
                    return;
                }
                break;
            default :
                invalid_option();
                updateCustomer(customers);
                break;
        }
    } else {
        cout << "Customer with ID " << id << " not found." << endl;
    }
}

void Customer::deleteCustomer(std::vector<Customer>& customers) 
{
    int id;
    cout << "Enter the id of the Customer to be deleted : " ;
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }
    auto it = lower_bound(customers.begin(), customers.end(), id, [](const Customer& customer, int id) {
        return customer.id < id;
    });
    // auto it = searchCustomerById(customers,id);
    if (it == customers.end()) {
        cout << "Customer with ID " << id << " not found." << endl;
    } 
    else if(it->rented_cars.size() != 0){
        cout << "Customer with ID " << id << " has rented a car and cannot be deleted." << endl;
    }
    else {
        customers.erase(it);
        cout << "Customer with ID " << id << " deleted." << endl;
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
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(8) << "ID" << setw(NAME_CAP) << "Name" << setw(10) << "Fine" << setw(18) << "Customer-Record" << setw(8) << "Rented-Cars" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    for (auto& it : customers)
        it.show();
}



void Employee :: set_password(string pass)
{
    password = pass;
}
void Customer :: set_password(string pass)
{
    password = pass;
}

void Employee :: clear_due()
{
    fine_due = 0;
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
    cout << "Employee with ID " << id << " added successfully." << endl;
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
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }

    auto it = lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
        return Employee.id < id;
    });

    if (it != Employees.end()) {
        string name,password;
        int k;

        cout << "Choose the entry to modify" << endl;
        cout << "1 - Name\n2 - Password\n3 - Fine due\n4 - Record" <<endl;
        if(!(cin >> k)){
            non_int();
            exit();
            return;
        }
        switch(k)
        {
            case 1 :
                cout << "Enter the new name : " ;
                cin >> name;
                it->name = name;
                break;
            case 2 :
                cout << "Enter the new password : ";
                cin >> password;
                it->set_password(password);
                break;
            case 3 :
                cout << "Enter the new fine-due : ";
                if(!(cin >> it->fine_due)){
                    non_int();
                    exit();
                    return;
                }
                break;
            case 4 :
                cout << "Enter the new record : ";
                if(!(cin >> it->record)){
                    non_int();
                    exit();
                    return;
                }
                break;
            default :
                invalid_option();
                updateEmployee(Employees);
                break;
        }
    } else {
        cout << "Employee with ID " << id << " not found." << endl;
    }
}

// Function to delete a Employee from the database
void Employee :: deleteEmployee(vector<Employee>& Employees) {

    int id;
    cout << "Enter the id of the Customer to be deleted : " ;
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }

    auto it = lower_bound(Employees.begin(), Employees.end(), id, [](const Employee& Employee, int id) {
        return Employee.id < id;
    });
    if (it == Employees.end()) {
        cout << "Employee with ID " << id << " not found." << endl;
    } 
    else if (it->rented_cars.size() != 0){
        cout << "Employee with ID " << id << " has rented a car and connot be deleted." << endl;
    }
    else {
        Employees.erase(it);
        cout << "Employee with ID " << id << " deleted." << endl;
    }
}

void Employee :: show_employees(vector<Employee>& Employees)
{
    cout << "-------------------------------------------------------------------------------------------" << endl;
    cout << left << setw(8) << "ID" << setw(NAME_CAP) << "Name" << setw(10) << "Fine" << setw(18) << "Employee-Record" << setw(8) << "Rented-Cars" << endl;
    cout << "-------------------------------------------------------------------------------------------" << endl;
    for (auto& it : Employees)
        it.show();
}


void Employee :: rent_request(vector<Car>& cars)
{
    int carID;
    string date;
    int d,m,y;
    int d_,y_,m_;

    cout << "Enter the id of the car to be rented : " ;
    if(!(cin >> carID)){
        non_int();
        exit();
        return;
    }
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
            rent_request(cars);
            return;
        }

        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            rent_request(cars);
            return;
        }
    while(DateTime(y_,m_,d_) - DateTime(y,m,d) <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d_,&m_,&y_))
        {
            rent_request(cars);
            return;
        }
    }
        record -= THRESHOLD_RECORD;
        rented_cars.push_back(carID);
        carIt->rent_request(id,y_,m_,d_,y,m,d);
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
    if(!(cin >> carID)){
        non_int();
        exit();
        return;
    }
    carIt = Car :: searchCarById(cars,carID);
    if(carIt == nullptr)
    {
        cout << "Car is not Rented by the Employee" << endl;
        begin_session(cars);
        return;
    }

    cout << "Enter the date of return in DD-MM-YYYY format : ";
    cin >> date;
    if (!parse_date(date,&d,&m,&y))
    {
        begin_session(cars);
        return;
    }
    while(DateTime(y,m,d) - carIt->due_date <= 0)
    {
        cout << "Date of return must be at least one day from date of rental!" << endl;
        cout << "Enter the date of return in DD-MM-YYYY format : ";
        cin >> date;
        if (!parse_date(date,&d,&m,&y))
        {
            begin_session(cars);
            return;
        }
    }

    cout << "Enter the condition of the Car\n1 - Heavy Damage\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
    if(!(cin >> condition)){
        non_int();
        exit();
        return;
    }

    carIt->ownerID = 0;
    rented_cars.erase(remove(rented_cars.begin(), rented_cars.end(), carID), rented_cars.end());
    int delta = carIt->condition - condition;
    carIt->condition = condition;

    duration = carIt->due_date - carIt->rent_date;
    payable_rent = duration*carIt->rent;
    cout << "Amount to be paid is : " << payable_rent << endl;

    late_duration = DateTime(y,m,d) - carIt->due_date;
    late_duration = late_duration > 0 ? late_duration : 0;
    update_record(late_duration,delta);

    fine = carIt->rent*late_duration*PENALTY_EMPLOYEE_FRAC + carIt->rent*(FINE - condition) ; //fine for late return and damage(if any)
    cout << "Fine charged for late return : " << carIt->rent*late_duration*PENALTY_EMPLOYEE_FRAC << endl;
    cout << "Fine charged for damages : " << carIt->rent*(FINE - condition) << endl;
    cout << "Total fine : " << fine << endl;

    fine_due += fine;
}

void Employee :: update_record(int late_duration,int delta)
{
    record = record + RETURN_REWARD - DAMAGE_PENALTY* delta- LATE_RECORD_CHARGE * late_duration;
    // cout << "record is " << record << endl;
}

int Employee :: show_due()
{
    return fine_due;
}



void Manager :: set_password( string pass) {password = pass;} 
Manager :: Manager(const string& name = "", int id = 0, const string& password = "") : User(name, id, password){}

void Manager :: show_records(vector<Customer>& customers, vector<Employee>& employees, vector<Car>& cars)
{
    int k;
    cout << "Which Records are to be shown?\n1 - Cars\n2 - Customers\n3 - Employees\n4 - Exit" << endl;
    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }

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
            exit();
            return;
        default : 
            invalid_option();
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
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }

    auto it = lower_bound(Managers.begin(), Managers.end(), id, [](const Manager& Manager, int id) {
        return Manager.id < id;
    });

    int manID,k;
    string name,password;
    if (it != Managers.end()) {
        cout << "Choose the entry to modify" << endl;
        cout << "1 - Name\n2 - Password" <<endl;
        if(!(cin >> k)){
            non_int();
            exit();
            return;
        }
        switch(k)
        {
            case 1 :
                cout << "Enter the new name : " ;
                cin >> name;
                it->name = name;
                break;
            case 2 :
                cout << "Enter the new password : ";
                cin >> password;
                it->set_password(password);
                break;
            default :
                invalid_option();
                updateManager(Managers);
                break;
        }
    } else {
        cout << "Manager with ID " << id << " not found." << endl;
    }
}

// Function to delete a Manager from the database
void Manager :: deleteManager(vector<Manager>& Managers) {

    int id;
    cout << "Enter the id of the Customer to be deleted : " ;
    if(!(cin >> id)){
        non_int();
        exit();
        return;
    }

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

void Manager :: search_records(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
{
    int k;
    int id;

    cout << "What do you want to search\n1 - Car\n2 - Customer\n3 - Employee\n4 - Exit" << endl ;
    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }
    switch(k)
    {
        case 1: 
            cout << "Enter the id of car to be searched : " ;
            cin >> id;
            if(Car :: searchCarById(cars,id) == nullptr)
                cout << "Car not found!" << endl;
            else{
                cout << "-----------------------------------------------------------------------------------------" << endl;
                cout << left << setw(8) << "ID" << setw(15) << "Model" << setw(20) << "Condition" << setw(12) << "Status" << setw(7) << "Rent" << setw(8) << "OwnerID" << setw(11) << "Rent-Date" << setw(10) << "Due-Date" << endl;
                cout << "-----------------------------------------------------------------------------------------" << endl;
                Car :: searchCarById(cars,id)->show(0);
            }
            break;
        case 2:
            cout << "Enter the id of the customer to be searched : ";
            cin >> id;
            if(Customer :: searchCustomerById(customers,id) == nullptr)
                cout << "Customer not found!" << endl;
            else{
                cout << "-------------------------------------------------------------------------------------------" << endl;
                cout << left << setw(8) << "ID" << setw(NAME_CAP) << "Name" << setw(10) << "Fine" << setw(18) << "Customer-Record" << setw(8) << "Rented-Cars" << endl;
                cout << "-------------------------------------------------------------------------------------------" << endl;
                Customer :: searchCustomerById(customers,id)->show();
            }
            break;
        case 3:
            cout << "Enter the id of the employee to be searched : ";
            cin >> id;
            if(Employee :: searchEmployeeById(employees,id) == nullptr)
                cout << "Employee not found!" << endl;
            else{
                cout << "-------------------------------------------------------------------------------------------" << endl;
                cout << left << setw(8) << "ID" << setw(NAME_CAP) << "Name" << setw(10) << "Fine" << setw(18) << "Employee-Record" << setw(8) << "Rented-Cars" << endl;
                cout << "-------------------------------------------------------------------------------------------" << endl;
                Employee :: searchEmployeeById(employees,id)->show();
            }
            break;
        case 4 :
            return;
    }
    search_records(customers,cars,employees);
}

void Manager :: modify_records(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
{
    cout << "Which database is to be modified?\n1 - Customers\n2 - Employees\n3 - Cars\n4 - Exit" << endl;
    int k,j;

    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }

    string name,password;
    switch(k){
        case 1 : 
            cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
            if(!(cin >> j)){
                non_int();
                exit();
                return;
            }

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
            case 4 :
                begin_session(customers,cars,employees);
                return;
            default:
                invalid_option();
                modify_records(customers,cars,employees);
                return;
            }
            break;
        case 2 : 
            cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
            if(!(cin >> j)){
                non_int();
                exit();
                return;
            }

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
                invalid_option();
                modify_records(customers,cars,employees);

                break;
            }
            break ;
        case 3 : 
            cout << "Choose the operation you want to perform\n1 - Add\n2 - Update\n3 - Delete" << endl;
            if(!(cin >> j)){
                non_int();
                exit();
                return;
            }

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
                invalid_option();
                modify_records(customers,cars,employees);

                break;
            }
            break;
        default :
            invalid_option();
            modify_records(customers,cars,employees);
            break;
    }
}

void Manager :: show()
{
    cout << left << setw(8) << id << setw(NAME_CAP) << name << endl;
}

void Customer :: begin_session(vector<Car>& cars) 
{
    cout << "-----------------" << endl;
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    int k;
    cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Clear Dues\n5 - Exit" << endl;
    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }

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
            if(record >= THRESHOLD_RECORD)
                rent_request(cars);
            else 
                cout << "Currently not eligible for renting a car" << endl;
            break;

        case 3 :
            if(!rented_cars.empty())
                return_request(cars);
            else 
                cout << "No cars rented currently" << endl;
            break;
        case 4 :
            cout << "Current dues : " << show_due() << endl;
            if(!show_due())
                cout << "No dues" << endl;
            else{
                cout << "Do you want to pay dues ?\n1 - Yes\n2 - No" << endl;
                if(!(cin >> k)){
                    non_int();
                    exit();
                    return;
                }
                if(k == 1)
                    clear_due();
            }
            break;
        case 5 :
            exit();
            return;
            break;
        default :
            invalid_option();
            break;
    }
    begin_session(cars);
}
void Employee :: begin_session(vector<Car>& cars)
{
    cout << "-----------------" << endl;
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    int k;
    cout << "Choose an option\n1 - Show Available Cars\n2 - Rent a Car\n3 - Return a Car\n4 - Clear Dues\n5 - Repair Car\n6 - Exit" << endl;
    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }

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
            if( record < THRESHOLD_RECORD)
                cout << "Currently not eligible to rent a car" << endl;
            else 
                rent_request(cars);
            break;

        case 3 :
            if(rented_cars.empty())
                cout << "No cars rented currently" << endl;
            else 
                return_request(cars);
            break;
        case 4 :
            cout << "Current dues : " << show_due() << endl;
            if(!show_due())
                cout << "No dues" << endl;
            else{
                cout << "Do you want to pay dues ?\n1 - Yes\n2 - No" << endl;
                if(!(cin >> k)){
                    non_int();
                    exit();
                    return;
                }
                if(k == 1)
                    clear_due();
            }
            break;
        case 5 :
            cout << "Enter the ID of the car to be Repaired : ";
            if(!(cin >> carID)){
                non_int();
                exit();
                return;
            }

            carIt = Car :: searchCarById(cars,carID);
            if(carIt == nullptr)
                cout << "Car not found" << endl ;
            else if (carIt->condition == FINE)
                cout << "Car is already in good condition" << endl;
            else if (carIt->ownerID  != 0)
                cout << "Car is rented currently" << endl;
            else{
                record = record + REPAIR_REWARD * (FINE - carIt->condition); 
                carIt->repair();
            }
            break;
        case 6 :
            exit();
            return;
            break;
        default :
            invalid_option();
            break;
    }
    begin_session(cars);
}
void Manager :: begin_session(vector<Customer>& customers, vector<Car>& cars, vector<Employee>& employees)
{
    cout << "-----------------" << endl;
    cout << "Welcome " << name << endl;
    cout << "-----------------" << endl;
    cout << "Choose an option\n1 - View the Records\n2 - Modify the Records\n3 - Search the Records\n4 - Exit" << endl;
    int k;
    if(!(cin >> k)){
        non_int();
        exit();
        return;
    }

    switch(k)
    {
        case 1:
            show_records(customers,employees,cars);
            break;
        case 2:
            modify_records(customers,cars,employees);
            break;
        case 3:
            search_records(customers,cars,employees);
            break;
        case 4:
            exit();
            return;
        default:
            invalid_option();
    }
    begin_session(customers,cars,employees);
}

void Customer :: saveToFile(const std::vector<Customer>& customers, const std::string& filename) 
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

void Employee :: saveToFile(const vector<Employee>& employees, const string& filename) 
{
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& employee : employees) {
            outFile << employee.id << " " << employee.name << " " << employee.password  << " " << employee.fine_due << " " << employee.record ;

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
            rented_cars.clear();
            for(int i=0 ; i<numCars ; i++)
            {
                inFile >> carID;
                rented_cars.push_back(carID);
            }
            sum_record += record;
            customers.push_back(Customer(name,id,password,fine_due,record,rented_cars));
        }
        inFile.close();
        if(customers.size())
            AVG_CUSTOMER_RECORD = sum_record / customers.size();
        cout << "Records loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}
void Employee ::loadFromFile(std::vector<Employee>& employess, const std::string& filename) 
{
    ifstream inFile(filename);
    int sum_record = 0;
    if (inFile.is_open()) {
        employess.clear(); // Clear existing data
        int id, fine_due, record, numCars,carID;
        string name,password;
        vector<int> rented_cars;

        while (inFile >> id >> name >> password >> fine_due >> record >> numCars) {
            rented_cars.clear();
            for(int i=0 ; i<numCars ; i++)
            {
                inFile >> carID;
                rented_cars.push_back(carID);
            }
            sum_record += record;
            employess.push_back(Employee(name,id,password,fine_due,record,rented_cars));
        }
        inFile.close();
        if(employess.size())
            AVG_EMPLOYEE_RECORD = sum_record / employess.size();
        cout << "Records loaded from " << filename << endl;
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
