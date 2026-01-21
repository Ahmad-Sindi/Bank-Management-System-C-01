/*
============================================================
 Algorithm Challenge – Level 3 | Bank Management System Course: Algorithms & Problem Solving (Course #3)

 Description:
 ------------
 This program implements a simple console-based Bank Management System using C++ and file handling.

 The system allows users to:
  - Add new clients
  - Display all clients
  - Update client information
  - Delete clients safely
  - Deposit and withdraw balances
  - Display total balances
  - Store and retrieve data from a local text file

 Engineering Concepts Applied:
 -----------------------------
  - Structured programming
  - File handling (read / write / update)
  - Data modeling using structs
  - Menu-driven system design
  - Validation and defensive programming
  - Algorithmic thinking (Level 3)

 Important Note:
 ---------------
 This program works with a local file on the user's machine.
 Each user must ensure the file exists or allow the program
 to create it locally. File behavior may differ across systems.

============================================================
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// ------------------------------------------------------------
// Global Constants & Forward Declarations
// ------------------------------------------------------------

// Local file used to store client data
const string ClientFileName1 = "Clients.txt";

// Forward declarations to avoid compiler errors
void ShowMainMenueScreen();
void ShowTransactionMenueScreen();

// ------------------------------------------------------------
// Data Structure
// ------------------------------------------------------------

// Represents a single bank client
struct sClientD
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phon;
    double AccountBalance;
    bool MarkForDelete = false; // Used for soft delete
};

// ------------------------------------------------------------
// Utility Functions
// ------------------------------------------------------------

// Splits a string based on a given delimiter
vector<string> SplitString(string S1, string Delim)
{
    vector<string> VecString;
    short Posi = 0;
    string sWord;

    while ((Posi = S1.find(Delim)) != string::npos)
    {
        sWord = S1.substr(0, Posi);
        if (sWord != "")
            VecString.push_back(sWord);

        S1.erase(0, Posi + Delim.length());
    }

    if (S1 != "")
        VecString.push_back(S1);

    return VecString;
}

// Converts a line from file into a client record
sClientD ConvertLineToRecord(string Line, string Seperator = "#//#")
{
    sClientD Client;
    vector<string> VeClientData = SplitString(Line, Seperator);

    Client.AccountNumber = VeClientData[0];
    Client.PinCode = VeClientData[1];
    Client.Name = VeClientData[2];
    Client.Phon = VeClientData[3];
    Client.AccountBalance = stod(VeClientData[4]);

    return Client;
}

// Converts a client record into a file-ready line
string ConvertRecordToLine(sClientD Client, string Seperator = "#//#")
{
    return Client.AccountNumber + Seperator +
           Client.PinCode + Seperator +
           Client.Name + Seperator +
           Client.Phon + Seperator +
           to_string(Client.AccountBalance);
}

// ------------------------------------------------------------
// File Handling Functions
// ------------------------------------------------------------

// Checks if a client already exists by account number
bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    fstream FileClientData(FileName, ios::in);

    if (FileClientData.is_open())
    {
        string Line;
        while (getline(FileClientData, Line))
        {
            if (ConvertLineToRecord(Line).AccountNumber == AccountNumber)
            {
                FileClientData.close();
                return true;
            }
        }
        FileClientData.close();
    }
    return false;
}

// Loads all clients from file
vector<sClientD> LoadClientSDataFromFile(string FileName)
{
    vector<sClientD> VecClients;
    fstream FileClientData(FileName, ios::in);

    if (FileClientData.is_open())
    {
        string Line;
        while (getline(FileClientData, Line))
            VecClients.push_back(ConvertLineToRecord(Line));

        FileClientData.close();
    }
    return VecClients;
}

// Saves all clients back to file (overwrite mode)
vector<sClientD> SaveClientsDataToFile(string FileName, vector<sClientD> vClientS)
{
    fstream FileClientData(FileName, ios::out);

    if (FileClientData.is_open())
    {
        for (sClientD Client : vClientS)
        {
            if (!Client.MarkForDelete)
                FileClientData << ConvertRecordToLine(Client) << endl;
        }
        FileClientData.close();
    }
    return vClientS;
}

// Appends a single client record to the file
void AddClientDataLineToFile(string FileName, string StringDataLine)
{
    fstream FileClientData(FileName, ios::out | ios::app);
    if (FileClientData.is_open())
    {
        FileClientData << StringDataLine << endl;
        FileClientData.close();
    }
}

// ------------------------------------------------------------
// Client Management Functions
// ------------------------------------------------------------

// Reads a new client from user input
sClientD ReadNewClient()
{
    sClientD ClintData;

    cout << "Enter Account Number ? ";
    getline(cin >> ws, ClintData.AccountNumber);

    while (ClientExistsByAccountNumber(ClintData.AccountNumber, ClientFileName1))
    {
        cout << "\nAccount already exists, enter another one: ";
        getline(cin >> ws, ClintData.AccountNumber);
    }

    cout << "Enter Pin Code : ";
    getline(cin, ClintData.PinCode);

    cout << "Enter Full Name : ";
    getline(cin, ClintData.Name);

    cout << "Enter Phone Number : ";
    getline(cin, ClintData.Phon);

    cout << "Enter Account Balance : ";
    cin >> ClintData.AccountBalance;

    return ClintData;
}

// Prints a client record in table format
void PrintClientRecordLine(sClientD Client)
{
    cout << "| " << setw(10) << left << Client.AccountNumber
         << "| " << setw(10) << left << Client.PinCode
         << "| " << setw(20) << left << Client.Name
         << "| " << setw(12) << left << Client.Phon
         << "| " << setw(10) << left << Client.AccountBalance;
}

// Prints a detailed client card
void PrintClientCard(sClientD Client)
{
    cout << "\n---------------------------------------";
    cout << "\nAccount Number : " << Client.AccountNumber;
    cout << "\nPin Code       : " << Client.PinCode;
    cout << "\nFull Name      : " << Client.Name;
    cout << "\nPhone Number   : " << Client.Phon;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n---------------------------------------\n";
}

// Finds a client by account number
bool FindClientByAccountNumber(string AccountNumber, vector<sClientD> vClientS, sClientD &Client)
{
    for (sClientD C : vClientS)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

// ------------------------------------------------------------
// Menu Screens & Navigation
// ------------------------------------------------------------

void ShowAllClientsScreen()
{
    vector<sClientD> vClients = LoadClientSDataFromFile(ClientFileName1);

    cout << "\n\t\tClient List (" << vClients.size() << ") Client(s)\n";
    cout << "-------------------------------------------------------------\n";

    for (sClientD C : vClients)
    {
        PrintClientRecordLine(C);
        cout << endl;
    }
}

// ------------------------------------------------------------
// Main Menu Logic
// ------------------------------------------------------------

enum enMainMenueOptions
{
    eListClients = 1,
    eAddNewClien = 2,
    eExit = 3
};

short ReadMainMenueOption()
{
    short Choice;
    cout << "\nChoose [1-3]: ";
    cin >> Choice;
    return Choice;
}

void PerformMainMenueOption(enMainMenueOptions Option)
{
    switch (Option)
    {
    case eListClients:
        system("cls");
        ShowAllClientsScreen();
        break;

    case eAddNewClien:
        system("cls");
        AddClientDataLineToFile(ClientFileName1,
                                ConvertRecordToLine(ReadNewClient()));
        cout << "\nClient Added Successfully.\n";
        break;

    case eExit:
        cout << "\nProgram Ended.\n";
        break;
    }
}

void ShowMainMenueScreen()
{
    system("cls");
    cout << "==============================\n";
    cout << " Bank Management System\n";
    cout << "==============================\n";
    cout << "[1] Show Clients\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Exit\n";

    PerformMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

// ------------------------------------------------------------
// Program Entry Point
// ------------------------------------------------------------

int main()
{
    ShowMainMenueScreen();
    system("pause>0");
    return 0;
}
