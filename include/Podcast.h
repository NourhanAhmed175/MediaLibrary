#pragma once
#include "Media.h"

class Podcast : public Media {
public:
    Podcast(const std::string& title, const Duration& duration,
            const std::string& host, int episodeNumber);

    std::string getType() const override;
    std::string describe() const override;
    std::string play() const override;

    const std::string& getHost() const { return m_host; }
    int getEpisodeNumber() const { return m_episodeNumber; }

private:
    std::string m_host;
    int m_episodeNumber;
};
