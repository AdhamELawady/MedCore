#include "FileManager.h"
#include "Admin.h"
#include "Doctor.h"
#include "Receptionist.h"
#include "Patient.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

FileManager::FileManager(std::string dataDir) : dataDir(std::move(dataDir)) {}

std::string FileManager::PathFor(const std::string& filename) const {
    return (std::filesystem::path(dataDir) / filename).string();
}

namespace {
std::vector<std::string> Split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, delimiter)) fields.push_back(field);
    return fields;
}

std::string Join(const std::vector<std::string>& items, char delimiter) {
    std::string out;
    for (size_t i = 0; i < items.size(); ++i) {
        out += items[i];
        if (i + 1 < items.size()) out += delimiter;
    }
    return out;
}

void EnsureDirExists(const std::string& path) {
    std::filesystem::path p(path);
    if (p.has_parent_path()) std::filesystem::create_directories(p.parent_path());
}
} // namespace

// ---------------- Users ----------------

void FileManager::SaveUsers(const std::vector<User*>& users) const {
    std::string path = PathFor("users.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) {
        std::cerr << "[FileManager] ERROR: could not open " << path << " for writing." << std::endl;
        return;
    }
    for (const User* user : users) {
        std::string extra;
        if (const Doctor* doc = dynamic_cast<const Doctor*>(user)) {
            extra = doc->GetSpecialization() + ";" + doc->GetDepartmentID();
        } else if (const Patient* pat = dynamic_cast<const Patient*>(user)) {
            // Patient now contains a PatientRecord; serialize its contact fields
            extra = pat->GetDOB() + ";" + pat->GetPhone() + ";" + pat->GetAddress();
        }
        out << user->GetUserID() << "|" << user->GetUsername() << "|"
            << user->GetPasswordHash() << "|" << user->GetRole() << "|" << extra << "\n";
    }
}

std::vector<User*> FileManager::LoadUsers() const {
    std::vector<User*> users;
    std::ifstream in(PathFor("users.txt"));
    if (!in.is_open()) return users;

    std::string line;
    int lineNumber = 0;
    while (std::getline(in, line)) {
        ++lineNumber;
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 4) {
            std::cerr << "[FileManager] Skipping malformed users.txt line " << lineNumber << std::endl;
            continue;
        }
        const std::string& userID = f[0];
        const std::string& username = f[1];
        const std::string& hash = f[2];
        const std::string& role = f[3];
        const std::string extra = f.size() > 4 ? f[4] : "";
        std::vector<std::string> sub = Split(extra, ';');

        User* user = nullptr;
        if (role == "Admin") {
            user = new Admin(userID, username, "");
        } else if (role == "Doctor") {
            std::string spec = sub.size() > 0 ? sub[0] : "General";
            std::string dept = sub.size() > 1 ? sub[1] : "";
            user = new Doctor(userID, username, "", spec, dept);
        } else if (role == "Receptionist") {
            user = new Receptionist(userID, username, "");
        } else if (role == "Patient") {
            std::string dob = sub.size() > 0 ? sub[0] : "";
            std::string phone = sub.size() > 1 ? sub[1] : "";
            std::string address = sub.size() > 2 ? sub[2] : "";
            // Construct a Patient (login-capable) that owns a PatientRecord
            user = new Patient(userID, username, "", dob, phone, address);
        } else {
            std::cerr << "[FileManager] Skipping users.txt line " << lineNumber
                      << ": unknown role '" << role << "'" << std::endl;
            continue;
        }
        user->SetPasswordHash(hash);
        users.push_back(user);
    }
    return users;
}

// ---------------- Departments ----------------

void FileManager::SaveDepartments(const std::vector<Department>& departments) const {
    std::string path = PathFor("departments.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) { std::cerr << "[FileManager] ERROR writing departments.txt" << std::endl; return; }
    for (const auto& d : departments) {
        out << d.GetDepartmentID() << "|" << d.GetName() << "|" << d.GetDescription() << "|"
            << Join(d.GetDoctorIDs(), ',') << "\n";
    }
}

std::vector<Department> FileManager::LoadDepartments() const {
    std::vector<Department> departments;
    std::ifstream in(PathFor("departments.txt"));
    if (!in.is_open()) return departments;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 3) continue;
        Department d(f[0], f[1], f[2]);
        if (f.size() > 3 && !f[3].empty()) {
            for (const auto& docID : Split(f[3], ',')) d.AddDoctor(docID);
        }
        departments.push_back(d);
    }
    return departments;
}

// ---------------- Appointments ----------------

void FileManager::SaveAppointments(const std::vector<Appointment>& appointments) const {
    std::string path = PathFor("appointments.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) { std::cerr << "[FileManager] ERROR writing appointments.txt" << std::endl; return; }
    for (const auto& a : appointments) {
        out << a.GetAppointmentID() << "|" << a.GetPatientID() << "|" << a.GetDoctorID() << "|"
            << a.GetDate() << "|" << a.GetTime() << "|" << AppointmentStatusToString(a.GetStatus()) << "\n";
    }
}

std::vector<Appointment> FileManager::LoadAppointments() const {
    std::vector<Appointment> appointments;
    std::ifstream in(PathFor("appointments.txt"));
    if (!in.is_open()) return appointments;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 6) continue;
        Appointment a(f[0], f[1], f[2], f[3], f[4]);
        a.UpdateStatus(AppointmentStatusFromString(f[5]));
        appointments.push_back(a);
    }
    return appointments;
}

// ---------------- MedicalRecords ----------------

void FileManager::SaveMedicalRecords(const std::vector<MedicalRecord>& records) const {
    std::string path = PathFor("medical_records.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) { std::cerr << "[FileManager] ERROR writing medical_records.txt" << std::endl; return; }
    for (const auto& r : records) {
        out << r.GetRecordID() << "|" << r.GetPatientID() << "|" << r.GetDoctorID() << "|"
            << r.GetTimestamp() << "|" << r.GetDiagnosis() << "|" << r.GetTreatment() << "|"
            << Join(r.GetLabResults(), ';') << "\n";
    }
}

std::vector<MedicalRecord> FileManager::LoadMedicalRecords() const {
    std::vector<MedicalRecord> records;
    std::ifstream in(PathFor("medical_records.txt"));
    if (!in.is_open()) return records;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 4) continue;
        MedicalRecord r(f[0], f[1], f[2], f[3]);
        if (f.size() > 4 && !f[4].empty()) r.AddDiagnosis(f[4]);
        if (f.size() > 5 && !f[5].empty()) r.UpdateTreatment(f[5]);
        if (f.size() > 6 && !f[6].empty()) {
            for (const auto& lab : Split(f[6], ';')) r.AddLabResult(lab);
        }
        records.push_back(r);
    }
    return records;
}

// ---------------- Prescriptions ----------------

void FileManager::SavePrescriptions(const std::vector<Prescription>& prescriptions) const {
    std::string path = PathFor("prescriptions.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) { std::cerr << "[FileManager] ERROR writing prescriptions.txt" << std::endl; return; }
    for (const auto& p : prescriptions) {
        std::vector<std::string> medStrs;
        for (const auto& m : p.GetMedicines()) medStrs.push_back(m.name + ":" + m.dosage);
        out << p.GetPrescriptionID() << "|" << p.GetRecordID() << "|" << p.GetDuration() << "|"
            << Join(medStrs, ',') << "\n";
    }
}

std::vector<Prescription> FileManager::LoadPrescriptions() const {
    std::vector<Prescription> prescriptions;
    std::ifstream in(PathFor("prescriptions.txt"));
    if (!in.is_open()) return prescriptions;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 3) continue;
        Prescription p(f[0], f[1], f[2]);
        if (f.size() > 3 && !f[3].empty()) {
            for (const auto& medStr : Split(f[3], ',')) {
                std::vector<std::string> kv = Split(medStr, ':');
                if (kv.size() == 2) p.AddMedicine(kv[0], kv[1]);
            }
        }
        prescriptions.push_back(p);
    }
    return prescriptions;
}

// ---------------- Bills ----------------

void FileManager::SaveBills(const std::vector<Bill>& bills) const {
    std::string path = PathFor("bills.txt");
    EnsureDirExists(path);
    std::ofstream out(path);
    if (!out.is_open()) { std::cerr << "[FileManager] ERROR writing bills.txt" << std::endl; return; }
    for (const auto& b : bills) {
        std::vector<std::string> itemStrs;
        for (const auto& item : b.GetItems()) itemStrs.push_back(item.first + ":" + std::to_string(item.second));
        out << b.GetBillID() << "|" << b.GetPatientID() << "|" << b.GetTotalAmount() << "|"
            << (b.IsPaid() ? "1" : "0") << "|" << b.GetPaymentDate() << "|" << Join(itemStrs, ',') << "\n";
    }
}

std::vector<Bill> FileManager::LoadBills() const {
    std::vector<Bill> bills;
    std::ifstream in(PathFor("bills.txt"));
    if (!in.is_open()) return bills;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::vector<std::string> f = Split(line, '|');
        if (f.size() < 5) continue;
        Bill b(f[0], f[1]);
        std::vector<std::pair<std::string, double>> items;
        if (f.size() > 5 && !f[5].empty()) {
            for (const auto& itemStr : Split(f[5], ',')) {
                std::vector<std::string> kv = Split(itemStr, ':');
                if (kv.size() == 2) {
                    try { items.emplace_back(kv[0], std::stod(kv[1])); } catch (...) {}
                }
            }
        }
        b.GenerateBill(items);
        if (f[3] == "1") b.PayBill(f[4]);
        bills.push_back(b);
    }
    return bills;
}
