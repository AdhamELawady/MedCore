#include "User.h"
#include "../include/picosha2.h"
#include <iostream>
#include <sstream>

namespace {
std::string BuildSalt(const std::string& userID) {
    // A deterministic, user-specific salt keeps identical passwords from
    // producing the same hash across users. For production-grade use, prefer
    // bcrypt/argon2 which includes a secure random salt internally.
    return "medcore-salt-" + userID + "-v2";
}
}

std::string HashPassword(const std::string& plainPassword, const std::string& salt) {
    const std::string salted = salt + ":" + plainPassword + ":MedCore-2026";
    return picosha2::hash256_hex_string(salted);
}

User::User(std::string userID, std::string username, const std::string& plainPassword)
    : userID(std::move(userID)), username(std::move(username)), passwordHash("") {
    passwordHash = HashPassword(plainPassword, BuildSalt(this->userID));
}

bool User::Login(const std::string& plainPassword) const {
    return HashPassword(plainPassword, BuildSalt(userID)) == passwordHash;
}

void User::ChangePassword(const std::string& newPlainPassword) {
    passwordHash = HashPassword(newPlainPassword, BuildSalt(userID));
}

void User::ViewProfile() const {
    std::cout << "UserID: " << userID
              << " | Username: " << username
              << " | Role: " << GetRole() << std::endl;
}

std::string User::GetUserID() const { return userID; }
std::string User::GetUsername() const { return username; }
std::string User::GetPasswordHash() const { return passwordHash; }

void User::SetPasswordHash(const std::string& hash) { passwordHash = hash; }
