#include<iostream>

class Customer {
  public:
    void DepositMoney(float money) {
      balance += money;
      on_hand_balance -= money;
    }
    void GetBalance() {std::cout << balance << std::endl;}
    void WithdrawMoney(float money) {
      balance -= money;
      on_hand_balance += money;
    }
  private:
    float bank_balance, on_hand_balance;
};

int main() {
 return 0;
}