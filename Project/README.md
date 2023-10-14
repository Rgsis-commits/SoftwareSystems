
**Online University Login System - Project Overview**

**Project Goal:**
To develop a user-friendly and efficient online university login system that allows students, faculty, and administrators to securely access their respective accounts and perform various operations.

This project primarily focuses on providing a seamless and secure login experience, as well as managing user roles and permissions effectively.

**Features of the Online University Login System (OULS):**

1. **User Authentication:**
    - Login functionality for Students, Faculty, and Administrators.
    - Password-protected access to ensure security.

2. **Role-Based Access:**
    - Different levels of access for Students, Faculty, and Administrators.
    - Each role has specific privileges and functionalities.

3. **Multi-User Support:**
    - Handling multiple login sessions simultaneously.
    - Ensuring each user has a unique session.

4. **Secure Communication:**
    - Utilizing HTTPS for secure data transmission.
    - Encryption protocols to safeguard user information.

5. **Database Management:**
    - Storing user data and credentials securely.
    - Utilizing a relational database for efficient retrieval.

6. **User Profile Management:**
    - Allowing users to update their profiles (e.g., passwords).


8. **Session Management:**
    - Implementing session timeouts for security.
    - Ensuring inactive sessions are terminated.

9. **Error Handling:**
    - Gracefully handling login failures, incorrect passwords, and other potential issues.

10. **Logging and Auditing:**
    - Keeping logs of login activities for security and auditing purposes.

**Operational Workflow:**

1. **User Login:**
    - Users (Students, Faculty, Administrators) provide their credentials.
    - System verifies the login information.

2. **Role-Based Access:**
    - Based on the role (Student, Faculty, Administrator), users are directed to their respective dashboards.

3. **Dashboard Functionalities:**
    - Each role has access to specific functionalities tailored to their needs (e.g., course registration, grade viewing, administrative tasks).

**Implementation Structure:**

- `authentication`: Contains methods for user authentication and session management.
- `database`: Manages the storage and retrieval of user data.
- `user_profiles`: Handles user profile updates and password recovery.
- `logging`: Logs login activities and critical events.

**Execution:**

- Start the server by running `./server.out`.
- Run the client program using `./client.out`.

**Default Credentials:**

- Admin Login ID: Rishav
- Admin Password: R88S



**Notable Results:**

- Only authenticated users can access the system.
- Role-based access ensures users can perform only authorized actions.
- The system supports multiple concurrent sessions, each with a unique session ID.

---

