#include "BankAccount.h"
#include <vector>
#include <thread>
#include <random>
#include <atomic>
#include <memory>  

struct SimConfig {
    int numAccounts;
    int numThreads;
    int opsPerThread;
    BankAccount::amount_t initialPerAccount;
};

static std::atomic<long long> totalDeposited{0};
static std::atomic<long long> totalWithdrawn{0};

static void worker(std::vector<std::unique_ptr<BankAccount>>* accounts,
                   int opsPerThread,
                   unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> accDist(0, static_cast<int>(accounts->size()) - 1);
    std::uniform_int_distribution<int> opDist(0, 99);
    std::uniform_int_distribution<int> amtDist(1, 1000);

    for (int i = 0; i < opsPerThread; ++i) {
        int opPick = opDist(rng);
        int idxA = accDist(rng);
        int idxB = accDist(rng);
        auto& A = *accounts->at(idxA);  
        auto& B = *accounts->at(idxB);
        BankAccount::amount_t amt = amtDist(rng);

        if (opPick < 30) {
            A.deposit(amt);
            totalDeposited.fetch_add(amt, std::memory_order_relaxed);
        } else if (opPick < 60) {
            if (A.withdraw(amt)) {
                totalWithdrawn.fetch_add(amt, std::memory_order_relaxed);
            }
        } else {
            if (idxA != idxB) {
                (void)BankAccount::transfer(A, B, amt);
            }
        }
    }
}

void run_simulation(const SimConfig& cfg) {
    std::vector<std::unique_ptr<BankAccount>> accounts;
    accounts.reserve(cfg.numAccounts);  // Now safe - pointers can move
    for (int i = 0; i < cfg.numAccounts; ++i) {
        accounts.push_back(std::make_unique<BankAccount>(i, cfg.initialPerAccount));
    }

    long long initialTotal = 1LL * cfg.numAccounts * cfg.initialPerAccount;

    std::vector<std::thread> threads;
    threads.reserve(cfg.numThreads);
    std::random_device rd;
    for (int t = 0; t < cfg.numThreads; ++t) {
        threads.emplace_back(worker, &accounts, cfg.opsPerThread,
                             rd() ^ (std::mt19937::result_type(t) << 16));
    }
    for (auto& th : threads) th.join();

    long long finalTotal = 0;
    for (auto& a : accounts) finalTotal += a->balance();  // Dereference pointer

    long long net = initialTotal + totalDeposited.load() - totalWithdrawn.load();

    printf("Initial total: %lld\n", initialTotal);
    printf("Deposited:     %lld\n", totalDeposited.load());
    printf("Withdrawn:     %lld\n", totalWithdrawn.load());
    printf("Expected:      %lld\n", net);
    printf("Final total:   %lld\n", finalTotal);
    printf("CONSISTENT:    %s\n", (finalTotal == net ? "YES" : "NO"));
}
