#include "Library.h"
#include "MediaExceptions.h"
#include "MediaUtils.h"
#include <algorithm>

int Library::add(std::shared_ptr<Media> media) {
    if (!media) {
        throw MediaException("Cannot add a null media item to the library");
    }
    const std::string key = media_utils::toLower(media->getTitle());
    if (m_titleIndex.count(key) > 0) {
        throw DuplicateMediaException("The library already contains a title called \"" +
                                      media->getTitle() + "\"");
    }
    const int id = media->getId();
    m_titleIndex[key] = id;
    m_byId[id] = std::move(media);
    return id;
}

void Library::remove(int id) {
    auto it = m_byId.find(id);
    if (it == m_byId.end()) {
        throw MediaNotFoundException("No media with ID " + std::to_string(id) + " in the library");
    }
    // Erase the title index entry first, while the item is still reachable.
    m_titleIndex.erase(media_utils::toLower(it->second->getTitle()));
    m_byId.erase(it);   // drops the library's shared_ptr; playlists keep theirs
}

bool Library::contains(int id) const { return m_byId.count(id) > 0; }

std::size_t Library::size() const { return m_byId.size(); }

std::shared_ptr<Media> Library::findById(int id) const {
    auto it = m_byId.find(id);
    if (it == m_byId.end()) {
        throw MediaNotFoundException("No media with ID " + std::to_string(id) + " in the library");
    }
    return it->second;
}

std::shared_ptr<Media> Library::findByTitle(const std::string& title) const {
    auto it = m_titleIndex.find(media_utils::toLower(title));
    if (it == m_titleIndex.end()) {
        throw MediaNotFoundException("No media titled \"" + title + "\" in the library");
    }
    return findById(it->second);
}

std::vector<std::shared_ptr<Media>> Library::searchByTitle(const std::string& text) const {
    const std::string needle = media_utils::toLower(text);
    std::vector<std::shared_ptr<Media>> results;
    for (const auto& entry : m_byId) {
        if (media_utils::toLower(entry.second->getTitle()).find(needle) != std::string::npos) {
            results.push_back(entry.second);
        }
    }
    return results;
}

std::vector<std::shared_ptr<Media>> Library::all() const {
    std::vector<std::shared_ptr<Media>> items;
    items.reserve(m_byId.size());
    for (const auto& entry : m_byId) {
        items.push_back(entry.second);
    }
    return items;
}

std::vector<std::shared_ptr<Media>> Library::sortedByTitle() const {
    std::vector<std::shared_ptr<Media>> items = all();
    std::sort(items.begin(), items.end(), media_utils::compareByTitle);
    return items;
}

std::vector<std::shared_ptr<Media>> Library::sortedByDuration() const {
    std::vector<std::shared_ptr<Media>> items = all();
    std::sort(items.begin(), items.end(), media_utils::compareByDuration);
    return items;
}

Duration Library::totalDuration() const {
    Duration total;
    for (const auto& entry : m_byId) {
        total += entry.second->getDuration();
    }
    return total;
}

std::weak_ptr<Media> Library::observe(int id) const {
    return findById(id);   // shared_ptr temporary -> weak_ptr, temporary is released immediately
}
