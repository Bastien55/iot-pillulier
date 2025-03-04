#include "DayModel.h"

// Constructor
DayModel::DayModel() {
    setTime(2025, 3, 3, 16, 35, 00);
    updateTime(); // Fetch time from RTC on startup
}

// Get the name of the day
String DayModel::getDayName(int dayIndex) {
    const String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return days[dayIndex % 7];
}

// Manually set RTC time (only needed once)
void DayModel::setTime(int year, int month, int day, int hour, int minute, int second) {
    struct tm timeInfo;
    timeInfo.tm_year = year - 1900;  // tm_year is years since 1900
    timeInfo.tm_mon = month - 1;     // tm_mon is 0-based
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = minute;
    timeInfo.tm_sec = second;

    time_t t = mktime(&timeInfo);
    struct timeval now = { .tv_sec = t, .tv_usec = 0 }; 
    settimeofday(&now, NULL);  // Set ESP32 RTC time
}

// Fetch the current date/time from RTC
void DayModel::updateTime() {
    struct tm timeInfo;
    
    if (!getLocalTime(&timeInfo)) {
        Serial.println("Failed to get time from RTC");
        return;
    }

    dayName = getDayName(timeInfo.tm_wday);
    year = timeInfo.tm_year + 1900;
    month = timeInfo.tm_mon + 1;
    day = timeInfo.tm_mday;
}

// Get formatted date as String
String DayModel::getCurrentDate() {
    return String(day) + "/" + String(month) + "/" + String(year);
}

// Determine the time of day
String DayModel::getTimePeriod(int hour) {
    if (hour >= 5 && hour <= 11)
        return "Morning";
    else if (hour >= 12 && hour <= 17)
        return "Midday/Afternoon";
    else
        return "Night";
}

// Display information
void DayModel::displayInfo(int hour) {
    Serial.print("Today is: ");
    Serial.println(dayName);
    Serial.print("Date: ");
    Serial.println(getCurrentDate());
    Serial.print("Time period for hour ");
    Serial.print(hour);
    Serial.print(": ");
    Serial.println(getTimePeriod(hour));
}

// Days of the week
const char* weekDays[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

void ScheduleParser::parseDayPeriods(const String &inputString, DayModel days[7]) {
    if (inputString.length() != 21) {
        Serial.println("Error: Invalid input format. Must be 21 characters long.");
        return;
    }

    for (int i = 0; i < 7; i++) {
        String segment = inputString.substring(i * 3, i * 3 + 3); // Extract three characters
        days[i].setIsMorning(segment[0] == '1');
        days[i].setIsMidday(segment[1] == '1');
        days[i].setIsNight(segment[2] == '1');
        
        // Debug output
        Serial.print(weekDays[i]);
        Serial.print(": Morning(");
        Serial.print(days[i].IsMorning() ? "✅" : "❌");
        Serial.print("), Midday(");
        Serial.print(days[i].IsMidday() ? "✅" : "❌");
        Serial.print("), Night(");
        Serial.print(days[i].IsNight() ? "✅" : "❌");
        Serial.println(")");
    }
}