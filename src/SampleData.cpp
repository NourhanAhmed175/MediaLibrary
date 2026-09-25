#include "SampleData.h"
#include "MediaExceptions.h"
#include "Podcast.h"
#include "Song.h"
#include "Video.h"

namespace {

// Returns true if the item was added, false if its title was already in the library.
bool addIfNew(Library& library, std::shared_ptr<Media> media) {
    try {
        library.add(std::move(media));
        return true;
    } catch (const DuplicateMediaException&) {
        return false;
    }
}

}  // namespace

int loadSampleData(Library& library) {
    int added = 0;
    added += addIfNew(library, std::make_shared<Song>("Bohemian Rhapsody", Duration(5, 55), "Queen", "A Night at the Opera")) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Song>("Blinding Lights", Duration(3, 20), "The Weeknd", "After Hours")) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Song>("Habibi Ya Nour El Ain", Duration(4, 30), "Amr Diab", "Nour El Ain")) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Video>("Pointers in 10 Minutes", Duration(10, 15), "ECU Media Lab", "1080p")) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Video>("OOP Crash Course", Duration(25, 40), "Eng. Karim", "720p")) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Podcast>("Smart Pointers Weekly", Duration(45, 10), "Dr. Sara", 12)) ? 1 : 0;
    added += addIfNew(library, std::make_shared<Podcast>("Tech Talk Egypt", Duration(32, 5), "Omar", 3)) ? 1 : 0;
    return added;
}
