# MedCore -- Phase 3 (Full Implementation, Qt6 GUI)

CSAI 151 Object Oriented Programming, Summer 2026
Adham EL-Awady -- 202201921

## What's implemented

Per the Phase 3 requirements ("Full implementation"):

- [x] **Project Full Code** -- every functionality from the Phase 1 proposal is
      implemented, not just Admin (as in Phase 2). All 4 roles (Admin, Doctor,
      Receptionist, Patient) have complete, working workflows.
- [x] **Files for different project data, finalized and filled with sample
      data** -- `data/` ships with real, cross-referenced sample data: 2
      departments, 1 admin, 2 doctors, 1 receptionist, 2 patients, an
      appointment, a medical record, a prescription, and a bill.
- [x] **Startup load -> operate on data structures -> shutdown save** -- see
      `ApplicationManager::LoadAll()` / `SaveAll()`. All Operation classes
      work exclusively against in-memory vectors; files are only touched at
      startup and shutdown (and on-demand backup).
- [x] **Qt6 GUI** replacing the Phase 2 console UI entirely (confirmed
      allowed by the instructor). See "Architecture" below for how this
      changed the Operation-class design from Phase 2.

## Architecture

### Why the Operation classes changed shape since Phase 2

Phase 2's `Operation` classes (`LoginOperation`, `RegisterOperation`) were
driven by the console `UI` class -- they blocked on `cin`/`cout` via a `UI&`
member. A Qt GUI is event-driven (button clicks fire slots), which doesn't
fit a blocking-input model. Phase 3 switches to the **"alternative design"
explicitly permitted by the course spec**: every Operation class now takes
its inputs as **constructor parameters** and exposes results via
`WasSuccessful()` / `GetMessage()`. A Qt slot handler reads values out of
its widgets, constructs the Operation, calls `Execute()`, and shows the
result in a `QMessageBox`. The exact same Operation classes are used by the
automated test suite, so business logic is tested identically to how the
GUI drives it.

The domain layer (`User` hierarchy, Core classes) is completely UI-agnostic
and required **zero changes** to support the switch from console to Qt --
that separation, established in Phase 2, is what made this migration
straightforward.

### Layers

```
gui/            Qt6 widgets only -- MainWindow (navigation), LoginPage,
                RegisterPage, and 4 role dashboards. No business logic lives
                here; every button click constructs an Operation and reads
                its result.
include/,src/   Domain layer: User hierarchy, Core classes, 16 Operation
                classes, FileManager, ApplicationManager. Zero Qt
                dependency -- compiles and is fully testable headless.
tests/          Automated test suite (38 checks) against an isolated
                ApplicationManager, exercising every Operation class
                directly plus a full persistence round-trip.
data/           Shipped sample data files (see below).
```

### The 16 Operation classes

Login, RegisterPatient, SearchPatient, SearchDoctor, BookAppointment,
CancelAppointment, RescheduleAppointment, UpdateMedicalRecord,
WritePrescription, GenerateBill, PayBill, AddDoctor, RemoveDoctor,
AddReceptionist, ManageDepartment (Add/Update/Remove), AssignDoctor,
GenerateReports.

### User hierarchy

`User` (abstract) -> `Admin`, `Doctor`, `Receptionist`, `Patient`. Business
logic that used to live on `Admin` in Phase 2 was moved into Operation
classes in Phase 3 for consistency with how every other role's actions
work (documented in `include/Admin.h`).

## Data files (`data/`)

One pipe-delimited text file per entity, per the schema in the SRS:

| File | Format |
|---|---|
| `users.txt` | `UserID\|Username\|PasswordHash\|Role\|Extra` (Extra: `specialization;deptID` for Doctor, `dob;phone;address` for Patient) |
| `departments.txt` | `DeptID\|Name\|Description\|doctorID1,doctorID2,...` |
| `appointments.txt` | `ApptID\|PatientID\|DoctorID\|Date\|Time\|Status` |
| `medical_records.txt` | `RecordID\|PatientID\|DoctorID\|Timestamp\|Diagnosis\|Treatment\|lab1;lab2;...` |
| `prescriptions.txt` | `RxID\|RecordID\|Duration\|med1:dosage1,med2:dosage2` |
| `bills.txt` | `BillID\|PatientID\|Total\|Paid(0/1)\|PaymentDate\|item1:amt1,item2:amt2` |

Passwords are never stored in plaintext (hashed via `HashPassword`).

The shipped `data/` already contains sample records. Deleting `data/`
before running re-triggers `SeedSampleDataIfEmpty()`, regenerating the same
starter dataset.

## Build & run

Requires Qt6 (`qt6-base-dev`), CMake >= 3.16, and a C++17 compiler.

```bash
cmake -S . -B build
cmake --build build

./build/medcore_gui        # launch the application
./build/medcore_tests       # run the 38-check automated test suite
```

### Logging in

First run seeds a default Admin: **username `admin`, password `admin123`**.
Sample Doctor/Receptionist/Patient accounts are also seeded (see
`ApplicationManager::SeedSampleDataIfEmpty()` for the full list and their
passwords: `doc123`, `rec123`, `pat123` respectively). New patients can also
self-register from the login screen.

## Test suite

`tests/test_operations.cpp` runs against a disposable, isolated data
directory (never touches the real `data/`), exercising every Operation
class's success and failure paths (duplicate usernames, appointment
conflicts, removing a doctor with active appointments, paying an
already-paid bill, etc.), plus a full save-then-reload round trip to verify
persistence correctness. 38/38 checks passing.

## Presentation & Demo

Per the Phase 3 deliverables, a live demo and presentation are required
separately from the code; this repository provides the working application
and pre-loaded sample data needed to run one.
