// test_patient_bst.cpp
// Complete test program for Patient Records BST Module

#include "PatientRecordsBST.h"
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

// Utility function to get current time as string
string getCurrentTime() {
    time_t now = time(0);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    return string(buffer);
}

// Display menu
void displayMenu() {
    cout << "\n========== PATIENT RECORDS BST - TEST MENU ==========\n";
    cout << "1.  Add New Patient\n";
    cout << "2.  Search Patient by ID\n";
    cout << "3.  Delete Patient\n";
    cout << "4.  Update Patient Information\n";
    cout << "5.  Display All Patients\n";
    cout << "6.  Display Patients by Priority\n";
    cout << "7.  Display Statistics\n";
    cout << "8.  Save Records to File\n";
    cout << "9.  Load Records from File\n";
    cout << "10. Add Sample Test Data\n";
    cout << "11. Clear All Records\n";
    cout << "12. 🌳 VISUALIZE TREE STRUCTURE (Proof it's a BST!)\n";
    cout << "13. 🔍 VERIFY BST PROPERTIES\n";
    cout << "0.  Exit\n";
    cout << "====================================================\n";
    cout << "Enter your choice: ";
}

// Add sample data for testing
void addSampleData(PatientRecordsBST& bst) {
    PatientData patients[] = {
        {1001, "John Smith", 45, "Chest Pain, Difficulty Breathing", 1, getCurrentTime()},
        {1002, "Emily Johnson", 28, "High Fever, Cough", 2, getCurrentTime()},
        {1003, "Michael Brown", 62, "Minor Cut, Needs Stitches", 3, getCurrentTime()},
        {1004, "Sarah Davis", 35, "Severe Headache, Dizziness", 2, getCurrentTime()},
        {1005, "Robert Wilson", 71, "Heart Attack Symptoms", 1, getCurrentTime()},
        {1006, "Lisa Anderson", 23, "Ankle Sprain", 3, getCurrentTime()},
        {1007, "David Martinez", 50, "Broken Arm", 2, getCurrentTime()},
        {1008, "Jennifer Taylor", 19, "Routine Checkup", 3, getCurrentTime()},
        {1009, "William Thomas", 55, "Stroke Symptoms", 1, getCurrentTime()},
        {1010, "Mary Garcia", 40, "Allergic Reaction", 2, getCurrentTime()}
    };
    
    for (const auto& patient : patients) {
        bst.insertPatient(patient);
    }
    
    cout << "\n✓ Successfully added 10 sample patients!\n";
}

// Display all patients in formatted table
void displayAllPatients(PatientRecordsBST& bst) {
    vector<PatientData> patients = bst.getAllPatients();
    
    if (patients.empty()) {
        cout << "\nNo patients in the system.\n";
        return;
    }
    
    cout << "\n========== ALL PATIENT RECORDS (Sorted by ID) ==========\n";
    cout << left << setw(8) << "ID" 
         << setw(20) << "Name" 
         << setw(6) << "Age" 
         << setw(12) << "Priority"
         << "Symptoms\n";
    cout << string(80, '-') << endl;
    
    for (const auto& p : patients) {
        string priority;
        switch(p.priorityLevel) {
            case 1: priority = "Critical"; break;
            case 2: priority = "Urgent"; break;
            case 3: priority = "Standard"; break;
            default: priority = "Unknown";
        }
        
        cout << left << setw(8) << p.patientID
             << setw(20) << p.name
             << setw(6) << p.age
             << setw(12) << priority
             << p.symptoms << endl;
    }
    cout << string(80, '=') << endl;
}

// Display statistics
void displayStatistics(PatientRecordsBST& bst) {
    cout << "\n========== PATIENT RECORDS STATISTICS ==========\n";
    cout << "Total Patients: " << bst.getTotalPatients() << endl;
    cout << "Critical (Priority 1): " << bst.getPatientCountByPriority(1) << endl;
    cout << "Urgent (Priority 2): " << bst.getPatientCountByPriority(2) << endl;
    cout << "Standard (Priority 3): " << bst.getPatientCountByPriority(3) << endl;
    cout << "Tree Height: " << bst.getTreeHeight() << endl;
    cout << "================================================\n";
}

int main() {
    PatientRecordsBST patientRecords;
    int choice;
    
    cout << "========================================\n";
    cout << "  PATIENT RECORDS BST - MODULE 2\n";
    cout << "  Hospital Emergency Management System\n";
    cout << "========================================\n";
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline from buffer
        
        switch(choice) {
            case 1: { // Add New Patient
                PatientData newPatient;
                
                cout << "\n--- Add New Patient ---\n";
                cout << "Enter Patient ID: ";
                cin >> newPatient.patientID;
                cin.ignore();
                
                cout << "Enter Name: ";
                getline(cin, newPatient.name);
                
                cout << "Enter Age: ";
                cin >> newPatient.age;
                cin.ignore();
                
                cout << "Enter Symptoms: ";
                getline(cin, newPatient.symptoms);
                
                cout << "Enter Priority (1=Critical, 2=Urgent, 3=Standard): ";
                cin >> newPatient.priorityLevel;
                cin.ignore();
                
                newPatient.admissionTime = getCurrentTime();
                
                if (patientRecords.insertPatient(newPatient)) {
                    cout << "✓ Patient added successfully!\n";
                } else {
                    cout << "✗ Failed to add patient.\n";
                }
                break;
            }
            
            case 2: { // Search Patient
                int id;
                cout << "\nEnter Patient ID to search: ";
                cin >> id;
                
                PatientData* patient = patientRecords.searchPatient(id);
                if (patient != nullptr) {
                    patientRecords.displayPatient(id);
                } else {
                    cout << "✗ Patient not found!\n";
                }
                break;
            }
            
            case 3: { // Delete Patient
                int id;
                cout << "\nEnter Patient ID to delete: ";
                cin >> id;
                
                if (patientRecords.deletePatient(id)) {
                    cout << "✓ Patient deleted successfully!\n";
                } else {
                    cout << "✗ Failed to delete patient. ID not found.\n";
                }
                break;
            }
            
            case 4: { // Update Patient
                int id;
                cout << "\nEnter Patient ID to update: ";
                cin >> id;
                cin.ignore();
                
                PatientData* existing = patientRecords.searchPatient(id);
                if (existing == nullptr) {
                    cout << "✗ Patient not found!\n";
                    break;
                }
                
                PatientData updated = *existing;
                
                cout << "Update Name (current: " << updated.name << "): ";
                getline(cin, updated.name);
                
                cout << "Update Age (current: " << updated.age << "): ";
                cin >> updated.age;
                cin.ignore();
                
                cout << "Update Symptoms (current: " << updated.symptoms << "): ";
                getline(cin, updated.symptoms);
                
                cout << "Update Priority (current: " << updated.priorityLevel << "): ";
                cin >> updated.priorityLevel;
                
                if (patientRecords.updatePatient(id, updated)) {
                    cout << "✓ Patient updated successfully!\n";
                } else {
                    cout << "✗ Failed to update patient.\n";
                }
                break;
            }
            
            case 5: { // Display All
                displayAllPatients(patientRecords);
                break;
            }
            
            case 6: { // Display by Priority
                int priority;
                cout << "\nEnter Priority (1=Critical, 2=Urgent, 3=Standard): ";
                cin >> priority;
                
                vector<PatientData> patients = patientRecords.getPatientsByPriority(priority);
                
                if (patients.empty()) {
                    cout << "No patients with priority " << priority << endl;
                } else {
                    cout << "\nPatients with Priority " << priority << ":\n";
                    for (const auto& p : patients) {
                        cout << "  ID: " << p.patientID << " - " << p.name << " (" << p.symptoms << ")\n";
                    }
                }
                break;
            }
            
            case 7: { // Statistics
                displayStatistics(patientRecords);
                break;
            }
            
            case 8: { // Save to File
                string filename;
                cout << "\nEnter filename (e.g., patients.csv): ";
                cin >> filename;
                
                if (patientRecords.saveToFile(filename)) {
                    cout << "✓ Records saved successfully!\n";
                } else {
                    cout << "✗ Failed to save records.\n";
                }
                break;
            }
            
            case 9: { // Load from File
                string filename;
                cout << "\nEnter filename to load: ";
                cin >> filename;
                
                if (patientRecords.loadFromFile(filename)) {
                    cout << "✓ Records loaded successfully!\n";
                } else {
                    cout << "✗ Failed to load records.\n";
                }
                break;
            }
            
            case 10: { // Add Sample Data
                addSampleData(patientRecords);
                break;
            }
            
            case 11: { // Clear All
                char confirm;
                cout << "\nAre you sure you want to clear all records? (y/n): ";
                cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    patientRecords.clearAllRecords();
                    cout << "✓ All records cleared!\n";
                }
                break;
            }
            
            case 12: { // Visualize Tree
                patientRecords.printTreeStructure();
                break;
            }
            
            case 13: { // Verify BST
                patientRecords.displayBSTProperties();
                break;
            }
            
            case 0: { // Exit
                cout << "\nThank you for using Patient Records BST Module!\n";
                break;
            }
            
            default:
                cout << "✗ Invalid choice! Please try again.\n";
        }
        
    } while (choice != 0);
    
    return 0;
}