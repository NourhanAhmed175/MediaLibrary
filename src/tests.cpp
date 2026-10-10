// Small self-checking test program (no external framework needed).
// Build target: tests   Run: ./tests   -> exit code 0 means every check passed.
#include <iostream>
#include <memory>
#include <string>

#include "Audiobook.h"
#include "Library.h"
#include "MediaExceptions.h"
#include "PlaylistManager.h"
#include "Song.h"
#include "Video.h"
#include "Podcast.h"

void testMediaTypes() {
    Podcast podcast("Tech Talk", Duration(30, 0), "Sara", 5);
    check(podcast.getType() == "Podcast", "Podcast reports its type");
    check(podcast.describe().find("Episode 5") != std::string::npos, "Podcast describe shows the episode");
    check(podcast.play().find("episode 5") != std::string::npos, "Podcast play mentions the episode");
    check(throwsType<MediaException>([] { Podcast("Bad", Duration(10, 0), "Host", 0); }),
          "podcast episode number 0 rejected");

    Video video("Intro to C++", Duration(12, 30), "Eng. Karim", "1080p");
    check(video.getType() == "Video", "Video reports its type");
    check(video.describe().find("Directed by Eng. Karim") != std::string::npos, "Video describe shows the director");
    check(video.play().find("1080p") != std::string::npos, "Video play mentions the resolution");

    Song song("Blinding Lights", Duration(3, 20), "The Weeknd", "After Hours");
    check(song.getType() == "Song", "Song reports its type");
    check(song.getArtist() == "The Weeknd", "Song keeps its artist");

    check(throwsType<MediaException>([] { Song("", Duration(1, 0), "A", "B"); }), "empty title rejected");
    check(throwsType<InvalidDurationException>([] { Song("Silent", Duration(0, 0), "A", "B"); }),
          "zero length rejected");

    const int before = Media::getCreatedCount();
    try {
        Song("", Duration(1, 0), "A", "B");
    } catch (const MediaException&) {
    }
    check(Media::getCreatedCount() == before, "a failed constructor does not use up an ID");
}
namespace {

int g_failed = 0;
int g_total = 0;

void check(bool condition, const std::string& name) {
    ++g_total;
    if (!condition) {
        ++g_failed;
        std::cout << "  FAIL: " << name << "\n";
    }
}

template <typename ExceptionType, typename Function>
bool throwsType(Function fn) {
    try {
        fn();
    } catch (const ExceptionType&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

void testDuration() {
    check(Duration(3, 45) + Duration(2, 30) == Duration(6, 15), "Duration addition carries seconds");
    check(Duration(1, 0) < Duration(1, 1), "Duration operator<");
    check(Duration(0, 0).toString() == "0:00", "Duration 0:00 formatting");
    check(Duration(126, 55).toHMS() == "2:06:55", "Duration h:mm:ss formatting");
    check(throwsType<InvalidDurationException>([] { Duration(1, 75); }), "seconds above 59 rejected");
    check(throwsType<InvalidDurationException>([] { Duration(-2, 0); }), "negative minutes rejected");
}

void testLibrary() {
    Library library;
    const int id = library.add(std::make_shared<Song>("Blinding Lights", Duration(3, 20), "The Weeknd", "After Hours"));
    library.add(std::make_shared<Video>("OOP Crash Course", Duration(25, 40), "Eng. Karim", "720p"));

    check(library.size() == 2, "library size after two adds");
    check(library.findByTitle("BLINDING lights")->getId() == id, "title lookup ignores case");
    check(library.searchByTitle("crash").size() == 1, "partial search finds one item");
    check(library.searchByTitle("zzz").empty(), "partial search with no match is empty");
    check(library.findByType("Video").size() == 1, "findByType finds the video");
    check(throwsType<DuplicateMediaException>([&] {
              library.add(std::make_shared<Song>("blinding lights", Duration(1, 0), "X", "Y"));
          }), "duplicate title rejected");
    check(throwsType<MediaNotFoundException>([&] { library.remove(9999); }), "remove unknown ID throws");
    check(library.sortedByDuration().front()->getId() == id, "shortest item sorts first by duration");
}

void testSharedOwnership() {
    Library library;
    PlaylistManager playlists;
    const int id = library.add(std::make_shared<Audiobook>("Clean Code", Duration(95, 30), "R. Martin", "Narrator"));
    playlists.create("Commute").add(library.findById(id));

    std::weak_ptr<Media> observer = library.observe(id);
    check(observer.use_count() == 2, "library + playlist own the item");

    library.remove(id);
    check(!observer.expired(), "playlist keeps the item alive after library removal");
    check(playlists.get("Commute").items().front()->play().find("Clean Code") != std::string::npos,
          "item still plays after library removal");

    playlists.remove("Commute");
    check(observer.expired(), "item destroyed when the last owner lets go");
}

}  // namespace

int main() {
    testMediaTypes();
    testDuration();
    testLibrary();
    testSharedOwnership();
    std::cout << (g_total - g_failed) << "/" << g_total << " checks passed.\n";
    return g_failed == 0 ? 0 : 1;
}
