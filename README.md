# Multi-Tier Sacco Transit Payment System

A C++ console application simulating a real-world public transport fare collection system for a Kenyan Sacco (matatu cooperative). The system models a three-tier architecture — from passenger interaction all the way to corporate treasury settlement — with full support for cash and digital wallet payments.

---

## System Architecture

The project is structured around **3 logical tiers**, each with a distinct responsibility:

| Tier | Class | Role |
|------|-------|------|
| **Tier 1** | `PassengerTerminalCLI` | Passenger-facing UI — ticket printing & interaction |
| **Tier 2** | `InBusSmartVault` | In-bus validation engine — fare processing & local ledger |
| **Tier 3** | `SaccoCorporateTreasury` | Central HQ — fleet-wide settlement & audit logging |

---

## Features

-  **Fixed fare route matrix** with predefined Juja / Ruiru / Nairobi CBD routes
-  **Cash payment** with automatic change calculation
-  **Digital wallet payment** with real-time balance deduction
-  **Ticket receipts** with unique reference numbers
-  **Live bus ledger** showing all transactions per trip
-  **End-of-route settlement** that syncs vault funds to the corporate treasury
-  **Corporate audit logs** tracking all fleet settlements

---

##  OOP Concepts Demonstrated

This project is a practical demonstration of all **four pillars of Object-Oriented Programming**:

###  Encapsulation
All internal state in `InBusSmartVault` (vault balance, ledger, route matrix) is kept `private` and accessed only through public methods.

###  Inheritance
`CashPayment` and `DigitalPayment` both inherit from the abstract base class `IPaymentMethod`.

###  Polymorphism
Both payment classes override `processPayment()`, allowing the vault to handle different payment types through a single `IPaymentMethod*` pointer.

###  Abstraction
`IPaymentMethod` defines a pure virtual interface, hiding implementation details from the calling code.

---

##  Project Structure

```
sacco-payment-system/
│
├── main.cpp          # Full source code (single-file project)
└── README.md         # Project documentation
```

---

## Getting Started

### Prerequisites
- A C++ compiler supporting **C++11** or later (e.g. `g++`, `clang++`)

### Compile & Run

```bash
g++ -std=c++11 -o sacco_system main.cpp
./sacco_system
```

---

## Usage

On launch, you are presented with an interactive console menu:

```
=== MAIN CONTROL TERMINAL CONSOLE ===
1. View Route fare matrix (Tier 1 Display)
2. Pay Fare using CASH
3. Pay Fare using DIGITAL WALLET
4. View Current Bus Vault Logs (Tier 2 Ledger)
5. End Route & Sync to Sacco HQ (Tier 3 Settlement)
6. Shut Down Project System Application
```

### Example Workflow
1. Select **Option 1** to view available routes and fares
2. Select **Option 2** or **3** to process a passenger payment
3. Select **Option 4** to inspect the bus ledger and vault balance
4. Select **Option 5** to settle funds to the Sacco treasury
5. Select **Option 6** to exit

---

## Available Routes

| Route ID | From | To | Fixed Fare |
|----------|------|----|------------|
| 1 | Juja | Ruiru | KES 50 |
| 2 | Juja | Nairobi CBD | KES 100 |
| 3 | Ruiru | Nairobi CBD | KES 70 |

---

## Demo Passengers (Digital Wallets)

| Wallet ID | Name | Starting Balance |
|-----------|------|-----------------|
| W-1 | John Mwangi | KES 80.00 |
| W-2 | Grace Otieno | KES 350.00 |

---

## Sacco Details (Hardcoded Demo)

| Field | Value |
|-------|-------|
| Sacco Name | Super Metro Sacco Ltd |
| Initial Treasury Reserve | KES 500,000.00 |
| Active Bus Plate | KDK 888X |

---
