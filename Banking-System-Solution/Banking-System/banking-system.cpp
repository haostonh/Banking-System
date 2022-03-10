#include"input-checkers.h"

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include"mysql_driver.h"
#include"mysql_connection.h"
#include<cppconn/driver.h>
#include<cppconn/exception.h>
#include<cppconn/statement.h>
#include<cppconn/prepared_statement.h>
#include<cppconn/datatype.h>

class Customer {
public:
    void DepositMoney(float money) {
        bank_balance += money;
        on_hand_balance -= money;
    }
    float GetBankBalance() { return bank_balance; }
    void GetBankBalancePrompt() { std::cout << "Your Bank Balance: " << bank_balance << std::endl; }
    float GetOnHandBalance() { return on_hand_balance; }
    void GetOnHandBalancePrompt() {
        std::cout << "Your On Hand Balance: " << on_hand_balance << std::endl;
    }
    void SetBankBalance(float money) {
        bank_balance = money;
    }
    void SetOnHandBalance(float money) {
        on_hand_balance = money;
    }
    void WithdrawMoney(float money) {
        bank_balance -= money;
        on_hand_balance += money;
    }
private:
    float bank_balance, on_hand_balance;
};

void DepositFromBankBalance(Customer* person) {
    bool is_valid_deposit = false;
    float deposit_amount = 0;

    while (!is_valid_deposit) {
        deposit_amount = CheckForValidAmount("Deposit");

        if (deposit_amount > person->GetOnHandBalance()) {
            std::cout << "Cannot Deposit Amount Specified, Try Again" << std::endl;
            continue;
        }
        else if (deposit_amount < 0) {
            std::cout << "Cannot Deposit a Negative Amount, Try Again" << std::endl;
            continue;
        }
        else {
            person->DepositMoney(deposit_amount);
            std::cout << "Money Deposited Successfully!" << std::endl;
            person->GetOnHandBalancePrompt();
            person->GetBankBalancePrompt();
            is_valid_deposit = true;
        }
    }
}

void WithdrawFromBankBalance(Customer* person) {
    bool is_valid_withdrawal = false;
    float withdraw_amount = 0;

    while (!is_valid_withdrawal) {
        withdraw_amount = CheckForValidAmount("Withdraw");

        if (withdraw_amount > person->GetBankBalance()) {
            std::cout << "Cannot Withdraw Amount Specified, Try Again" << std::endl;
            continue;
        }
        else if (withdraw_amount < 0) {
            std::cout << "Cannot Withdraw a Negative Amount, Try Again" << std::endl;
            continue;
        }
        else {
            person->WithdrawMoney(withdraw_amount);
            std::cout << "Money Withdrew Successfully!" << std::endl;
            person->GetOnHandBalancePrompt();
            person->GetBankBalancePrompt();
            is_valid_withdrawal = true;
        }
    }
}

void BalanceOptions(Customer* person) {
    unsigned int bal_option_num;

    std::cout << "Enter one of the numbers to get help on balance: " << std::endl;
    std::cout << "(1) Get Bank Balance" << std::endl;
    std::cout << "(2) Deposit Into Bank Balance" << std::endl;
    std::cout << "(3) Withdraw Out Bank Balance" << std::endl;
    std::cout << "(0) Exit" << std::endl;

    std::vector<int> valid_bal_nums = { 0, 1, 2, 3 };
    bal_option_num = CheckNumberInput(valid_bal_nums);

    switch (bal_option_num) {
    case 0:
        std::cout << "Goodbye" << std::endl;
        break;
    case 1:
        person->GetBankBalancePrompt();
        break;
    case 2:
        DepositFromBankBalance(person);
        break;
    case 3:
        WithdrawFromBankBalance(person);
        break;
    default:
        std::cout << "Not a Valid Option" << std::endl;
        break;
    }
}

int main(int argc, const char **argv) {
    unsigned int service_num;
    sql::Connection* con;
    sql::Driver* driver;
    sql::Statement* statement;
    sql::PreparedStatement* pstatement;
    

    try {
        driver = get_driver_instance();
        con = driver->connect(argv[1], argv[2], argv[3]);
        std::cout << "Connection Successful! " << std::endl;
    } catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error Message: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Created schema ahead of time in MySQL Workbench
    try {
        con->setSchema("earth_bank");
    } catch (sql::SQLException& e) {
        std::cout << "Could not find schema. Error Message: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    statement = con->createStatement();
    try {
        statement->executeQuery("CREATE TABLE IF NOT EXISTS \\
            customer_information(customer_id INT PRIMARY KEY, first_name VARCHAR(20), last_name VARCHAR(20), on_hand_balance DECIMAL(10,2), bank_balance DECIMAL(10,2));");
    } catch (sql::SQLException& e) {

    }
    
    Customer* John = new Customer();
    John->SetBankBalance(213);
    John->SetOnHandBalance(200);

    std::cout << "Welcome to Earth Bank!" << std::endl << std::endl;
    std::cout << "Enter the one of the following numbers to be serviced:" << std::endl;
    std::cout << "(1) Balance" << std::endl;
    std::cout << "(0) Exit" << std::endl;

    std::vector<int> valid_service_nums = { 0, 1 };
    service_num = CheckNumberInput(valid_service_nums);

    std::cout << std::endl;

    switch (service_num) {
    case 0:
        std::cout << "Goodbye" << std::endl;
        break;
    case 1:
        std::cout << "Balance" << std::endl;
        BalanceOptions(John);
        break;
    default:
        std::cout << "Not a valid service number" << std::endl;
        break;
    }

    delete statement;
    delete con;
    return EXIT_SUCCESS;
}