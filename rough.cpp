void saveToFile(const vector<Customer>& Customers, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& Customer : Customers) {
            outFile << Customer.id << " " << Customer.name << " " << Customer.condition << " " << Customer.due_date.getYear() << " " << Customer.due_date.getMonth() << " " << Customer.due_date.getDay() << endl;
            // cout << Customer.id << " " << Customer.model << " " << Customer.condition << endl;
        }
        outFile.close();
        cout << "Records saved to " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

void loadFromFile(vector<Customer>& Customers, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        Customers.clear(); // Clear existing data
        int id, model, condition;
        int y,m,d;

        while (inFile >> id >> model >> condition >> y >> m >> d) {
            DateTime due_date(y,m,d);
            Customer Customer(id,model,condition,due_date);
            Customers.push_back(Customer);
        }
        inFile.close();
        cout << "Records loaded from " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}