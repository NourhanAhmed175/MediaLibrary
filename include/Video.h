#pragma once
#include "Media.h"

class Video : public Media {
public:
    Video(const std::string& title, const Duration& duration,
          const std::string& director, const std::string& resolution);

    std::string getType() const override;
    std::string describe() const override;
    std::string play() const override;

    const std::string& getDirector() const { return m_director; }
    const std::string& getResolution() const { return m_resolution; }

private:
    std::string m_director;
    std::string m_resolution;
};
