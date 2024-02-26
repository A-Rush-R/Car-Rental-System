        void begin_session(vector<Customer>& Customers)
        {
            cout << "Welcome " << name << endl;
            int k;
            cout << "Choose an option\n1 - Show Available Customers\n2 - Rent a Customer\n3 - Return a Customer" << endl;
            cin >> k;

            char delimiter;

            string date;
            DateTime dt1,dt2;
            int diff;

            Customer* CustomerIt;
            int d,m,y;
            int CustomerID;
            regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
            switch(k)
            {
                case 1 :
                    Customer :: showCustomers(Customers, rented_Customers, id);
                    break;
                case 2 :
                    cout << "Enter the id of the Customer to be rented : " ;
                    cin >> CustomerID;
                    cout << "Enter the date of rental in DD-MM-YYYY format : ";
                    cin >> date;
                    parse_date(date,&d,&m,&y);

                    CustomerIt = Customer :: searchCustomerById(Customers,CustomerID);

                    if(CustomerIt->ownerID == 0){
                        CustomerIt->ownerID = id;
                        CustomerIt->due_date = DateTime(y,m,d);
                        rented_Customers.push_back(CustomerID);
                    }
                    else
                        cout << "This Customer is not available for rental" << endl;

                    begin_session(Customers);
                    break;

                case 3 :
                    cout << "Enter the id of the Customer to be returned" << endl;
                    cin >> CustomerID;
                    cout << "Enter the date of return in DD-MM-YYYY format : ";
                    cin >> date;


                    // Check if the input matches the pattern
                    if (regex_match(date, pattern)) {}
                    else {
                        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                        begin_session(Customers);
                    }

                    CustomerIt = Customer :: searchCustomerById(Customers,CustomerID);
                    CustomerIt->ownerID = 0;
                    rented_Customers.erase(remove(rented_Customers.begin(), rented_Customers.end(), CustomerID), rented_Customers.end());

                    parse_date(date,&d,&m,&y);

                    diff = DateTime(y,m,d) - CustomerIt->due_date;
                    fine_due += diff*PENALTY_CUSTOMER;

                    break;

                default :
                    cout << "Enter a valid option" << endl;
                    begin_session(Customers);
                    break;
            }
        }