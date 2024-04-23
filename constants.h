#ifndef CONSTANTS_H  
#define CONSTANTS_H

#define PENALTY_CUSTOMER_FRAC 2
#define PENALTY_EMPLOYEE_FRAC 1.5
#define NUM_USERS 100000
#define CAR_BEGIN_ID NUM_USERS
#define CUSTOMER_BEGIN_ID 2 * NUM_USERS
#define EMPLOYEE_BEGIN_ID 3 * NUM_USERS
#define MANAGER_BEGIN_ID 4 * NUM_USERS
#define REPAIR_REWARD 50 //reward for repairing car given to employee (one unit per unit condition improved)
#define NAME_CAP 20 //maximum length of name
#define HEAVY_DAMAGE 1
#define LIGHT_DAMAGE 2
#define MINOR_SCRATCHES 3
#define FINE 4 
#define DISCOUNT 0.15 //discount for the employees (between 0 and 1)
#define THRESHOLD_RECORD 200 // minimum record to be able to rent a car
#define CUSTOMER_RECORD  THRESHOLD_RECORD * 2//default value
#define EMPLOYEE_RECORD THRESHOLD_RECORD * 3 //default value
#define RETURN_REWARD THRESHOLD_RECORD * 2 //reward for returning the car
#define DAMAGE_PENALTY THRESHOLD_RECORD / 4 // penalty per unit change in condition of the car when returned (assumed the condition is at most same at the time of rental)
#define LATE_RECORD_CHARGE THRESHOLD_RECORD / 4 //per day charge of returning the car late

#endif 
