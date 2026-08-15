#ifndef PATIENT_H
#define PATIENT_H

#include "User.h"
#include <string>
#include <memory>

// PatientRecord: domain entity storing patient-specific data. This is NOT a User.
class PatientRecord {
public:
    PatientRecord() = default;
    PatientRecord(std::string id, std::string dob, std::string phone, std::string address)
        : patientID(std::move(id)), dob(std::move(dob)), phone(std::move(phone)), address(std::move(address)) {}

    const std::string& GetPatientID() const { return patientID; }
    const std::string& GetDOB() const { return dob; }
    const std::string& GetPhone() const { return phone; }
    const std::string& GetAddress() const { return address; }

    void UpdateContact(const std::string& newPhone, const std::string& newAddress) {
        phone = newPhone; address = newAddress;
    }

private:
    std::string patientID;
    std::string dob;
    std::string phone;
    std::string address;
};

// Patient: login-capable User that owns a PatientRecord (composition)
class Patient : public User {
public:
    Patient(std::string userID, std::string username, const std::string& plainPassword,
            std::string dob = "", std::string phone = "", std::string address = "")
        : User(std::move(userID), std::move(username), plainPassword) {
        patientRecord = std::make_shared<PatientRecord>(GetUserID(), std::move(dob), std::move(phone), std::move(address));
    }

    std::string GetRole() const override { return "Patient"; }

    std::string GetDOB() const { return patientRecord ? patientRecord->GetDOB() : std::string(); }
    std::string GetPhone() const { return patientRecord ? patientRecord->GetPhone() : std::string(); }
    std::string GetAddress() const { return patientRecord ? patientRecord->GetAddress() : std::string(); }

    void UpdateInformation(const std::string& newPhone, const std::string& newAddress) {
        if (patientRecord) patientRecord->UpdateContact(newPhone, newAddress);
    }

    std::shared_ptr<PatientRecord> GetPatientRecord() const { return patientRecord; }

private:
    std::shared_ptr<PatientRecord> patientRecord;
};

#endif // PATIENT_H
