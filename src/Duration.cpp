#include "Duration.h"
#include "MediaExceptions.h"
#include <iomanip>
#include <sstream>

Duration::Duration() : m_totalSeconds(0) {}

Duration::Duration(int minutes, int seconds) : m_totalSeconds(0) {
    if (minutes < 0) {
        throw InvalidDurationException("Invalid duration: minutes cannot be negative (got " +
                                       std::to_string(minutes) + ")");
    }
    if (seconds < 0 || seconds > 59) {
        throw InvalidDurationException("Invalid duration: seconds must be between 0 and 59 (got " +
                                       std::to_string(seconds) + ")");
    }
    m_totalSeconds = minutes * 60 + seconds;
}

Duration Duration::fromSeconds(int totalSeconds) {
    if (totalSeconds < 0) {
        throw InvalidDurationException("Invalid duration: total seconds cannot be negative");
    }
    return Duration(totalSeconds / 60, totalSeconds % 60);
}

int Duration::getMinutes() const { return m_totalSeconds / 60; }
int Duration::getSeconds() const { return m_totalSeconds % 60; }
int Duration::totalSeconds() const { return m_totalSeconds; }

std::string Duration::toString() const {
    std::ostringstream out;
    out << *this;
    return out.str();
}

std::string Duration::toHMS() const {
    const int hours = m_totalSeconds / 3600;
    const int minutes = (m_totalSeconds % 3600) / 60;
    const int seconds = m_totalSeconds % 60;
    std::ostringstream out;
    out << hours << ':' << std::setfill('0') << std::setw(2) << minutes
        << ':' << std::setw(2) << seconds;
    return out.str();
}

Duration Duration::operator+(const Duration& other) const {
    Duration result = *this;
    result += other;
    return result;
}

Duration& Duration::operator+=(const Duration& other) {
    m_totalSeconds += other.m_totalSeconds;
    return *this;
}

bool Duration::operator==(const Duration& other) const { return m_totalSeconds == other.m_totalSeconds; }
bool Duration::operator!=(const Duration& other) const { return !(*this == other); }
bool Duration::operator<(const Duration& other) const { return m_totalSeconds < other.m_totalSeconds; }
bool Duration::operator>(const Duration& other) const { return other < *this; }

std::ostream& operator<<(std::ostream& os, const Duration& d) {
    // Build the text in a temporary stream so we never change os's fill/width state.
    std::ostringstream text;
    text << (d.m_totalSeconds / 60) << ':' << std::setfill('0') << std::setw(2)
         << (d.m_totalSeconds % 60);
    return os << text.str();
}
