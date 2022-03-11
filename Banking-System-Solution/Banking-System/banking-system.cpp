#include"input-checkers.h"

#include<iostream>
#include<sstream>
#include<string>
#include<vector>

#include"mysql_connection.h"
#include<cppconn/driver.h>
#include<cppconn/exception.h>
#include<cppconn/statement.h>

class Customer {
 public:
  Customer() {}
  Customer(std::string f_n, std::string l_n, long double b_b, long double o_n_b): 
      first_name_(f_n), last_name_(l_n), bank_balance_(b_b), on_hand_balance_(o_n_b) {
  }

  void DepositMoney(long double money) {
    bank_balance_ += money;
    on_hand_balance_ -= money;
  }
  void WithdrawMoney(long double money) {
    bank_balance_ -= money;
    on_hand_balance_ += money;
  }

  long double GetBankBalance() { return bank_balance_; }
  void GetBankBalancePrompt() { std::cout << "Your Bank Balance: " << bank_balance_ << "\n"; }

  long double GetOnHandBalance() { return on_hand_balance_; }
  void GetOnHandBalancePrompt() {
    std::cout << "Your On Hand Balance: " << on_hand_balance_ << "\n";
  }

  void SetBankBalance(long double money) {
    bank_balance_ = money;
  }
  void SetOnHandBalance(long double money) {
    on_hand_balance_ = money;
  }

  std::string GetFirstName() { return first_name_; }
  std::string GetLastName() { return last_name_; }

 private:
  long double bank_balance_ = 0;
  long double on_hand_balance_ = 0;

  std::string first_name_;
  std::string last_name_;
};

void DepositFromBankBalance(Customer* person, sql::Statement* statement) {
  bool is_valid_deposit = false;
  long double deposit_amount = 0;
  std::string bank_balance;
  std::string on_hand_balance;
  std::ostringstream ss_bank_balance;
  std::ostringstream ss_on_hand_balance;

  while (!is_valid_deposit) {
    deposit_amount = CheckForValidAmount("Deposit");

    if (deposit_amount > person->GetOnHandBalance()) {
      std::cout << "Cannot Deposit Amount Specified, Try Again" << std::endl;
      continue;
    } else if (deposit_amount < 0) {
      std::cout << "Cannot Deposit a Negative Amount, Try Again" << std::endl;
      continue;
    } else {
      person->DepositMoney(deposit_amount);
      try {
        ss_bank_balance << person->GetBankBalance();
        ss_on_hand_balance << person->GetOnHandBalance();
        bank_balance = ss_bank_balance.str();
        on_hand_balance = ss_on_hand_balance.str();
        statement->execute("UPDATE customer_information SET bank_balance = " + bank_balance + ", on_hand_balance = " + on_hand_balance + " WHERE customer_id = 1;");
      } catch (sql::SQLException& e) {
        std::cout << "Deposit Update Not Successful. Error Message: " << e.what() << "\n";
        break;
      }
      std::cout << "Money Deposited Successfully!" << std::endl;
      person->GetOnHandBalancePrompt();
      person->GetBankBalancePrompt();
      is_valid_deposit = true;
    }
  }
}

void WithdrawFromBankBalance(Customer* person, sql::Statement* statement) {
    bool is_valid_withdrawal = false;
    long double withdraw_amount = 0;
    std::string bank_balance, on_hand_balance;
    std::ostringstream ss_bank_balance, ss_on_hand_balance;

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
            try {
                ss_bank_balance << person->GetBankBalance();
                ss_on_hand_balance << person->GetOnHandBalance();
                bank_balance = ss_bank_balance.str();
                on_hand_balance = ss_on_hand_balance.str();
                statement->execute("UPDATE customer_information SET bank_balance = " + bank_balance + ", on_hand_balance = " + on_hand_balance + " WHERE customer_id = 1;");
            }
            catch (sql::SQLException& e) {
                std::cout << "Withdraw Update Not Successful. Error Message: " << e.what() << "\n";
                break;
            }
            std::cout << "Money Withdrew Successfully!" << std::endl;
            person->GetOnHandBalancePrompt();
            person->GetBankBalancePrompt();
            is_valid_withdrawal = true;
        }
    }
}

void BalanceOptions(Customer* person, sql::Statement* statement) {
    unsigned int bal_option_num;

    std::cout << "Enter one of the numbers to get help on balance:\n";
    std::cout << "(1) Get Bank Balance\n";
    std::cout << "(2) Deposit Into Bank Balance\n";
    std::cout << "(3) Withdraw Out Bank Balance\n";
    std::cout << "(0) Exit\n";

    std::vector<unsigned int> valid_bal_nums = { 0, 1, 2, 3 };
    bal_option_num = CheckNumberInput(valid_bal_nums);

    switch (bal_option_num) {
    case 0:
        std::cout << "Goodbye\n";
        break;
    case 1:
        person->GetBankBalancePrompt();
        break;
    case 2:
        DepositFromBankBalance(person, statement);
        break;
    case 3:
        WithdrawFromBankBalance(person, statement);
        break;
    default:
        std::cout << "Not a Valid Option\n";
        break;
    }
}

int main(int argc, const char **argv) {
    unsigned int service_num;
    sql::Connection* con;
    sql::Driver* driver;
    sql::Statement* statement;
    sql::ResultSet* result_set;
    
    // Make the Connection with MySQL Server
    try {
        driver = get_driver_instance();
        con = driver->connect(argv[1], argv[2], argv[3]);
        std::cout << "Connection Successful!\n" ;
    } catch (sql::SQLException& e) {
        std::cout << "Could not connect to server. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    // Created schema ahead of time in MySQL Workbench
    try {
        con->setSchema("earth_bank");
    } catch (sql::SQLException& e) {
        std::cout << "Could not find schema. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    // Create table of customer information
    statement = con->createStatement();
    try {
        statement->execute("CREATE TABLE IF NOT EXISTS customer_information(customer_id INT NOT NULL AUTO_INCREMENT PRIMARY KEY, first_name VARCHAR(20), last_name VARCHAR(20), bank_balance DECIMAL(10,2), on_hand_balance DECIMAL(10,2));");
    } catch (sql::SQLException& e) {
        std::cout << "Creation of table failed. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
    
    // Insert customer information
    try {
        statement->execute("INSERT INTO customer_information VALUES(NULL,'John','Smith',213.73,510.67);");
    }
    catch (sql::SQLException& e) {
        std::cout << "Insertion into table failed. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    // Retreive balances from table
    long double bank_balance = 0;
    long double on_hand_balance = 0;
    std::string first_name, last_name;

    try {
        result_set = statement->executeQuery("SELECT first_name,last_name,on_hand_balance,bank_balance FROM customer_information WHERE customer_id = 1;");
    }
    catch (sql::SQLException& e) {
        std::cout << "Cound not retreive data. Error Message: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    while (result_set->next()) {
        first_name = result_set->getString("first_name");
        last_name = result_set->getString("last_name");
        bank_balance = result_set->getDouble("bank_balance");
        on_hand_balance = result_set->getDouble("on_hand_balance");
    }

    Customer* person = new Customer(first_name, last_name, bank_balance, on_hand_balance);
    
    // Introduction to customer
    std::cout << "Welcome to Earth Bank, " << first_name << " " << last_name << "!\n\n";
    std::cout << "Enter the one of the following numbers to be serviced:\n" ;
    std::cout << "(1) Balance\n";
    std::cout << "(0) Exit\n";

    std::vector<unsigned int> valid_service_nums = { 0, 1 };
    service_num = CheckNumberInput(valid_service_nums);

    std::cout << "\n";

    switch (service_num) {
    case 0:
        std::cout << "Goodbye\n";
        break;
    case 1:
        std::cout << "Balance\n";
        BalanceOptions(person, statement);
        break;
    default:
        std::cout << "Not a valid service number\n";
        break;
    }

    delete result_set;
    delete statement;
    delete con;
    return EXIT_SUCCESS;
}