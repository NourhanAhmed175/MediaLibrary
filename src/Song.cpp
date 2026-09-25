#include "Song.h"

Song::Song(const std::string& title, const Duration& duration,
           const std::string& artist, const std::string& album)
    : Media(title, duration), m_artist(artist), m_album(album) {}

std::string Song::getType() const { return "Song"; }

std::string Song::describe() const {
    return baseInfo() + " - " + m_artist + " | Album: " + m_album;
}

std::string Song::play() const {
    return "Now playing song: \"" + getTitle() + "\" by " + m_artist;
}
