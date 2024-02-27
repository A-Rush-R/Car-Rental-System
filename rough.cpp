
void saveToFile(const vector<Manager>& managers, const string& filename) {
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

void loadFromFile(vector<Manager>& managers, const string& filename) {
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