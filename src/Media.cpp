#include "Media.h"
#include "MediaExceptions.h"

int Media::s_nextId = 1;

Media::Media(const std::string& title, const Duration& duration)
    : m_id(0), m_title(title), m_duration(duration) {
    if (title.empty()) {
        throw MediaException("Media title cannot be empty");
    }
    if (duration == Duration()) {
        throw InvalidDurationException("Invalid duration: media must be longer than 0:00");
    }
    // Only take an ID once validation has passed, so failed objects don't waste IDs.
    m_id = s_nextId++;
}

std::string Media::play() const {
    return "Playing " + getType() + ": " + m_title + " [" + m_duration.toString() + "]";
}

int Media::getCreatedCount() { return s_nextId - 1; }

std::string Media::baseInfo() const {
    return "[" + getType() + " #" + std::to_string(m_id) + "] " + m_title +
           " (" + m_duration.toString() + ")";
}
