#ifndef USER_H
#define USER_H

#include <string>

// A stronger, salt-derived hash for the academic project.
// The hash is computed with a user-specific salt so the same plain password
// does not generate the same hash for different users. For production-grade
// deployments, use bcrypt/argon2 with a secure per-user salt.
std::string HashPassword(const std::string& plainPassword, const std::string& salt);

// Abstract base class for every user role in MedCore.
// Demonstrates: Encapsulation (private data + public accessors),
// Abstraction (pure virtual GetRole), and is the root of the
// Inheritance/Polymorphism hierarchy (Admin, Doctor, Receptionist, Patient).
class User {
protected:
    std::string userID;
    std::string username;
    std::string passwordHash;

public:
    User(std::string userID, std::string username, const std::string& plainPassword);
    virtual ~User() = default;

    // Common behaviour shared by all roles.
    bool Login(const std::string& plainPassword) const;
    void ChangePassword(const std::string& newPlainPassword);
    virtual void ViewProfile() const;

    // Pure virtual: forces every derived class to identify its role.
    // This is the hook used for polymorphic dispatch (e.g., menu routing).
    virtual std::string GetRole() const = 0;

    std::string GetUserID() const;
    std::string GetUsername() const;
    std::string GetPasswordHash() const;

    // Used by FileManager when reconstructing a user from a stored hash
    // (i.e., we already have the hash, we should not re-hash it).
    void SetPasswordHash(const std::string& hash);
};

#endif // USER_H
