#include "User.h"

User::User(double cash): cash(cash) {}

double User::getBalance() {
    std::lock_guard<std::mutex> lock(mutex);
    return cash;
}
