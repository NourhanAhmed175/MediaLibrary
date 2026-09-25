#pragma once
#include <iostream>
#include <string>

// Value class: a length of time stored as whole seconds and shown as minutes:seconds.
// Rule of 0: it only holds an int, so the compiler-generated copy/move/destructor are correct.
class Duration {
public:
    Duration();                              // 0:00
    Duration(int minutes, int seconds);      // throws InvalidDurationException
    static Duration fromSeconds(int totalSeconds);

    int getMinutes() const;                  // total minutes (can exceed 59)
    int getSeconds() const;                  // 0..59
    int totalSeconds() const;

    std::string toString() const;            // "m:ss"
    std::string toHMS() const;               // "h:mm:ss"

    Duration operator+(const Duration& other) const;
    Duration& operator+=(const Duration& other);

    bool operator==(const Duration& other) const;
    bool operator!=(const Duration& other) const;
    bool operator<(const Duration& other) const;
    bool operator>(const Duration& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Duration& d);

private:
    int m_totalSeconds;
};
