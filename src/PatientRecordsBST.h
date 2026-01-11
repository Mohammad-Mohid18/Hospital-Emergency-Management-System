#ifndef PATIENT_RECORDS_BST_H
#define PATIENT_RECORDS_BST_H

#include <string>
#include <vector>

using namespace std;

struct PatientData {
    int patientID;
    string name;
    int age;
    string symptoms;
    int priorityLevel; // 1=Critical, 2=Urgent, 3=Standard
    string admissionTime;

    // Default constructor
    PatientData() : patientID(0), age(0), priorityLevel(3) {}

    // Parameterized constructor
    PatientData(int id, const string& n, int a, const string& s, int p, const string& time = "")
        : patientID(id), name(n), age(a), symptoms(s), priorityLevel(p), admissionTime(time) {}
};

struct PatientNode {
    PatientData data;
    PatientNode* left;
    PatientNode* right;

    PatientNode(PatientData d) : data(d), left(nullptr), right(nullptr) {}
};

class PatientRecordsBST {
private:
    PatientNode* root;

    PatientNode* insertHelper(PatientNode* node, PatientData data);
    PatientNode* searchHelper(PatientNode* node, int id);
    void inOrderHelper(PatientNode* node, vector<PatientData>& list);
    void destroyTree(PatientNode* node);

public:
    PatientRecordsBST();
    ~PatientRecordsBST();

    bool insertPatient(PatientData data);
    // File Operations
    bool saveToFile(const string& filename);
    bool loadFromFile(const string& filename);

    PatientData* searchPatient(int id);
    vector<PatientData> getAllPatients();
};

#endif
