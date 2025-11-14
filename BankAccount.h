#pragma once
#include <mutex>
#include <cstdint>

class BankAccount {
public:
    using amount_t = long long;

    explicit BankAccount(int id, amount_t initial = 0);

    // Single-account operations: lock only this->mtx_
    void deposit(amount_t amt);
    // Returns false if insufficient funds (no change made)
    bool withdraw(amount_t amt);
    amount_t balance() const;

    int id() const { return id_; }

    // Deadlock-free transfer between two different accounts.
    // Returns true if withdrawn from 'from' and deposited to 'to'.
    static bool transfer(BankAccount& from, BankAccount& to, amount_t amt);

private:
    int id_;
    amount_t bal_;
    mutable std::mutex mtx_;
};
