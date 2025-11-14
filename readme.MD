# Concurrent Bank Account Simulator

A small C++17 project that simulates a bank with multiple accounts being hit by many concurrent deposits, withdrawals and transfers, and then verifies that no money is accidentally created or lost.

---

## Overview

This project models a tiny “toy bank” where several worker threads randomly operate on a shared set of bank accounts.  
At the end of the run, the program checks that the total balance across all accounts matches the mathematically expected amount, acting as a simple consistency stress‑test.

Key ideas:

- Each account is wrapped in a thread‑safe `BankAccount` class.  
- Multiple threads perform random operations on these accounts.  
- The simulator tracks how much money was deposited and withdrawn to compute the expected final total.  
- The final sum of all balances must equal this expected total, or the run is flagged as inconsistent.

---

## Features

- Thread‑safe `BankAccount` with deposit, withdraw and transfer operations using mutexes.  
- Randomized workload of deposits, withdrawals and account‑to‑account transfers.  
- Multi‑threaded transaction simulation using the C++ standard threading library.  
- Consistency check comparing final balances against the expected net total.  
- Simple command‑line interface for configuring number of accounts, threads and operations.

---
