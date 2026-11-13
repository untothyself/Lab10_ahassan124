/**
 * @file Lab10_ahassan124.cpp
 * @author Abass Hassan
 * @date 2026-11-10
 * @brief Runs a savings account program that can save account details to a file.
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <fstream>
#include <limits>
using namespace std;

class Customer
{
private:
    string customerID;
    string email;

public:
    /** @brief Creates a customer with default information. */
    Customer() : customerID("Unknown"), email("none@example.com") {}

    /**
     * @brief Creates a customer with their ID and email.
     * @param id The customer's ID.
     * @param emailAddr The customer's email address.
     */
    Customer(const string &id, const string &emailAddr)
        : customerID(id), email(emailAddr) {}

    /** @brief Gets the customer ID. @return The customer ID. */
    string getCustomerID() const { return customerID; }

    /** @brief Gets the email address. @return The email address. */
    string getEmail() const { return email; }

    /** @brief Displays the customer's information. @return Nothing. */
    void printCustomerInfo() const
    {
        cout << "Customer ID: " << customerID << endl;
        cout << "Email: " << email << endl;
    }
};

class Account
{
protected:
    string accountNumber;
    double balance;
    Customer owner;
    vector<double> deposits;
    vector<double> withdrawals;

    /**
     * @brief Prints the results after closing the business day.
     * @param previousBalance The balance before today's transactions.
     * @param totalDeposits The total deposited today.
     * @param totalWithdrawals The total withdrawn today.
     * @param newBalance The balance after processing everything.
     * @return Nothing.
     */
    void printSummary(double previousBalance, double totalDeposits,
                      double totalWithdrawals, double newBalance) const
    {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Previous Balance: $" << previousBalance << endl;
        cout << "Total Deposits: $" << totalDeposits << endl;
        cout << "Total Withdrawals: $" << totalWithdrawals << endl;
        cout << "New Balance after COB: $" << newBalance << endl;
    }

public:
    /** @brief Creates an empty account. */
    Account() : accountNumber("0000000000"), balance(0.0) {}

    /**
     * @brief Creates an account for a customer.
     * @param accNum The account number.
     * @param initialBalance The starting balance.
     * @param customer The account's owner.
     */
    Account(const string &accNum, double initialBalance, const Customer &customer)
        : accountNumber(accNum), balance(initialBalance), owner(customer) {}

    /** @brief Allows safe cleanup through an Account pointer. */
    virtual ~Account() = default;

    /**
     * @brief Adds a deposit to be processed at close of business.
     * @param amount The deposit amount.
     * @return True if the amount is positive.
     */
    bool deposit(double amount)
    {
        if (amount <= 0)
            return false;
        deposits.push_back(amount);
        return true;
    }

    /**
     * @brief Adds a withdrawal to be processed at close of business.
     * @param amount The withdrawal amount.
     * @return True if the amount is positive.
     */
    bool withdraw(double amount)
    {
        if (amount <= 0)
            return false;
        withdrawals.push_back(amount);
        return true;
    }

    /** @brief Applies transactions to a regular account. @return Nothing. */
    virtual void closeOfBusiness()
    {
        double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
        double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);
        double previousBalance = balance;
        balance += totalDeposits - totalWithdrawals;
        printSummary(previousBalance, totalDeposits, totalWithdrawals, balance);
        deposits.clear();
        withdrawals.clear();
    }

    /** @brief Gets the current balance. @return The account balance. */
    double getBalance() const { return balance; }

    /** @brief Gets the account number. @return The account number. */
    string getAccountNumber() const { return accountNumber; }

    /** @brief Gets the account owner. @return The customer object. */
    Customer getCustomer() const { return owner; }

    /** @brief Prints the regular account information. @return Nothing. */
    virtual void printAccountInfo() const
    {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Current Balance: $" << balance << endl;
        owner.printCustomerInfo();
    }

    /** @brief Shows pending deposits and withdrawals. @return Nothing. */
    void printTransactions() const
    {
        cout << "Deposits: ";
        for (double amount : deposits)
            cout << "$" << amount << " ";
        cout << endl
             << "Withdrawals: ";
        for (double amount : withdrawals)
            cout << "$" << amount << " ";
        cout << endl;
    }
};

class SavingsAccount : public Account
{
private:
    float interest;

public:
    /**
     * @brief Creates a savings account with a daily interest rate.
     * @param accNum The account number.
     * @param initialBalance The starting balance.
     * @param interestRate The daily interest rate as a decimal.
     * @param customer The account's owner.
     */
    SavingsAccount(const string &accNum, double initialBalance,
                   float interestRate, const Customer &customer)
        : Account(accNum, initialBalance, customer), interest(interestRate) {}

    /** @brief Processes transactions and adds interest if the balance is positive. @return Nothing. */
    void closeOfBusiness() override
    {
        double totalDeposits = accumulate(deposits.begin(), deposits.end(), 0.0);
        double totalWithdrawals = accumulate(withdrawals.begin(), withdrawals.end(), 0.0);
        double previousBalance = balance;

        // Process transactions before adding interest.
        balance += totalDeposits - totalWithdrawals;
        if (balance > 0)
        {
            balance += balance * interest;
        }

        printSummary(previousBalance, totalDeposits, totalWithdrawals, balance);
        deposits.clear();
        withdrawals.clear();
    }

    /** @brief Prints account details and the interest rate. @return Nothing. */
    void printAccountInfo() const override
    {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Current Balance: $" << balance << endl;
        cout << "Interest Rate: " << interest * 100 << "%" << endl;
        owner.printCustomerInfo();
    }

    /** @brief Saves the account number, balance, and interest to a text file. @return Nothing. */
    void Save() const
    {
        // Save the latest account details, replacing the old file.
        ofstream outFile("account_summary.txt");
        if (!outFile)
        {
            cout << "Could not save account_summary.txt." << endl;
            return;
        }

        outFile << "Account Number: " << accountNumber << endl;
        outFile << "Current Balance: $" << balance << endl;
        outFile << "Daily Interest Rate: " << interest << endl;
        outFile.close();

        cout << "Account information saved to account_summary.txt." << endl;
    }
};

const int DEPOSIT = 1;
const int WITHDRAWAL = 2;
const int CLOSE_OF_BUSINESS = 3;
const int SAVE = 4;
const int QUIT = 5;

/** @brief Displays the banking menu. @return Nothing. */
void printMenu()
{
    cout << "\nCSCC Bank and Trust" << endl;
    cout << "Select an option:" << endl;
    cout << "1. Make a deposit" << endl;
    cout << "2. Make a withdrawal" << endl;
    cout << "3. Close of Business" << endl;
    cout << "4. Save Account" << endl;
    cout << "5. Quit" << endl;
}

/** @brief Gets a valid menu selection. @return The chosen menu option. */
int getMenuOption()
{
    int choice = 0;
    do
    {
        printMenu();
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }
        if (choice < DEPOSIT || choice > QUIT)
        {
            cout << "Invalid option selected. Please try again." << endl;
        }
    } while (choice < DEPOSIT || choice > QUIT);
    return choice;
}

/**
 * @brief Gets a positive amount for a deposit or withdrawal.
 * @param prompt The message shown to the user.
 * @return The amount entered by the user.
 */
double getAmount(const char *prompt)
{
    double amount = 0;
    do
    {
        cout << prompt;
        if (!(cin >> amount))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            amount = 0;
        }
        if (amount <= 0)
        {
            cout << "The dollar amount must be greater than $0.00. Please try again." << endl;
        }
    } while (amount <= 0);
    return amount;
}

/** @brief Runs the banking program and saves account information. @return 0 on success. */
int main()
{
    Customer customer("0123456789", "customer@cscc.edu");

    // Keep a base pointer so we can demonstrate polymorphism.
    SavingsAccount *savingsAccount = new SavingsAccount(
        "5423678409", 100.00, 0.02f, customer);
    Account *account = savingsAccount;
    account->printAccountInfo();

    int choice = getMenuOption();
    while (choice != QUIT)
    {
        switch (choice)
        {
        case DEPOSIT:
            account->deposit(getAmount("Please enter the deposit amount: $"));
            break;
        case WITHDRAWAL:
            account->withdraw(getAmount("Please enter the withdrawal amount: $"));
            break;
        case CLOSE_OF_BUSINESS:
            account->closeOfBusiness();
            savingsAccount->Save(); // Save automatically after closing.
            break;
        case SAVE:
            savingsAccount->Save(); // Save without closing the day.
            break;
        }
        choice = getMenuOption();
    }

    // Account has a virtual destructor, so cleanup is safe.
    delete account;
    account = nullptr;
    return 0;
}
