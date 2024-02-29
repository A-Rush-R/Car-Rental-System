#include "User.h"

User::User(std::string name, int id, std::string password)
    : name(name), id(id), password(password) {}

Customer::Customer(const std::string& name, int id, const std::string& password, int fine_due, int record, const std::vector<int>& rented_cars)
    : User(name, id, password), fine_due(fine_due), record(record), rented_cars(rented_cars) {}

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

void Customer::rent_request(std::vector<Car>& cars) {
    // Implementation of rent_request function
}

void Customer::begin_session(std::vector<Car>& cars) {
    // Implementation of begin_session function
}

void Customer::addCustomer(std::vector<Customer>& customers) {
    // Implementation of addCustomer function
}

Customer* Customer::login(std::vector<Customer>& customers) {
    // Implementation of login function
    return nullptr;
}

void Customer::updateCustomer(std::vector<Customer>& customers) {
    // Implementation of updateCustomer function
}

void Customer::deleteCustomer(std::vector<Customer>& customers) {
    // Implementation of deleteCustomer function
}

Customer* Customer::searchCustomerById(std::vector<Customer>& customers, int customerId) {
    // Implementation of searchCustomerById function
    return nullptr;
}

void Customer::show_customers(std::vector<Customer>& customers) {
    // Implementation of show_customers function
}

void Customer::saveToFile(const std::vector<Customer>& customers, const std::string& filename) {
    // Implementation of saveToFile function
}

void Customer::loadFromFile(std::vector<Customer>& customers, const std::string& filename) {
    // Implementation of loadFromFile function
}