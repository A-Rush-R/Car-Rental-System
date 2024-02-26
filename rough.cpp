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