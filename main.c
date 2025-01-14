/*Final Project
//MTE 121
//Sai Prakash Kumar, Victor Salvador, Timothy Pham, Vishan Muralikaran
Parallel Parking Simulation*/

// Constants for various time conversions and limits
const float WHEEL_RADIUS = 2.75; // Radius of the wheel in cm
const int MIN_IN_MS = 60000;     // Minutes in milliseconds
const int HOUR_IN_MS = 3600000;  // Hours in milliseconds
const int HOURS_IN_DAY = 24;     // Total hours in a day
const int MINS_IN_HOUR = 60;     // Total minutes in an hour
const int MAX_DRIVE_DIST = 10000; // Maximum driving distance (100m in cm)

// Structure to represent time in military (24-hour) format
typedef struct {
    int hours;   // Hours (0-23)
    int minutes; // Minutes (0-59)
} militaryTime;

// Function to calibrate sensors
void calibrateSensors() {
    SensorType[S4] = sensorEV3_Gyro;          // Configure Gyro sensor
    wait1Msec(50);
    SensorMode[S4] = modeEV3Gyro_Calibration; // Calibrate Gyro
    wait1Msec(50);
    SensorMode[S4] = modeEV3Gyro_RateAndAngle; // Set Gyro to rate and angle mode
    wait1Msec(50);

    SensorType[S3] = sensorEV3_Ultrasonic;    // Configure Ultrasonic sensor
    wait1Msec(50);

    SensorType[S1] = sensorEV3_Touch;         // Configure Touch sensor
    wait1Msec(50);
}

// Function to determine AM/PM based on military time
void dayOrNot(string &aP, const militaryTime &mTim) {
    if (mTim.hours < 12) {
        aP = "AM"; // Morning
    } else {
        aP = "PM"; // Afternoon/Evening
    }
}

// Convert military hours (24-hour format) to standard hours (12-hour format)
int militaryToStdHrs(const militaryTime &milTime) {
    if (milTime.hours > 12) {
        return milTime.hours - 12; // Convert 13-23 to 1-11 PM
    } else if (milTime.hours > 0) {
        return milTime.hours;     // 1-12 AM
    } else {
        return 12;                // Midnight
    }
}

// Function to display time on the screen
void outputTime(int line, string mornEve, int hur, int miin) {
    if (miin < 10) {
        displayBigTextLine(line, " %d:0%d %s", hur, miin, mornEve); // Add leading zero for minutes
    } else {
        displayBigTextLine(line, " %d:%d %s", hur, miin, mornEve);
    }
}

// Convert milliseconds to time (hours and minutes)
void msToTime(long int milliSecs, long int startTime, militaryTime &milliTime) {
    int milToMins = milliSecs / MIN_IN_MS;    // Convert milliseconds to minutes
    milliTime.hours = milToMins / MINS_IN_HOUR; // Extract hours
    milliTime.minutes = milToMins % MINS_IN_HOUR; // Extract remaining minutes
}

// Convert time (hours and minutes) to milliseconds
long int timeToMs(const militaryTime &milTimeeMs) {
    return ((milTimeeMs.hours * HOUR_IN_MS) + (milTimeeMs.minutes * MIN_IN_MS));
}

// Function to get time input from buttons and convert to milliseconds
long int getButtonTime(const string &typeTime, militaryTime &inputTime) {
    int stdH = 12, stdM = 0;
    string aAmPm = "AM";

    displayString(1, "Press L/R to set minutes,");
    displayString(2, "U/D to set hours.");
    outputTime(4, aAmPm, stdH, stdM);
    displayString(8, "Press ENTER when done setting");
    displayString(9, "the %s time.", typeTime);

    while (!getButtonPress(buttonEnter)) {
        while (!getButtonPress(buttonAny)) {}

        if (getButtonPress(buttonLeft)) {
            while (getButtonPress(buttonLeft)) {}
            --inputTime.minutes;
        } else if (getButtonPress(buttonRight)) {
            while (getButtonPress(buttonRight)) {}
            ++inputTime.minutes;
        } else if (getButtonPress(buttonUp)) {
            while (getButtonPress(buttonUp)) {}
            ++inputTime.hours;
        } else if (getButtonPress(buttonDown)) {
            while (getButtonPress(buttonDown)) {}
            --inputTime.hours;
        }

        wait1Msec(50);

        // Handle overflow and underflow for minutes and hours
        if (inputTime.minutes < 0) {
            --inputTime.hours;
            inputTime.minutes += MINS_IN_HOUR;
        } else if (inputTime.minutes >= MINS_IN_HOUR) {
            inputTime.minutes -= MINS_IN_HOUR;
            ++inputTime.hours;
        }

        if (inputTime.hours < 0) {
            inputTime.hours += HOURS_IN_DAY;
        } else if (inputTime.hours >= HOURS_IN_DAY) {
            inputTime.hours -= HOURS_IN_DAY;
        }

        stdH = militaryToStdHrs(inputTime);
        stdM = inputTime.minutes;
        dayOrNot(aAmPm, inputTime);
        outputTime(4, aAmPm, stdH, stdM);
    }

    return timeToMs(inputTime);
}

// Function to control the robot's motors
void driveBot(int const mPowA, int const mPowD) {
    motor[motorA] = mPowA;
    motor[motorD] = mPowD;
}

// Function to rotate the robot when obstacles are detected
void rotateRobot() {
    driveBot(0, 0);
    wait1Msec(50);

    while (SensorValue[S3] < 40) {
        driveBot(-15, -15); // Reverse
    }

    driveBot(0, 0);
    wait1Msec(50);

    while (SensorValue[S3] < 70 && getGyroDegrees(S4) < 180) {
        driveBot(25, -25); // Rotate right
    }

    resetGyro(S4);
    driveBot(50, 50); // Resume
}

// Function for the alarm protocol
void alarmProtocol() {
    motor[motorB] = 50; // Alarm movement
    driveBot(50, 50);   // Drive forward
    displayString(5, "PRESS ENTER TO STOP ALARM");

    while (!getButtonPress(buttonEnter) && nMotorEncoder[motorA] < ((MAX_DRIVE_DIST * 180) / (PI * WHEEL_RADIUS))) {
        if ((SensorValue[S1] != 0) || (SensorValue[S3] < 25)) {
            rotateRobot(); // Rotate on obstacle
        }
    }

    driveBot(0, 0);   // Stop
    motor[motorB] = 0; // Stop alarm movement
}

// Calculate wait time before wake-up
long int getWaitTime(long int currMS, long int wakeMS) {
    if (currMS < wakeMS) {
        return (wakeMS - currMS);
    } else {
        return ((HOURS_IN_DAY * HOUR_IN_MS) - (currMS - wakeMS));
    }
}

// Main task to execute alarm and robot operations
task main() {
    calibrateSensors(); // Initialize sensors

    militaryTime currTime = {0, 0};
    militaryTime wakeTime = {0, 0};

    string currentAmPm = "AM";
    long int currTimeStart = getButtonTime("current", currTime);
    time1[T1] = 0;

    long int wakeUp = getButtonTime("wake-up", wakeTime);
    long int alarmTime = getWaitTime(currTimeStart, wakeUp);

    if (alarmTime > 0) {
        alarmProtocol();
    }
}
