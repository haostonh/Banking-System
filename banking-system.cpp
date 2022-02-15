#include<iostream>

class Customer {
  public:
    void DepositMoney(float money) {
      bank_balance += money;
      on_hand_balance -= money;
    }
    void GetBalance() {std::cout << bank_balance << std::endl;}
    void WithdrawMoney(float money) {
      bank_balance -= money;
      on_hand_balance += money;
    }
  private:
    float bank_balance, on_hand_balance;
};

int main() {
  std::cout << "Welcome to Earth Bank!" << std::endl << std::endl;
  std::cout << "Enter the one of the following numbers to be serviced:" std::endl;
  std::cout << "(1) Balance" << std::endl;
  std::cout << "(0) Exit" std::endl;

  return 0;
}