#include <iostream>
#include <vector>
#include <memory>

using namespace std;


// KLASA PRACOWNIKÓW

class Employee {
private:
    string name;
    int salary;

public:
    Employee(const string& name, int salary) : name(name), salary(salary) {}
    virtual ~Employee() {}

    virtual void displayInfo() const = 0;

    string getName() const { return name; }  // Getter for name
    int getSalary() const { return salary; } // Getter for salary
};


// TYPY PRACOWNIKÓW

// iINŻYNIER

class Enjenir : public Employee {
    string faculty;
    static const int CI;

public:
    Enjenir(const string& name, const string& faculty)
        : Employee(name, 5000), faculty(faculty) {} // wypłata inżyniera

    void displayInfo() const override {
        cout << "Inzynier: " << getName() << ", Wynagrodzenie: " << getSalary()
             << ", Wydzial: " << faculty << "\n";
    }

    static int getProductValueIncrease() { return CI; }
};
const int Enjenir::CI = 200; //stała inżyniera

// MAGAZYNIER

class WarehouseWorker : public Employee {
    bool forkliftCertified;
    static const int CMag;

public:
    WarehouseWorker(const string& name, bool forkliftCertified)
        : Employee(name, 3000), forkliftCertified(forkliftCertified) {} //wypłata magayniera

    void displayInfo() const override {
        cout << "Magazynier: " << getName() << ", Wynagrodzenie: " << getSalary()
             << ", Obsluga wozka widlowego: "
             << (forkliftCertified ? "Tak" : "Nie") << "\n";
    }

    static int getStorageCapacityIncrease() { return CMag; }
};
const int WarehouseWorker::CMag = 100; //stała magazyniera

// MARKETER

class Marketer : public Employee {
    int socialMediaFollowers;
    static const int CMkt;

public:
    Marketer(const string& name, int followers)
        : Employee(name, 4000), socialMediaFollowers(followers) {} // wypłata marketera

    void displayInfo() const override {
        cout << "Marketer: " << getName() << ", Wynagrodzenie: " << getSalary()
             << ", Liczba obserwujacych: " << socialMediaFollowers << "\n";
    }

    static int getDemandIncrease() { return CMkt; }
};
const int Marketer::CMkt = 30; // stała marketera

// ROBOTNIK

class Worker : public Employee {
    float shoeSize;
    static const int CR;

public:
    Worker(const string& name, float shoeSize)
        : Employee(name, 2500), shoeSize(shoeSize) {} // wypłata robotnika

    void displayInfo() const override {
        cout << "Robotnik: " << getName() << ", Wynagrodzenie: " << getSalary()
             << ", Rozmiar buta: " << shoeSize << "\n";
    }

    static int getProductionIncrease() { return CR; }
};
const int Worker::CR = 50; // stała robotnika


// KREDYTY

struct Loan {
    int amount;
    int monthsRemaining;
    int monthlyPayment;
    bool isPaidOff = false;  // Flaga spłacenia

    Loan(int amount, int months, int interestRate)
        : amount(amount), monthsRemaining(months) {
        monthlyPayment = amount * (1 + interestRate / 100.0) / months;
    }
};


// KLASA FIRMY

class Company {
    int balance = 200000; // początkowy balans konta firmowego
    int debt = 0;
    int productStock = 0;
    int productsSoldLastMonth = 0;
    int productPrice = 0;
    int productionCapacity = 0;
    int storageCapacity = 0;
    vector<int> revenueHistory;
    int companyValue = 0;
    int goalValue = 50000; // cel wartości firmy
    vector<unique_ptr<Employee>> employees;
    vector<Loan> loans;  // Lista kredytów

    static const int MAX_LOAN_TERM = 24;
    static const int MAX_LOAN_MULTIPLIER = 2;
    static const int REVENUE_HISTORY_SIZE = 3; //wartość firmy obliczana z N(REVENUE_HISTORY_SIZE) miesiecy

public:
    Company() { //początkowi pracownicy
        employees.push_back(make_unique<Enjenir>("Jan", "Informatyka"));
        employees.push_back(make_unique<WarehouseWorker>("Anna", true));
        employees.push_back(make_unique<Marketer>("Marek", 500));
        employees.push_back(make_unique<Worker>("Kasia", 40.5));
    }

    int getBalance() const { return balance; }
    int getDebt() const { return debt; }
    int getCompanyValue() const { return companyValue; }
    int getProductStock() const { return productStock; }
    int getProductsSold() const { return productsSoldLastMonth; }
    int getProductPrice() const {
        int price = 0;
        for (const auto& employee : employees) {
            if (auto eng = dynamic_cast<Enjenir*>(employee.get())) {
                price += Enjenir::getProductValueIncrease();
            }
        }
        return price;
    }
    int getProductionCapacity() const {
        int production = 0;
        for (const auto& employee : employees) {
            if (auto wr = dynamic_cast<Worker*>(employee.get())) {
                production += Worker::getProductionIncrease();
            }
        }
        return production;
    }
    int getStorageCapacity() const {
        int capacity = 0;
        for (const auto& employee : employees) {
            if (auto wh = dynamic_cast<WarehouseWorker*>(employee.get())) {
                capacity += WarehouseWorker::getStorageCapacityIncrease();
            }
        }
        return capacity;
    }

    void addEmployee(unique_ptr<Employee> employee) {
        employees.push_back(move(employee));
    }

    void takeLoan(int amount, int term) {
        if (term > MAX_LOAN_TERM) {
            cout << "Maksymalny czas splaty kredytu to " << MAX_LOAN_TERM << " miesiecy.\n";
            return;
        }
        if (debt + amount > companyValue * MAX_LOAN_MULTIPLIER) {
            cout << "Nie możesz zaciagnac kredytu, przekracza on maksymalne zadluzenie.\n";
            return;
        }
        int interestRate = 5 + (term / 6) * 2;
        loans.push_back(Loan(amount, term, interestRate));
        balance += amount;
        debt += amount;
        cout << "Zaciągnieto kredyt w wysokosci " << amount
             << " na " << term << " miesiecy. Rata: " << loans.back().monthlyPayment << "\n";
    }

    void displayEmployees() const {
        cout << "\n Lista Pracownikow \n";
        for (const auto& employee : employees) {
            employee->displayInfo();
        }
        cout << "\n";
    }

    void calculateMonthlyResults() {
        int monthlyCosts = 0;
        int totalDemand = 0;

        for (const auto& employee : employees) {
            monthlyCosts += employee->getSalary();
            if (auto mk = dynamic_cast<Marketer*>(employee.get())) {
                totalDemand += Marketer::getDemandIncrease();
            }
        }

        int actualProduction = min(getProductionCapacity(), getStorageCapacity());
        productStock += actualProduction;
        productsSoldLastMonth = min(totalDemand, productStock);
        int revenue = productsSoldLastMonth * getProductPrice();
        productStock -= productsSoldLastMonth;

        int loanRepayments = 0;
        for (auto& loan : loans) {
            if (!loan.isPaidOff) {
                loanRepayments += loan.monthlyPayment;
                loan.monthsRemaining--;
                if (loan.monthsRemaining <= 0) {
                    debt -= loan.amount;
                    loan.isPaidOff = true;
                }
            }
        }

        balance += revenue - monthlyCosts - loanRepayments;

        if (revenueHistory.size() >= REVENUE_HISTORY_SIZE) {
            for (size_t i = 1; i < revenueHistory.size(); ++i) {
                revenueHistory[i - 1] = revenueHistory[i];
            }
            revenueHistory.back() = revenue;
        } else {
            revenueHistory.push_back(revenue);
        }

        companyValue = 0;
        for (int rev : revenueHistory) {
            companyValue += rev;
        }
        companyValue /= revenueHistory.size();
    }

    bool checkGameEnd() {
        if (balance < 0) {
            cout << "GAME OVER - firma zbankrutowala.\n";
            return true;
        }
        if (companyValue >= goalValue) {
            cout << "WIN, Osiagnąles wartosc firmy " << goalValue << "\n\n";
            return true;
        }
        return false;
    }
};


// MAIN

int main() {
    Company company;
    string command;

    while (true) {
        int balance = company.getBalance();
        int debt = company.getDebt();
        int companyValue = company.getCompanyValue();
        int productStock = company.getProductStock();
        int productsSold = company.getProductsSold();
        int productPrice = company.getProductPrice();
        int productionCapacity = company.getProductionCapacity();
        int storageCapacity = company.getStorageCapacity();

        cout << "\n";
        cout << "        MENU                          DANE FIRMY\n";
        cout << "lp - wylistuj pracownikow            Saldo konta: " << balance << "\n";
        cout << "i - zatrudnij inzyniera              Zadluzenie: " << debt << "\n";
        cout << "ma - zatrudnij magazyniera           Wartosc firmy: " << companyValue << "\n";
        cout << "mk - zatrudnij marketera             Stan magazynowy: " << productStock << "\n";
        cout << "r - zatrudnij robotnika              Sprzedane produkty: " << productsSold << "\n";
        cout << "kr - wez kredyt                      Cena produktu: " << productPrice << "\n";
        cout << "kt - zakoncz ture                    Produkcja na miesiac: " << productionCapacity << "\n";
        cout << "                                     Pojemnosc magazynu: " << storageCapacity << "\n\n";

        cout << "Podaj komende: ";
        cin >> command;

        if (command == "lp") {
            company.displayEmployees();
        } else if (command == "i") {
            string name, faculty;
            cout << "Podaj imie: ";
            cin >> name;
            cout << "Podaj nazwe wydzialu: ";
            cin >> faculty;
            company.addEmployee(make_unique<Enjenir>(name, faculty));
        } else if (command == "ma") {
            string name;
            bool forklift;
            cout << "Podaj imie: ";
            cin >> name;
            cout << "Czy obsługuje wozek widlowy (1 - tak, 0 - nie): ";
            cin >> forklift;
            company.addEmployee(make_unique<WarehouseWorker>(name, forklift));
        } else if (command == "mk") {
            string name;
            int followers;
            cout << "Podaj imie: ";
            cin >> name;
            cout << "Podaj liczbe obserwujacych: ";
            cin >> followers;
            company.addEmployee(make_unique<Marketer>(name, followers));
        } else if (command == "r") {
            string name;
            float shoeSize;
            cout << "Podaj imie: ";
            cin >> name;
            cout << "Podaj rozmiar buta: ";
            cin >> shoeSize;
            company.addEmployee(make_unique<Worker>(name, shoeSize));
        } else if (command == "kr") {
            int amount, term;
            cout << "Podaj kwote kredytu: ";
            cin >> amount;
            cout << "Podaj czas splaty (miesiace - max 24): ";
            cin >> term;
            company.takeLoan(amount, term);
        } else if (command == "kt") {
            company.calculateMonthlyResults();
            if (company.checkGameEnd()) break;
        } else {
            cout << "Nieznana komenda. Sprobuj ponownie.\n";
        }
    }

    return 0;
}
