# Secure Student Management System

A comprehensive web-based student record management system built with C++/CGI backend, featuring advanced security mechanisms including multi-factor authentication, role-based access control, and session management.

## Technical Stack

- **Backend**: C++/CGI
- **Database**: MySQL
- **Cryptography**: OpenSSL (AES-256-GCM encryption)
- **Web Framework**: CGICC
- **Security**: Custom authentication system with 2FA

## Architecture

The system implements a modular, security-first architecture with:

- **Session Management**: Cookie-based authentication with database shadowing
- **Multi-Factor Authentication**: Email-based PIN + hardware token verification
- **Role-Based Access Control**: Separate interfaces for administrators and lecturers
- **Input Validation**: Comprehensive sanitization against injection attacks
- **Timeout Management**: Automatic session expiration (30-60 second windows)

## Security Features

### Authentication System
- **Phase-based login**: 3-stage authentication process
- **Email verification**: PIN codes sent to registered email addresses
- **Hardware token**: Base64-encoded AES-256-GCM encrypted tokens for admin access
- **Session tracking**: Database-backed session state management

### Data Protection
- **Password encryption**: Custom cipher implementation
- **Input sanitization**: Protection against SQL injection and XSS
- **Access controls**: Role-based permissions with validation
- **Timeout mechanisms**: Automatic logout after inactivity

### Validation Framework
- Username: Minimum 4 characters, uniqueness enforced
- Passwords: Minimum 5 characters, encrypted storage
- Email addresses: Format validation with @ symbol requirement
- Marks: Integer validation between 0-100 range

## Project Structure 
CGI/                   # Main application modules <br />
├── checkToken.cpp          # Hardware token verification <br />
├── emailLogin.cpp          # Initial authentication handler <br />
├── lecturer.cpp            # Lecturer dashboard and authentication <br />
├── lecturerModule.cpp      # Module assignment interface
├── logOut.cpp              # Session termination
├── module.cpp              # Individual module management
├── saveLecturer.cpp        # User account creation
├── saveLecturerModules.cpp # Module assignment processing
├── saveModule.cpp          # Grade management and updates
├── saveStudentToModule.cpp # Student enrollment
├── selectLecturer.cpp      # Lecturer selection interface
├── selectModule.cpp        # Module selection interface
└── tokenLogin.cpp          # Administrator token authentication

crypto/                # Cryptographic utilities
├── base64.cpp              # Base64 encoding/decoding
├── base64.h                # Base64 header file
├── decrypt                 # Decryption executable
├── encrypt                 # Encryption executable
├── encrypt.cpp             # AES-256-GCM encryption implementation
└── evp-encrypt             # OpenSSL encryption utility

utils/                 # Database utilities
├── resetEmailPin.cpp       # Session table management
├── resetModules.cpp        # Module data initialization
├── resetUser.cpp           # User account setup
├── resetModules            # Module reset executable
└── resetUser               # User reset executable

## Database Schema

### Core Tables
- **users**: User accounts with encrypted passwords and module assignments
- **emailPin**: Session management and authentication state tracking
- **modules**: Individual module tables (maths, physics, chemistry, english, history)

### Data Relationships
- Users have many-to-many relationships with modules
- Session data links to user accounts with timestamp tracking
- Role-based data access through user ID classification

## Key Features

### For Administrators
- **User Management**: Create lecturer accounts with validation
- **Module Assignment**: Assign teaching responsibilities to lecturers
- **Student Enrollment**: Add students to specific modules
- **System Administration**: Full access to all system functions

### For Lecturers
- **Grade Management**: Update student marks with validation
- **Module Access**: View assigned modules and enrolled students
- **Student Records**: Access to students in assigned modules only

### Security Operations
- **Session Monitoring**: Real-time session state tracking
- **Automatic Logout**: Timeout-based security enforcement
- **Input Validation**: Multi-layer data sanitization
- **Access Verification**: Role-based permission checking

## System Workflow

1. **Initial Authentication**: Username/password verification against encrypted database
2. **Email Verification**: PIN code generation and email delivery
3. **Role Determination**: System routes to appropriate interface (admin/lecturer)
4. **Session Management**: Continuous timeout monitoring and validation
5. **Token Verification**: Additional hardware token step for administrative access
6. **Authorized Operations**: Role-specific functionality with input validation
