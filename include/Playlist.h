#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include "Duration.h"
#include "Media.h"

// A Playlist HAS-A list of media. It holds shared_ptr<Media>, so it shares ownership with
// the Library and with other playlists. Rule of 0: the compiler-generated copy/move/destructor
// are correct because shared_ptr already does the right thing (copying a playlist just adds owners).
class Playlist {
public:
    explicit Playlist(const std::string& name);

    const std::string& getName() const { return m_name; }

    // Throws DuplicateMediaException if the item is already in this playlist.
    void add(std::shared_ptr<Media> media);
    // Throws MediaNotFoundException if the item is not in this playlist.
    void removeById(int id);

    bool contains(int id) const;
    std::size_t size() const;
    bool empty() const;

    Duration totalDuration() const;            // sums the items with Duration::operator+
    const std::vector<std::shared_ptr<Media>>& items() const { return m_items; }

    void sortByTitle();
    void sortByDuration();

    std::string summary() const;               // e.g.  Playlist "Road Trip": 4 item(s), total 0:19:20

private:
    std::string m_name;
    std::vector<std::shared_ptr<Media>> m_items;
};
