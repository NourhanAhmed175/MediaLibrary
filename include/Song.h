#pragma once
#include "Media.h"

class Song : public Media {
public:
    Song(const std::string& title, const Duration& duration,
         const std::string& artist, const std::string& album);

    std::string getType() const override;
    std::string describe() const override;
    std::string play() const override;

    const std::string& getArtist() const { return m_artist; }
    const std::string& getAlbum() const { return m_album; }

private:
    std::string m_artist;
    std::string m_album;
};
