#include "Video.h"

Video::Video(const std::string& title, const Duration& duration,
             const std::string& director, const std::string& resolution)
    : Media(title, duration), m_director(director), m_resolution(resolution) {}

std::string Video::getType() const { return "Video"; }

std::string Video::describe() const {
    return baseInfo() + " - Directed by " + m_director + " | " + m_resolution;
}

std::string Video::play() const {
    return "Now playing video: \"" + getTitle() + "\" in " + m_resolution;
}
