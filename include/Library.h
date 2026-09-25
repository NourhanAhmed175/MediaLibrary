#pragma once
#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "Duration.h"
#include "Media.h"

// The Library owns the catalogue of media.
// Items are held with shared_ptr because playlists share them too:
// removing an item from the Library never destroys an object a playlist still holds.
// This class never prints or reads input, so it can be driven from any UI or the demo.
class Library {
public:
    // Adds an item and returns its ID. Throws DuplicateMediaException if the title
    // (compared without case) is already in the library.
    int add(std::shared_ptr<Media> media);

    // Removes an item from the catalogue. Throws MediaNotFoundException if absent.
    void remove(int id);

    bool contains(int id) const;
    std::size_t size() const;

    std::shared_ptr<Media> findById(int id) const;                    // throws MediaNotFoundException
    std::shared_ptr<Media> findByTitle(const std::string& title) const; // exact, ignores case; throws

    // Partial, case-insensitive match. Returns an empty vector when nothing matches.
    std::vector<std::shared_ptr<Media>> searchByTitle(const std::string& text) const;

    std::vector<std::shared_ptr<Media>> all() const; 
    std::vector<std::shared_ptr<Media>> findByType(const std::string& type) const;  // "Song", "Video", ...                 // ordered by ID
    std::vector<std::shared_ptr<Media>> sortedByTitle() const;
    std::vector<std::shared_ptr<Media>> sortedByDuration() const;

    Duration totalDuration() const;

    // Non-owning view of an item. weak_ptr::use_count() tells how many shared_ptr
    // owners exist without adding one itself (used by the demo to show ownership).
    std::weak_ptr<Media> observe(int id) const;

private:
    std::map<int, std::shared_ptr<Media>> m_byId;   // ID -> media (this map is the owner)
    std::map<std::string, int> m_titleIndex;        // lower-case title -> ID (fast title lookup)
};
