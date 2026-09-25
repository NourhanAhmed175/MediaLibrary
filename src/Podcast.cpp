#include "Podcast.h"
#include "MediaExceptions.h"

Podcast::Podcast(const std::string& title, const Duration& duration,
                 const std::string& host, int episodeNumber)
    : Media(title, duration), m_host(host), m_episodeNumber(episodeNumber) {
    if (episodeNumber <= 0) {
        throw MediaException("Podcast episode number must be positive");
    }
}

std::string Podcast::getType() const { return "Podcast"; }

std::string Podcast::describe() const {
    return baseInfo() + " - Host: " + m_host + " | Episode " + std::to_string(m_episodeNumber);
}

std::string Podcast::play() const {
    return "Now playing podcast episode " + std::to_string(m_episodeNumber) + ": \"" +
           getTitle() + "\" with " + m_host;
}
