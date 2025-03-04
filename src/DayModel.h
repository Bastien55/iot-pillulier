#ifndef DAYMODEL_H
#define DAYMODEL_H

#include <Arduino.h>
#include <ctime>

class DayModel {
    private:
    String dayName;
    int year, month, day;
    bool morning;
    bool midday;
    bool night;
    // Private helper function to get the day name
    String getDayName(int dayIndex);

public:
    // Constructor
    DayModel();
    void setTime(int year, int month, int day, int hour, int minute, int second);
    void updateTime();
    // Determine the time of day based on the given hour
    String getTimePeriod(int hour);

    // Display the day and time period info
    void displayInfo(int hour);
    String getCurrentDate();

    bool IsMorning() { return morning; }
    bool IsMidday() { return midday; }
    bool IsNight() { return night; }

    void setIsMorning(bool isMorning) { morning = isMorning; } 
    void setIsMidday(bool isMidday) { midday = isMidday; }
    void setIsNight(bool isNight) { night = isNight; }
};

class ScheduleParser {
    public:
        static void parseDayPeriods(const String &inputString, DayModel days[7]);
};

#endif