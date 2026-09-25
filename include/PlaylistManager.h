#pragma once
#include <map>
#include <string>
#include <vector>
#include "Playlist.h"

// Keeps track of the playlists by name (separate from Library: one class = one job).
class PlaylistManager {
public:
    Playlist& create(const std::string& name);       // throws DuplicateMediaException if the name is taken
    Playlist& get(const std::string& name);          // throws PlaylistNotFoundException
    const Playlist& get(const std::string& name) const;
    void remove(const std::string& name);            // throws PlaylistNotFoundException
    bool exists(const std::string& name) const;
    std::vector<std::string> names() const;

private:
    std::map<std::string, Playlist> m_playlists;
};
