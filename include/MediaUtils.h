#pragma once
#include <memory>
#include <string>
#include "Media.h"

// Small helpers shared by Library and Playlist.
namespace media_utils {

std::string toLower(const std::string& text);

// Comparison functions used with std::sort (strict weak ordering, ties broken by ID).
bool compareByTitle(const std::shared_ptr<Media>& a, const std::shared_ptr<Media>& b);
bool compareByDuration(const std::shared_ptr<Media>& a, const std::shared_ptr<Media>& b);

}  // namespace media_utils
