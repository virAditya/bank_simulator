#include "BankAccount.h"
#include <algorithm>
#include <mutex>

BankAccount::BankAccount(int id, amount_t initial) : id_(id), bal_(initial) {}

void BankAccount::deposit(amount_t amt) {
    std::lock_guard<std::mutex> lk(mtx_);
    bal_ += amt;
}

bool BankAccount::withdraw(amount_t amt) {
    std::lock_guard<std::mutex> lk(mtx_);
    if (bal_ < amt) return false;
    bal_ -= amt;
    return true;
}

BankAccount::amount_t BankAccount::balance() const {
    std::lock_guard<std::mutex> lk(mtx_);
    return bal_;
}

bool BankAccount::transfer(BankAccount& from, BankAccount& to, amount_t amt) {
    if (&from == &to) return false;

    // Deadlock-free: lock both with a single scoped_lock (uses std::lock internally)
    std::scoped_lock lock(from.mtx_, to.mtx_);

    if (from.bal_ < amt) return false;
    from.bal_ -= amt;
    to.bal_   += amt;
    return true;
}
