#pragma once
#include <exception>
#include <string>

// Base class for every error thrown by this project.
// Deriving from std::exception lets generic code catch it with
// catch (const std::exception&) while our own code can catch the precise type.
class MediaException : public std::exception {
public:
    explicit MediaException(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};

// A media item (by ID or title) does not exist in the library / playlist.
class MediaNotFoundException : public MediaException {
public:
    explicit MediaNotFoundException(const std::string& message) : MediaException(message) {}
};

// A title already exists in the library, or an item is already in a playlist.
class DuplicateMediaException : public MediaException {
public:
    explicit DuplicateMediaException(const std::string& message) : MediaException(message) {}
};

// A Duration was built with impossible values (e.g. 1:75 or a negative time).
class InvalidDurationException : public MediaException {
public:
    explicit InvalidDurationException(const std::string& message) : MediaException(message) {}
};

// A playlist name does not exist.
class PlaylistNotFoundException : public MediaException {
public:
    explicit PlaylistNotFoundException(const std::string& message) : MediaException(message) {}
};
