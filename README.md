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
