#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// ============================================================================
// DATA MODELS / STRUCTURES
// ============================================================================

struct Route {
    string routeID;
    string startLocation;
    string endLocation;
    double fixedFare;
};

struct TransactionReceipt {
    int ticketNumber;
    string routeDescription;
    double amountCharged;
    double amountPaid;
    double changeGiven;
    string paymentType;
};

struct PassengerWallet {
    string walletID;
    string passengerName;
    double remainingBalance;
};


// ============================================================================
// ABSTRACTION PILLAR: Abstract Base Class for Payment Methods
// ============================================================================
class IPaymentMethod {
public:
    virtual bool processPayment(double fare, TransactionReceipt& receipt, double& localVault) = 0;
    virtual string getPaymentType() = 0;
    virtual double getAmountPaid() = 0;
    virtual ~IPaymentMethod() {}
};


// ============================================================================
// INHERITANCE + POLYMORPHISM: Cash Payment Implementation
// ============================================================================
class CashPayment : public IPaymentMethod {
private:
    double cashGiven;
    double amountPaid;
    string paymentType;

public:
    CashPayment(double cash) : cashGiven(cash), amountPaid(0.0), paymentType("CASH") {}

    bool processPayment(double fare, TransactionReceipt& receipt, double& localVault) override {
        if (cashGiven < fare) {
            return false;
        }

        amountPaid = cashGiven;
        double changeDue = cashGiven - fare;
        localVault += fare;

        receipt.changeGiven = changeDue;
        receipt.amountPaid = cashGiven;
        receipt.amountCharged = fare;

        return true;
    }

    string getPaymentType() override {
        return paymentType;
    }

    double getAmountPaid() override {
        return amountPaid;
    }
};


// ============================================================================
// INHERITANCE + POLYMORPHISM: Digital Payment Implementation
// ============================================================================
class DigitalPayment : public IPaymentMethod {
private:
    PassengerWallet& wallet;
    double amountPaid;
    string paymentType;

public:
    DigitalPayment(PassengerWallet& w) : wallet(w), amountPaid(0.0), paymentType("DIGITAL (" + w.passengerName + ")") {}

    bool processPayment(double fare, TransactionReceipt& receipt, double& localVault) override {
        if (wallet.remainingBalance < fare) {
            return false;
        }

        wallet.remainingBalance -= fare;
        localVault += fare;
        amountPaid = fare;

        receipt.changeGiven = 0.0;
        receipt.amountPaid = fare;
        receipt.amountCharged = fare;

        return true;
    }

    string getPaymentType() override {
        return paymentType;
    }

    double getAmountPaid() override {
        return amountPaid;
    }
};


// ============================================================================
// TIER 3: CENTRAL SACCO CORPORATE TREASURY (Enterprise Ledger Layer)
// ============================================================================
class SaccoCorporateTreasury {
private:
    string saccoName;
    double totalBankReserveFunds;
    int totalFleetTripsSettled;
    vector<string> corporateAuditLogs;

public:
    SaccoCorporateTreasury(string name, double initialReserve) {
        saccoName = name;
        totalBankReserveFunds = initialReserve;
        totalFleetTripsSettled = 0;
        corporateAuditLogs.push_back("Treasury initialized with KES " + to_string(initialReserve));
    }

    void receiveRouteSettlement(string vehiclePlate, double settlementAmount) {
        totalBankReserveFunds += settlementAmount;
        totalFleetTripsSettled++;
        string logEntry = "Settlement from Bus [" + vehiclePlate + "] | Amount: KES " + to_string(settlementAmount);
        corporateAuditLogs.push_back(logEntry);
    }

    void printTreasuryReport() {
        cout << "\n======================================================\n";
        cout << "        CORPORATE TREASURY FINANCIAL REPORT          \n";
        cout << "======================================================\n";
        cout << "Sacco Name             : " << saccoName << "\n";
        cout << "Total Bank Reserves    : KES " << fixed << setprecision(2) << totalBankReserveFunds << "\n";
        cout << "Total Trips Settled    : " << totalFleetTripsSettled << "\n";
        cout << "------------------------------------------------------\n";
        cout << "Corporate Audit Logs:\n";
        for (size_t i = 0; i < corporateAuditLogs.size(); ++i) {
            cout << " [" << i + 1 << "] " << corporateAuditLogs[i] << "\n";
        }
        cout << "======================================================\n";
    }
};


// ============================================================================
// TIER 2: IN-BUS SECURE VAULT ENGINE (Validation & Local Caching Layer)
// ============================================================================
class InBusSmartVault {
private:  // ENCAPSULATION: All data members are private
    string plateNumber;
    double localCashVault;
    int receiptCounter;
    vector<Route> routeMatrix;
    vector<TransactionReceipt> localTripLedger;

public:
    InBusSmartVault(string plate) {
        plateNumber = plate;
        localCashVault = 0.0;
        receiptCounter = 1000;
        initializeFareMatrix();
    }

    void initializeFareMatrix() {
        routeMatrix.push_back({"1", "Juja", "Ruiru", 50.0});
        routeMatrix.push_back({"2", "Juja", "Nairobi_CBD", 100.0});
        routeMatrix.push_back({"3", "Ruiru", "Nairobi_CBD", 70.0});
    }

    void displayRoutes() {
        cout << "\n--- AVAILABLE OFFICIAL SACCO ROUTES ---\n";
        cout << left << setw(8) << "ID" << setw(15) << "From" << setw(15) << "To" << "Fixed Fare\n";
        cout << "------------------------------------------------------\n";
        for (size_t i = 0; i < routeMatrix.size(); ++i) {
            cout << left << setw(8) << routeMatrix[i].routeID
                 << setw(15) << routeMatrix[i].startLocation
                 << setw(15) << routeMatrix[i].endLocation
                 << "KES " << routeMatrix[i].fixedFare << "\n";
        }
    }

    bool processCashPayment(string routeChoice, double cashGiven, TransactionReceipt &outReceipt) {
        Route selectedRoute;
        bool routeFound = false;

        for (size_t i = 0; i < routeMatrix.size(); ++i) {
            if (routeMatrix[i].routeID == routeChoice) {
                selectedRoute = routeMatrix[i];
                routeFound = true;
                break;
            }
        }

        if (!routeFound) {
            cout << "\n>> ERROR: Invalid Route ID selected!\n";
            return false;
        }

        // POLYMORPHISM: Using the abstract interface
        IPaymentMethod* payment = new CashPayment(cashGiven);

        if (!payment->processPayment(selectedRoute.fixedFare, outReceipt, localCashVault)) {
            cout << "\n>> TRANSACTION REJECTED: Insufficient cash! Required: KES " << selectedRoute.fixedFare << "\n";
            delete payment;
            return false;
        }

        receiptCounter++;
        outReceipt.ticketNumber = receiptCounter;
        outReceipt.routeDescription = selectedRoute.startLocation + " to " + selectedRoute.endLocation;
        outReceipt.paymentType = payment->getPaymentType();
        outReceipt.amountCharged = selectedRoute.fixedFare;

        localTripLedger.push_back(outReceipt);

        delete payment;
        return true;
    }

    bool processDigitalPayment(string routeChoice, PassengerWallet &wallet, TransactionReceipt &outReceipt) {
        Route selectedRoute;
        bool routeFound = false;

        for (size_t i = 0; i < routeMatrix.size(); ++i) {
            if (routeMatrix[i].routeID == routeChoice) {
                selectedRoute = routeMatrix[i];
                routeFound = true;
                break;
            }
        }

        if (!routeFound) {
            cout << "\n>> ERROR: Invalid Route ID selected!\n";
            return false;
        }

        // POLYMORPHISM: Using the abstract interface with different implementation
        IPaymentMethod* payment = new DigitalPayment(wallet);

        if (!payment->processPayment(selectedRoute.fixedFare, outReceipt, localCashVault)) {
            cout << "\n>> TRANSACTION REJECTED: Low balance inside " << wallet.walletID
                 << "! Balance: KES " << wallet.remainingBalance << "\n";
            delete payment;
            return false;
        }

        receiptCounter++;
        outReceipt.ticketNumber = receiptCounter;
        outReceipt.routeDescription = selectedRoute.startLocation + " to " + selectedRoute.endLocation;
        outReceipt.paymentType = payment->getPaymentType();
        outReceipt.amountCharged = selectedRoute.fixedFare;

        localTripLedger.push_back(outReceipt);

        delete payment;
        return true;
    }

    void dispatchEndRouteSettlement(SaccoCorporateTreasury &corporateHQ) {
        if (localCashVault == 0.0) {
            cout << "\n>> Notice: Local vault is empty. Nothing to sync.\n";
            return;
        }
        cout << "\n[ROUTE TERMINATION] Connecting to Sacco Bank Server via network...\n";
        corporateHQ.receiveRouteSettlement(plateNumber, localCashVault);
        localCashVault = 0.0;
        localTripLedger.clear();
        cout << ">> Sync Complete! Local bus storage safely flushed.\n";
    }

    void printCurrentTripLogs() {
        cout << "\n------------------------------------------------------\n";
        cout << "          LIVE BUS LEDGER LOGS [PLATE: " << plateNumber << "]\n";
        cout << "------------------------------------------------------\n";
        cout << "Current Vault Cash Balance: KES " << localCashVault << "\n";
        cout << "Active Trip Tickets Issued: " << localTripLedger.size() << "\n\n";

        for (size_t i = 0; i < localTripLedger.size(); ++i) {
            cout << " Ticket #" << localTripLedger[i].ticketNumber << " | "
                 << localTripLedger[i].routeDescription << "\n"
                 << " Charged: KES " << localTripLedger[i].amountCharged
                 << " | Paid: KES " << localTripLedger[i].amountPaid
                 << " | Change: KES " << localTripLedger[i].changeGiven << "\n"
                 << " Method: " << localTripLedger[i].paymentType << "\n"
                 << " ..................................................\n";
        }
        cout << "------------------------------------------------------\n";
    }
};


// ============================================================================
// TIER 1: PASSENGER APPLICATION INTERFACE (User Interaction Simulation)
// ============================================================================
class PassengerTerminalCLI {
public:
    void printTicketReceipt(TransactionReceipt receipt) {
        cout << "\n========================================\n";
        cout << "         OFFICIAL SACCO TICKET          \n";
        cout << "========================================\n";
        cout << " Ticket Reference : TC-" << receipt.ticketNumber << "\n";
        cout << " Journey Trip     : " << receipt.routeDescription << "\n";
        cout << " Fare Charged     : KES " << receipt.amountCharged << "\n";
        cout << " Amount Rendered  : KES " << receipt.amountPaid << "\n";
        cout << " Balance Owed     : KES " << receipt.changeGiven << "\n";
        cout << " Payment Status   : VERIFIED VIA TIER 2\n";
        cout << "========================================\n";
        cout << ">> Safe travels! Step inside the vehicle.\n";
    }
};


// ============================================================================
// MAIN INTERACTIVE SIMULATION LOOP
// ============================================================================
int main() {
    SaccoCorporateTreasury superMetroHQ("Super Metro Sacco Ltd", 500000.00);
    InBusSmartVault activeMatatuVault("KDK 888X");
    PassengerTerminalCLI passengerTerminal;

    // Set up our dummy passengers for the wallet test cases
    PassengerWallet johnWallet = {"W-1", "John Mwangi", 80.00};
    PassengerWallet graceWallet = {"W-2", "Grace Otieno", 350.00};

    int menuChoice = 0;

    cout << "============================================================\n";
    cout << "   INTERACTIVE MULTI-TIER SYSTEM STARTING...                \n";
    cout << "============================================================\n";

    // Continuous interactive program loop
    while (menuChoice != 6) {
        cout << "\n=== MAIN CONTROL TERMINAL CONSOLE ===\n";
        cout << "1. View Route fare matrix (Tier 1 Display)\n";
        cout << "2. Pay Fare using CASH\n";
        cout << "3. Pay Fare using DIGITAL WALLET\n";
        cout << "4. View Current Bus Vault Logs (Tier 2 Ledger)\n";
        cout << "5. End Route & Sync to Sacco HQ (Tier 3 Settlement)\n";
        cout << "6. Shut Down Project System Application\n";
        cout << "Select operational option [1-6]: ";

        cin >> menuChoice;

        // Simple error cleaning loop if user types a letter instead of a number
        if(cin.fail()) {
            cin.clear();
            string dummy;
            cin >> dummy;
            cout << "Invalid typing! Please type a numeric choice.\n";
            continue;
        }

        TransactionReceipt tempReceipt;
        string chosenRoute;

        if (menuChoice == 1) {
            activeMatatuVault.displayRoutes();
        }
        else if (menuChoice == 2) {
            double cashInput;
            activeMatatuVault.displayRoutes();
            cout << "\nEnter Route ID choice: ";
            cin >> chosenRoute;
            cout << "Enter cash amount handed over (KES): ";
            cin >> cashInput;

            if (activeMatatuVault.processCashPayment(chosenRoute, cashInput, tempReceipt)) {
                passengerTerminal.printTicketReceipt(tempReceipt);
            }
        }
        else if (menuChoice == 3) {
            int identityChoice;
            activeMatatuVault.displayRoutes();
            cout << "\nEnter Route ID choice: ";
            cin >> chosenRoute;
            cout << "Select wallet (1=John Mwangi, 2=Grace Otieno): ";
            cin >> identityChoice;

            if (identityChoice == 1) {
                if (activeMatatuVault.processDigitalPayment(chosenRoute, johnWallet, tempReceipt)) {
                    passengerTerminal.printTicketReceipt(tempReceipt);
                }
            } else if (identityChoice == 2) {
                if (activeMatatuVault.processDigitalPayment(chosenRoute, graceWallet, tempReceipt)) {
                    passengerTerminal.printTicketReceipt(tempReceipt);
                }
            } else {
                cout << "Unknown user card profile!\n";
            }
        }
        else if (menuChoice == 4) {
            activeMatatuVault.printCurrentTripLogs();
        }
        else if (menuChoice == 5) {
            // Displays status balances before and after your explicit command triggers
            superMetroHQ.printTreasuryReport();
            activeMatatuVault.dispatchEndRouteSettlement(superMetroHQ);
            superMetroHQ.printTreasuryReport();
        }
        else if (menuChoice == 6) {
            cout << "\nSystem application safely powering down...\n";
        }
        else {
            cout << "Selection out of bounds! Choose between numbers 1 and 6.\n";
        }
    }

    return 0;
}
