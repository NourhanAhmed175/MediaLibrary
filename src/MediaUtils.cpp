#include "MediaUtils.h"
#include <cctype>

namespace media_utils {

std::string toLower(const std::string& text) {
    std::string result = text;
    for (char& c : result) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

bool compareByTitle(const std::shared_ptr<Media>& a, const std::shared_ptr<Media>& b) {
    const std::string ta = toLower(a->getTitle());
    const std::string tb = toLower(b->getTitle());
    if (ta == tb) {
        return a->getId() < b->getId();
    }
    return ta < tb;
}

bool compareByDuration(const std::shared_ptr<Media>& a, const std::shared_ptr<Media>& b) {
    if (a->getDuration() == b->getDuration()) {
        return a->getId() < b->getId();
    }
    return a->getDuration() < b->getDuration();   // uses Duration::operator<
}

}  // namespace media_utils
