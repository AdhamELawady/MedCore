#include "Patient.h"

Patient::Patient(std::string userID, std::string username, const std::string& plainPassword,
                  std::string dob, std::string phone, std::string address)
    : User(std::move(userID), std::move(username), plainPassword),
      dob(std::move(dob)), phone(std::move(phone)), address(std::move(address)) {}

std::string Patient::GetRole() const { return "Patient"; }
std::string Patient::GetDOB() const { return dob; }
std::string Patient::GetPhone() const { return phone; }
std::string Patient::GetAddress() const { return address; }

void Patient::UpdateInformation(const std::string& newPhone, const std::string& newAddress) {
    phone = newPhone;
    address = newAddress;
}
