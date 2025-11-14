#include <cstdio>
#include <cstdlib>

struct SimConfig;
void run_simulation(const SimConfig& cfg);

struct SimConfig {
    int numAccounts;
    int numThreads;
    int opsPerThread;
    long long initialPerAccount;
};

int main(int argc, char** argv) {
    SimConfig cfg;
    cfg.numAccounts       = (argc > 1) ? std::atoi(argv[1]) : 10;
    cfg.numThreads        = (argc > 2) ? std::atoi(argv[2]) : 8;
    cfg.opsPerThread      = (argc > 3) ? std::atoi(argv[3]) : 100000;
    cfg.initialPerAccount = (argc > 4) ? std::atoll(argv[4]) : 10000;

    run_simulation(cfg);
    return 0;
}
