#include "Playlist.h"
#include "MediaExceptions.h"
#include "MediaUtils.h"
#include <algorithm>

Playlist::Playlist(const std::string& name) : m_name(name) {
    if (name.empty()) {
        throw MediaException("Playlist name cannot be empty");
    }
}

void Playlist::add(std::shared_ptr<Media> media) {
    if (!media) {
        throw MediaException("Cannot add a null media item to a playlist");
    }
    if (contains(media->getId())) {
        throw DuplicateMediaException("\"" + media->getTitle() + "\" is already in playlist \"" +
                                      m_name + "\"");
    }
    m_items.push_back(std::move(media));
}

void Playlist::removeById(int id) {
    for (auto it = m_items.begin(); it != m_items.end(); ++it) {
        if ((*it)->getId() == id) {
            m_items.erase(it);
            return;
        }
    }
    throw MediaNotFoundException("No media with ID " + std::to_string(id) + " in playlist \"" +
                                 m_name + "\"");
}

bool Playlist::contains(int id) const {
    for (const auto& item : m_items) {
        if (item->getId() == id) {
            return true;
        }
    }
    return false;
}

std::size_t Playlist::size() const { return m_items.size(); }
bool Playlist::empty() const { return m_items.empty(); }

Duration Playlist::totalDuration() const {
    Duration total;
    for (const auto& item : m_items) {
        total += item->getDuration();
    }
    return total;
}

void Playlist::sortByTitle() {
    std::sort(m_items.begin(), m_items.end(), media_utils::compareByTitle);
}

void Playlist::sortByDuration() {
    std::sort(m_items.begin(), m_items.end(), media_utils::compareByDuration);
}

std::string Playlist::summary() const {
    return "Playlist \"" + m_name + "\": " + std::to_string(m_items.size()) +
           " item(s), total time " + totalDuration().toHMS();
}
