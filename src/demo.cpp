// Demo driver: runs every feature end to end with NO menu input.
// Build it with CMake and run ./demo   (or run it under valgrind to check for leaks).
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Library.h"
#include "MediaExceptions.h"
#include "PlaylistManager.h"
#include "SampleData.h"
#include "Song.h"

namespace {

void section(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

void printAll(const std::vector<std::shared_ptr<Media>>& items) {
    for (const auto& item : items) {
        std::cout << "  " << item->describe() << "\n";
    }
}

void showCount(const std::string& label, const std::weak_ptr<Media>& observer) {
    std::cout << "  " << label << "  ->  use_count = " << observer.use_count() << "\n";
}

}  // namespace

int main() {
    Library library;
    PlaylistManager playlists;

    // ------------------------------------------------------------------
    section("1. Library owns polymorphic Media objects (Song / Video / Podcast)");
    std::cout << "  Loaded " << loadSampleData(library) << " items.\n";
    printAll(library.all());
    const Duration libraryTime = library.totalDuration();
    std::cout << "  Total library time: " << libraryTime << "  (" << libraryTime.toHMS() << ")\n";
    std::cout << "  Media objects created so far (static counter): " << Media::getCreatedCount() << "\n";

    // ------------------------------------------------------------------
    section("2. Search by title (std::map index)");
    std::cout << "  Exact search, ignoring case: \"blinding LIGHTS\"\n";
    std::cout << "    " << library.findByTitle("blinding LIGHTS")->describe() << "\n";
    std::cout << "  Partial search: \"pointers\"\n";
    printAll(library.searchByTitle("pointers"));
    try {
        library.findByTitle("Song That Does Not Exist");
    } catch (const MediaNotFoundException& e) {
        std::cout << "  Caught MediaNotFoundException: " << e.what() << "\n";
    }

    // ------------------------------------------------------------------
    section("3. Sorting with std::sort");
    std::cout << "  By title:\n";
    printAll(library.sortedByTitle());
    std::cout << "  By duration (uses Duration::operator<):\n";
    printAll(library.sortedByDuration());

    // ------------------------------------------------------------------
    section("4. Duration value class: operators");
    const Duration a(3, 45);
    const Duration b(2, 30);
    std::cout << "  a = " << a << ", b = " << b << "\n";
    std::cout << "  a + b  = " << (a + b) << "\n";
    std::cout << "  a < b  = " << (a < b ? "true" : "false") << "\n";
    std::cout << "  a == b = " << (a == b ? "true" : "false") << "\n";
    std::cout << "  a == Duration(3, 45) = " << (a == Duration(3, 45) ? "true" : "false") << "\n";
    try {
        Duration bad(1, 75);
    } catch (const InvalidDurationException& e) {
        std::cout << "  Caught InvalidDurationException: " << e.what() << "\n";
    }

    // ------------------------------------------------------------------
    section("5. Playlists share Media objects");
    Playlist& roadTrip = playlists.create("Road Trip");
    Playlist& study = playlists.create("Study Time");

    roadTrip.add(library.findByTitle("Bohemian Rhapsody"));
    roadTrip.add(library.findByTitle("Blinding Lights"));
    roadTrip.add(library.findByTitle("Habibi Ya Nour El Ain"));
    roadTrip.add(library.findByTitle("Smart Pointers Weekly"));

    study.add(library.findByTitle("Smart Pointers Weekly"));   // the SAME object as in Road Trip
    study.add(library.findByTitle("Pointers in 10 Minutes"));
    study.add(library.findByTitle("OOP Crash Course"));

    for (const std::string& name : playlists.names()) {
        const Playlist& p = playlists.get(name);
        std::cout << "  " << p.summary() << "\n";
        printAll(p.items());
    }

    // ------------------------------------------------------------------
    section("6. Memory spotlight: shared_ptr use_count and removal from the library");
    const int trackId = library.add(std::make_shared<Song>("Shared Demo Track", Duration(3, 0),
                                                           "Demo Artist", "Demo Album"));
    // weak_ptr observes without owning, so its use_count() is the number of real owners.
    std::weak_ptr<Media> observer = library.observe(trackId);

    showCount("Just added to the library            ", observer);
    roadTrip.add(library.findById(trackId));
    showCount("Added to 'Road Trip'                 ", observer);
    study.add(library.findById(trackId));
    showCount("Added to 'Study Time'                ", observer);
    roadTrip.removeById(trackId);
    showCount("Removed from 'Road Trip'             ", observer);

    library.remove(trackId);
    showCount("Removed from the LIBRARY             ", observer);
    std::cout << "  Still in library?      " << (library.contains(trackId) ? "yes" : "no") << "\n";
    std::cout << "  Still in 'Study Time'? " << (study.contains(trackId) ? "yes" : "no") << "\n";
    std::cout << "  The playlist can still use it: " << study.items().back()->play() << "\n";
    std::cout << "  observer.expired() = " << (observer.expired() ? "true" : "false") << "\n";

    study.removeById(trackId);
    showCount("Removed from the last playlist       ", observer);
    std::cout << "  observer.expired() = " << (observer.expired() ? "true" : "false")
              << "   (last owner gone -> object destroyed automatically)\n";

    // Deleting a whole playlist also releases its shared owners.
    std::weak_ptr<Media> smart = library.observe(library.findByTitle("Smart Pointers Weekly")->getId());
    showCount("'Smart Pointers Weekly' (library + 2 playlists)", smart);
    playlists.remove("Road Trip");
    showCount("After deleting the 'Road Trip' playlist         ", smart);

    // ------------------------------------------------------------------
    section("7. Custom exceptions thrown and caught");
    try {
        study.add(library.findByTitle("OOP Crash Course"));
    } catch (const DuplicateMediaException& e) {
        std::cout << "  Caught DuplicateMediaException: " << e.what() << "\n";
    }
    try {
        library.remove(9999);
    } catch (const MediaNotFoundException& e) {
        std::cout << "  Caught MediaNotFoundException: " << e.what() << "\n";
    }
    try {
        playlists.get("Nope");
    } catch (const PlaylistNotFoundException& e) {
        std::cout << "  Caught PlaylistNotFoundException: " << e.what() << "\n";
    }
    try {
        library.add(std::make_shared<Song>("blinding lights", Duration(1, 0), "X", "Y"));
    } catch (const MediaException& e) {   // catches any of our exceptions through the base class
        std::cout << "  Caught MediaException (base class): " << e.what() << "\n";
    }

    // ------------------------------------------------------------------
    section("8. Polymorphism: same call, different behaviour");
    for (const auto& item : study.items()) {
        std::cout << "  " << item->play() << "\n";
    }

    std::cout << "\nDemo finished. Everything is released automatically (no new/delete in this project).\n";
    return 0;
}
