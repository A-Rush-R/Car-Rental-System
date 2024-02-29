        static Manager* login(vector<Manager>& Managers) {
            string name, password;
            cout << "Enter Username: ";
            cin >> name;
            cout << "Enter Password: ";
            cin >> password;

            // Search for the Manager with the given name
            auto it = find_if(Managers.begin(), Managers.end(), [&name](const Manager& manager) {
                return manager.name == name;
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