#include "Audiobook.h"

Audiobook::Audiobook(const std::string& title, const Duration& duration,
                     const std::string& author, const std::string& narrator)
    : Media(title, duration), m_author(author), m_narrator(narrator) {}

std::string Audiobook::getType() const { return "Audiobook"; }

std::string Audiobook::describe() const {
    return baseInfo() + " - By " + m_author + " | Narrated by " + m_narrator;
}

std::string Audiobook::play() const {
    return "Now playing audiobook: \"" + getTitle() + "\" narrated by " + m_narrator;
}