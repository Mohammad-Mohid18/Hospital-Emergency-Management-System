#ifndef PATIENT_BACKEND_H
#define PATIENT_BACKEND_H

#include <string>
#include <vector>
#include <ctime>

struct Patient {
    int id;
    std::string name;
    int age;
    std::string symptoms;
    int priority;
    std::string priorityLabel;
    time_t registrationTime;

    Patient() : id(0), age(0), priority(3) {
        registrationTime = time(nullptr);
    }
};

class BackendInterface {
private:
    std::string filename;

public:
    std::vector<Patient> patientQueue;
    int nextPatientID = 1001;

    BackendInterface(const std::string& file);

    void loadFromFile();
    void saveToFile();

    void addPatient(const Patient& p);

    std::vector<Patient> getQueuedPatients();
    Patient* searchPatient(int id);
    int getTotalPatients();
    int getPatientsByPriority(int priority);
};

#endif