#include "PlaylistManager.h"
#include "MediaExceptions.h"

Playlist& PlaylistManager::create(const std::string& name) {
    auto result = m_playlists.emplace(name, Playlist(name));
    if (!result.second) {
        throw DuplicateMediaException("A playlist called \"" + name + "\" already exists");
    }
    return result.first->second;
}

Playlist& PlaylistManager::get(const std::string& name) {
    auto it = m_playlists.find(name);
    if (it == m_playlists.end()) {
        throw PlaylistNotFoundException("No playlist called \"" + name + "\"");
    }
    return it->second;
}

const Playlist& PlaylistManager::get(const std::string& name) const {
    auto it = m_playlists.find(name);
    if (it == m_playlists.end()) {
        throw PlaylistNotFoundException("No playlist called \"" + name + "\"");
    }
    return it->second;
}

void PlaylistManager::remove(const std::string& name) {
    if (m_playlists.erase(name) == 0) {
        throw PlaylistNotFoundException("No playlist called \"" + name + "\"");
    }
}

bool PlaylistManager::exists(const std::string& name) const {
    return m_playlists.count(name) > 0;
}

std::vector<std::string> PlaylistManager::names() const {
    std::vector<std::string> result;
    for (const auto& entry : m_playlists) {
        result.push_back(entry.first);
    }
    return result;
}
