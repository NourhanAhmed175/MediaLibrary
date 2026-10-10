#pragma once
#include "Media.h"

// An audiobook: adds the author's name and the narrator's name.
class Audiobook : public Media {
public:
    Audiobook(const std::string& title, const Duration& duration,
              const std::string& author, const std::string& narrator);

    std::string getType() const override;
    std::string describe() const override;
    std::string play() const override;

    const std::string& getAuthor() const { return m_author; }
    const std::string& getNarrator() const { return m_narrator; }

private:
    std::string m_author;
    std::string m_narrator;
};