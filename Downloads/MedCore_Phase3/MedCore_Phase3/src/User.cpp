#include "User.h"
#include "../include/picosha2.h"
#include <iostream>
#include <functional>
#include <sstream>

std::string HashPassword(const std::string& plainPassword) {
    // Use SHA-256 (picosha2) to produce a deterministic, non-plaintext
    // representation of the password. This improves over std::hash used
    // previously. For production, prefer bcrypt/argon2 with salts.
    return picosha2::hash256_hex_string(plainPassword);
}

User::User(std::string userID, std::string username, const std::string& plainPassword)
    : userID(std::move(userID)), username(std::move(username)), passwordHash(HashPassword(plainPassword)) {}

bool User::Login(const std::string& plainPassword) const {
    return HashPassword(plainPassword) == passwordHash;
}

void User::ChangePassword(const std::string& newPlainPassword) {
    passwordHash = HashPassword(newPlainPassword);
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
