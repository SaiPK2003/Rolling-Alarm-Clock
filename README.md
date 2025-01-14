# Rolling Alarm Clock

## Introduction  
Waking up can be tough, especially for sleep-deprived students. The Rolling Alarm Clock offers an innovative solution by mimicking an alarm clock that rolls away and forces users to wake up and chase it to turn it off. In a team of 4, we built the mobile alarm clock using Lego EV3 Mindstorms kits and programmed in C, this project replicates alarm clock functionality while adding mobility and obstacle avoidance.

---

## Features
- **User Input for Current and Alarm Time:** Set time using button inputs for hours and minutes.
- **Time Display:** Displays time in AM/PM format, updating every minute.
- **Alarm Protocol:** 
  - Activates at the set alarm time.
  - Robot moves randomly while avoiding obstacles using ultrasonic and touch sensors.
  - Stops after 100m of travel or user interaction.
- **Obstacle Avoidance:** Reverses and rotates to avoid collisions.
- **Resets Easily:** Allows resetting the alarm before activation.

---

## Data Storage and Software Design
### Data Management
- Time stored in a `militaryTime` struct (`hours` and `minutes`).
- Variables for milliseconds for accurate timekeeping.

### Key Functions
1. **`getButtonTime`:** Allows users to set time using buttons. Includes input validation and real-time display updates.
2. **`timeToMs` & `msToTime`:** Convert between milliseconds and military time.
3. **`alarmProtocol`:** 
   - Activates alarm.
   - Calls `driveRobot` and `rotateRobot` to move the robot and avoid obstacles.
   - Stops after traveling 100m or user interaction.
4. **`militaryToStdHrs`:** Converts military time to standard time for display.
5. **`outputTime`:** Displays current time in large text.

### Design Decisions
- User inputs both current and alarm times, mimicking standard alarm clocks.
- Structured to handle time rollover (e.g., 11:59 PM → 12:00 AM).
- Efficient time calculations using arithmetic and periodic updates.

---

## Testing and Debugging
1. **Individual Functions:**
   - `getButtonTime`: Verified button input and time rollover.
   - `timeToMs` & `msToTime`: Tested conversion accuracy.
   - `alarmProtocol`: Tested robot movement, obstacle avoidance, and alarm sound.
2. **Integration Testing:**
   - Evaluated overall functionality.
   - Debugged issues with gyro resets, obstacle proximity, and incorrect time values.

---

## Challenges and Resolutions
- **Timer Initialization:** Overhauled to calculate the wait time dynamically.
- **Obstacle Avoidance:** Added reverse motion for better maneuverability.
- **Input Validation:** Resolved invalid time storage by restructuring conditional logic.

---

## Conclusion
The Rolling Alarm Clock is fully functional, forcing users to wake up and turn off the alarm by chasing the robot. It avoids obstacles, provides an accurate time display, and offers a simple reset mechanism. 

---

## Recommendations
- Improve sound generation for a louder, more effective alarm.
- Allow canceling alarms before activation.
- Add an automatic time-sync feature for easier setup.
- Offer a military/standard time display toggle.

---

## How to Use
1. Set the current and alarm time using buttons.
2. The robot will roll and avoid obstacles at the set alarm time.
3. To stop the alarm, press the Enter button or let the robot stop after 100m.
4. Reset the alarm as needed.

---

## Future Scope
Enhancing the Rolling Alarm Clock for industry use:
- Integrate an RTC (real-time clock) module for automatic timekeeping.
- Refine the physical design for better sound and durability.
