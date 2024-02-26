    // Function to add a new Employee to the database
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