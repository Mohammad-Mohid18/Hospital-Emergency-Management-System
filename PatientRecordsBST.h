
#ifndef PATIENT_RECORDS_BST_H
#define PATIENT_RECORDS_BST_H

#include <string>
#include <vector>
#include <fstream>
using namespace std;

// Patient Data Structure
struct PatientData {
    int patientID;
    string name;
    int age;
    string symptoms;
    int priorityLevel; // 1=Critical, 2=Urgent, 3=Standard
    string admissionTime;
    
    PatientData() : patientID(0), age(0), priorityLevel(3) {}
    
    PatientData(int id, string n, int a, string s, int p, string time = "")
        : patientID(id), name(n), age(a), symptoms(s), priorityLevel(p), admissionTime(time) {}
};

// BST Node Structure
struct PatientNode {
    PatientData data;
    PatientNode* left;
    PatientNode* right;
    
    PatientNode(PatientData d) : data(d), left(nullptr), right(nullptr) {}
};

// Binary Search Tree Class for Patient Records
class PatientRecordsBST {
private:
    PatientNode* root;
    
    // Private helper functions for recursion
    PatientNode* insertHelper(PatientNode* node, PatientData data);
    PatientNode* searchHelper(PatientNode* node, int patientID);
    PatientNode* deleteHelper(PatientNode* node, int patientID);
    PatientNode* findMin(PatientNode* node);
    void inOrderHelper(PatientNode* node, vector<PatientData>& patients);
    void preOrderHelper(PatientNode* node, vector<PatientData>& patients);
    void destroyTree(PatientNode* node);
    int countNodesHelper(PatientNode* node);
    int getHeightHelper(PatientNode* node);
    
public:
    // Constructor and Destructor
    PatientRecordsBST();
    ~PatientRecordsBST();
    
    // Core Operations
    bool insertPatient(PatientData data);
    PatientData* searchPatient(int patientID);
    bool deletePatient(int patientID);
    bool updatePatient(int patientID, PatientData newData);
    
    // Display Operations
    vector<PatientData> getAllPatients(); // In-order traversal
    vector<PatientData> getPatientsByPriority(int priority);
    void displayPatient(int patientID);
    
    // Statistics
    int getTotalPatients();
    int getPatientCountByPriority(int priority);
    int getTreeHeight();
    bool isEmpty();
    
    // File Operations
    bool saveToFile(const string& filename);
    bool loadFromFile(const string& filename);
    
    // Utility
    void clearAllRecords();
    vector<int> getAllPatientIDs();
    
    // Visualization (Proof it's a BST!)
    void printTreeStructure();
    void displayBSTProperties();
};

#endif // PATIENT_RECORDS_BST_H