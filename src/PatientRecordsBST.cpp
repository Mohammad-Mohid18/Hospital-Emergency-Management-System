#include <iostream>
#include <fstream>
#include "PatientRecordsBST.h"
#include <sstream>


PatientRecordsBST::PatientRecordsBST() {
    root = nullptr;
}

PatientRecordsBST::~PatientRecordsBST() {
    destroyTree(root);
}
bool PatientRecordsBST::saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return false;

    // CSV Header
    file << "PatientID,Name,Age,Symptoms,Priority\n";

    vector<PatientData> patients = getAllPatients();

    for (const auto& p : patients) {
        file << p.patientID << ","
             << p.name << ","
             << p.age << ","
             << p.symptoms << ","
             << p.priorityLevel << "\n";  // Added \n here - this was missing!
    }

    file.close();
    return true;
}
bool PatientRecordsBST::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string token;

        PatientData p;

        getline(ss, token, ','); p.patientID = stoi(token);
        getline(ss, p.name, ',');
        getline(ss, token, ','); p.age = stoi(token);
        getline(ss, p.symptoms, ',');
        getline(ss, token, ','); p.priorityLevel = stoi(token);

        insertPatient(p);
    }

    file.close();
    return true;
}


void PatientRecordsBST::destroyTree(PatientNode* node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
}

PatientNode* PatientRecordsBST::insertHelper(PatientNode* node, PatientData data) {
    if (!node) return new PatientNode(data);

    if (data.patientID < node->data.patientID)
        node->left = insertHelper(node->left, data);
    else if (data.patientID > node->data.patientID)
        node->right = insertHelper(node->right, data);

    return node;
}

bool PatientRecordsBST::insertPatient(PatientData data) {
    root = insertHelper(root, data);
    return true;
}

PatientNode* PatientRecordsBST::searchHelper(PatientNode* node, int id) {
    if (!node || node->data.patientID == id) return node;
    if (id < node->data.patientID)
        return searchHelper(node->left, id);
    return searchHelper(node->right, id);
}

PatientData* PatientRecordsBST::searchPatient(int id) {
    PatientNode* node = searchHelper(root, id);
    if (!node) return nullptr;
    return &node->data;
}

void PatientRecordsBST::inOrderHelper(PatientNode* node, vector<PatientData>& list) {
    if (!node) return;
    inOrderHelper(node->left, list);
    list.push_back(node->data);
    inOrderHelper(node->right, list);
}

vector<PatientData> PatientRecordsBST::getAllPatients() {
    vector<PatientData> list;
    inOrderHelper(root, list);
    return list;
}
