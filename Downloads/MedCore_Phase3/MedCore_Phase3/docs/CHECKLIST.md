MedCore – Final Revision & Pre-Submission Checklist

OOP Summer Project | Adham EL-Awady

This document is the canonical checklist to verify before final submission and GitHub publication.

# 1. CRITICAL — Fix Application Startup
- Ensure startup leads to Login page first.
- After authentication, detect role and open corresponding dashboard.
- Remove any temporary hard-coded Patient startup logic.
- Acceptance: fresh start -> Login page; Admin/Doctor/Receptionist/Patient logins open their dashboards accordingly.

# 2. CRITICAL — Correct the UML User Hierarchy
- User is abstract; Admin/Doctor/Receptionist/PatientUser inherit User.
- Patient is a domain entity associated with PatientUser (composition).
- Regenerate UML diagrams to reflect this structure.

# 3. CRITICAL — Make SRS and GUI Consistent
- Ensure docs refer to Qt6 GUI; remove console-only wording.
- Keep business logic independent from GUI layer and ensure names match.

# 4. CRITICAL — Persistence / Save-Load Verification
- Load required data at startup; persist on Save/Shutdown; verify relationships remain after reload.

# 5. CRITICAL — Password Storage
- Do NOT store plaintext passwords. Verify stored file contents.
- Authentication must compare hashes correctly.

# 6. HIGH — Audit All Operations
- Verify each Operation is implemented or marked as future work.

# 7. HIGH — Input Validation & Error Handling
- Validate empty/invalid fields, duplicates, negative billing values, etc.

# 8. HIGH — Role-Based Access Control
- GUI and operations enforce role permissions.

# 9. HIGH — Architecture Check
- Verify GUI -> ApplicationManager/Operations -> Domain -> FileManager layering.

# 10. MEDIUM — GUI Polish
- Consistent typography and behavior; logout returns to Login.

# 11. MEDIUM — Documentation / UML
- Final diagrams must match code; remove outdated diagrams.

# 12. GITHUB PREPARATION
- Directory layout: include/, src/, gui/, tests/, data/, docs/, CMakeLists.txt, README.md, LICENSE
- Do NOT upload build artifacts. Do NOT publish secrets.

# 13. FINAL DEMO TEST
- Checklist of acceptance tests (login flows, persistence, no-crash, clean build).

# 14. RECOMMENDED ORDER OF WORK
1. Fix Startup -> Login.
2. Verify Login -> correct role dashboard.
3. Correct/regenerate UML.
4. Run persistence tests.
5. Verify password storage and authentication.
6. Audit operations.
7. Add validation and role restrictions.
8. Polish GUI.
9. Update docs and README.
10. Clean repository and publish.
