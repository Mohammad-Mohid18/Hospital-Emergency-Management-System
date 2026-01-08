#include "PatientBackend.h"
#include <fstream>
#include <sstream>
#include <iostream>

BackendInterface::BackendInterface(const std::string& file) {
    filename = file;
    loadFromFile();
}

// Load patients from CSV file
void BackendInterface::loadFromFile() {
    patientQueue.clear();
    std::ifstream infile(filename);
    if (!infile.is_open()) return;

    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream ss(line);
        Patient p;
        std::string regTimeStr;
        std::string ageStr, priorityStr;

        std::getline(ss, line, ','); p.id = std::stoi(line);
        std::getline(ss, p.name, ',');
        std::getline(ss, ageStr, ','); p.age = std::stoi(ageStr);

        std::getline(ss, p.symptoms, ',');
        if (!p.symptoms.empty() && p.symptoms.front() == '\"' && p.symptoms.back() == '\"') {
            p.symptoms = p.symptoms.substr(1, p.symptoms.size() - 2);
        }

        std::getline(ss, priorityStr, ','); p.priority = std::stoi(priorityStr);
        std::getline(ss, p.priorityLabel, ',');
        std::getline(ss, regTimeStr); p.registrationTime = std::stol(regTimeStr);

        patientQueue.push_back(p);

        if (p.id >= nextPatientID) nextPatientID = p.id + 1;
    }

    infile.close();
}

// Save patients to CSV file
void BackendInterface::saveToFile() {
    std::ofstream outfile(filename, std::ios::trunc);
    for (const auto& p : patientQueue) {
        outfile << p.id << ","
                << p.name << ","
                << p.age << ","
                << "\"" << p.symptoms << "\"" << ","
                << p.priority << ","
                << p.priorityLabel << ","
                << p.registrationTime << "\n";
    }
    outfile.close();
}

// Add new patient
void BackendInterface::addPatient(const Patient& p) {
    patientQueue.push_back(p);
    saveToFile();
}

// Other functions
std::vector<Patient> BackendInterface::getQueuedPatients() {
    return patientQueue;
}

Patient* BackendInterface::searchPatient(int id) {
    for (auto& p : patientQueue) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

int BackendInterface::getTotalPatients() {
    return patientQueue.size();
}

int BackendInterface::getPatientsByPriority(int priority) {
    int count = 0;
    for (const auto& p : patientQueue) {
        if (p.priority == priority) count++;
    }
    return count;
}