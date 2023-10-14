#ifndef SERVER_CONSTANTS
#define SERVER_CONSTANTS


// ========== COMMON TEXT =============================

// LOGIN
#define LOGIN_ID "Enter your login ID"
#define PASSWORD "Enter your password \n# "
#define INVALID_LOGIN "The login ID specified doesn't exist!$"
#define INVALID_PASSWORD "The password specified doesn't match!$"
#define SALT_BAE "666"

// GET ACCOUNT DETAILS
#define GET_STUDENT_ID "Enter the student ID of the student you're searching for"
#define GET_FACULTY_ID "Enter the faculty ID of the faculty you're searching for"
#define GET_COURSE_ID "Enter the course ID of the course you want to enroll"
#define GET_COURSE_ID_DROP "Enter the course ID of the course you want to drop"

#define ERRON_INPUT_FOR_NUMBER "It seems you have passed a sequence of alphabets when a number was expected or you have entered an invalid number!\nYou'll now be redirected to the main menu!^"

#define INVALID_MENU_CHOICE "It seems you've made an invalid choice\nYou'll now be redirected to the main menu!^"

#define STUDENT_ID_DOESNT_EXIT "No student could be found for the given ID"
#define FACULTY_ID_DOESNT_EXIT "No faculty could be found for the given ID"
#define COURSE_ID_DOESNT_EXIT "No course could be found for the given ID"
#define FACULTY_LOGIN_ID_DOESNT_EXIT "No FACULTY could be found for the given login ID$"
#define STUDENT_LOGIN_ID_DOESNT_EXIT "No STUDENT could be found for the given login ID$"

#define ACCOUNT_ID_DOESNT_EXIT "No account could be found for the given account number"

#define TRANSACTIONS_NOT_FOUND "No transactions were performed on this account by the customer!^"

// ====================================================

// ========== ADMIN SPECIFIC TEXT======================

// LOGIN WELCOME
#define ADMIN_LOGIN_WELCOME "Welcome dear admin! With great power comes great responsibility!\nEnter your credentials to unlock this power!"
#define ADMIN_LOGIN_SUCCESS "Welcome superman!"

// ADMIN MENU
#define ADMIN_MENU "1. Add Student\n2. View Student Details\n3. Add Faculty\n4. View Faculty Details\n\
5. Activate Student\n6. Block Student\n7. Modify Student Details\n8. Modify Faculty Details\n9. Logout and Exit"

// ADD 
#define ADMIN_ADD_STUDENT_NAME "What is the student's name?"
#define ADMIN_ADD_FACULTY_NAME "What is the faculty's name?"
#define FACULTY_ADD_COURSE_NAME "What is the course's name?"
#define ADMIN_ADD_STUDENT_COURSE "What is the student's course? (comma separated course id)"
#define ADMIN_ADD_FACULTY_COURSE "What is the faculty's course?"
#define ADMIN_ADD_STUDENT_AUTOGEN_LOGIN "The autogenerated login ID for the student is : "
#define ADMIN_ADD_FACULTY_AUTOGEN_LOGIN "The autogenerated login ID for the faculty is : "
#define AUTOGEN_PASSWORD "spookytime" // append to end of next string macro
#define ADMIN_ADD_STUDENT_AUTOGEN_PASSWORD "The autogenerated password for the student is : "
#define ADMIN_ADD_FACULTY_AUTOGEN_PASSWORD "The autogenerated password for the faculty is : "
#define ADMIN_ADD_CUSTOMER_WRONG_GENDER "It seems you've enter a wrong gender choice!\nYou'll now be redirected to the main menu!^"




// MODIFY  INFO
#define ADMIN_MOD_STUDENT_ID "Enter the ID of the student who's information you want to edit"
#define ADMIN_MOD_FACULTY_ID "Enter the ID of the faculty who's information you want to edit"
#define COURSE_MOD_FACULTY_ID "Enter the ID of the course who's information you want to edit"
#define FACULTY_DEL_COURSE_ID "Enter the ID of the course who's information you want to delete"
#define FACULTY_DEL_COURSE_CONF "Are you sure you want to remove this course from your catalog? (y/n)"
#define ADMIN_MOD_STUDENT_MENU "Which information would you like to modify?\n1. Name\nPress any other key to cancel"
#define ADMIN_MOD_FACULTY_MENU "Which information would you like to modify?\n1. Name\nPress any other key to cancel"
#define COURSE_MOD_FACULTY_MENU "Which information would you like to modify?\n1. Name\n2. Limit"

#define ADMIN_MOD_NEW_NAME "What's the updated value for name?"
#define ADMIN_MOD_CUSTOMER_NEW_GENDER "What's the updated value for gender?"
#define ADMIN_MOD_CUSTOMER_NEW_AGE "What's the updated value for age?"
#define COURSES_ID_DOESNT_EXIT "Course ID is invalid!"

#define ADMIN_MOD_SUCCESS "The required modification was successfully made!\nYou'll now be redirected to the main menu!^"
#define COURSE_DEL_SUCCESS "Please login again to see your changes. :)\n\nThe required modification was successfully made!\nYou'll now be redirected to the main menu!^"

#define ADMIN_LOGOUT "Logging you out now superman! Good bye!$"

// ====================================================

// ========== FACULTY SPECIFIC TEXT===================

// LOGIN WELCOME
#define FACULTY_LOGIN_WELCOME "Welcome dear FACULTY! Enter your credentials to gain access to your account!"
#define FACULTY_LOGIN_SUCCESS "Welcome beloved FACULTY!"

#define FACULTY_LOGOUT "Logging you out now dear FACULTY! Good bye!$"

// ADMIN MENU
#define FACULTY_MENU "1. View Offering Courses\n2. Add New Course\n3. Remove Course from Catalog\n4. Update Course Details\n5. Change Password\n6. Logout and Exit\nEnter the number corresponding to the choice!\n"


#define PASSWORD_CHANGE_OLD_PASS "Enter your old password"
#define PASSWORD_CHANGE_OLD_PASS_INVALID "The entered password doesn't seem to match with the old password"
#define PASSWORD_CHANGE_NEW_PASS "Enter the new password"
#define PASSWORD_CHANGE_NEW_PASS_RE "Reenter the new password"
#define PASSWORD_CHANGE_NEW_PASS_INVALID "The new password and the reentered passwords don't seem to pass!^"
#define PASSWORD_CHANGE_SUCCESS "Password successfully changed!^"

// ====================================================

// ========== STUDENT SPECIFIC TEXT===================

// LOGIN WELCOME
#define STUDENT_LOGIN_WELCOME "Welcome dear STUDENT! Enter your credentials to gain access to your account!"
#define STUDENT_LOGIN_SUCCESS "Welcome beloved STUDENT!"

#define STUDENT_LOGOUT "Logging you out now dear STUDENT! Good bye!$"

// ADMIN MENU
#define STUDENT_MENU "1. View All Courses\n2. Enroll (pick) New Course\n3. Drop Course\n4. View Enrolled Course Details\n5. Change Password\n6. Logout and Exit\nEnter the number corresponding to the choice!\n"



#define PASSWORD_CHANGE_OLD_PASS "Enter your old password"
#define PASSWORD_CHANGE_OLD_PASS_INVALID "The entered password doesn't seem to match with the old password"
#define PASSWORD_CHANGE_NEW_PASS "Enter the new password"
#define PASSWORD_CHANGE_NEW_PASS_RE "Reenter the new password"
#define PASSWORD_CHANGE_NEW_PASS_INVALID "The new password and the reentered passwords don't seem to pass!^"
#define PASSWORD_CHANGE_SUCCESS "Password successfully changed!^"

// ====================================================


#define COURSE_FILE "./Database/courses.txt"
#define FACULTY_FILE "./Database/faculty.txt"
#define STUDENT_FILE "./Database/student.txt"

#endif