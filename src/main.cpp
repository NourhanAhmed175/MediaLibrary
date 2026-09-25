// Interactive console menu for the Media Library.
// All the real work happens in Library / Playlist / PlaylistManager; this file only
// reads input, calls those classes, and prints the results.
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Library.h"
#include "MediaExceptions.h"
#include "Playlist.h"
#include "PlaylistManager.h"
#include "Podcast.h"
#include "SampleData.h"
#include "Song.h"
#include "Video.h"

namespace {

// Thrown when stdin is closed (Ctrl+D / Ctrl+Z / end of a piped file) so we can exit cleanly.
struct EndOfInput {};

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw EndOfInput();
    }
    return line;
}

// Keeps asking until the user types a whole number.
int readInt(const std::string& prompt) {
    while (true) {
        const std::string line = readLine(prompt);
        try {
            std::size_t used = 0;
            const int value = std::stoi(line, &used);
            if (used == line.size()) {
                return value;
            }
        } catch (const std::invalid_argument&) {
            // not a number: fall through to the message below
        } catch (const std::out_of_range&) {
            // number too large for int: same message
        }
        std::cout << "  Please enter a valid whole number.\n";
    }
}

Duration readDuration() {
    const int minutes = readInt("Minutes: ");
    const int seconds = readInt("Seconds (0-59): ");
    return Duration(minutes, seconds);   // may throw InvalidDurationException
}

void printMedia(const std::vector<std::shared_ptr<Media>>& items) {
    if (items.empty()) {
        std::cout << "  (no results)\n";
        return;
    }
    for (const auto& item : items) {
        std::cout << "  " << item->describe() << "\n";
    }
}

void printMenu() {
    std::cout << "\n=========== Media Library ===========\n"
              << " 1) Add song\n"
              << " 2) Add video\n"
              << " 3) Add podcast\n"
              << " 4) List library\n"
              << " 5) Search by title\n"
              << " 6) List sorted by title\n"
              << " 7) List sorted by duration\n"
              << " 8) Remove media from library\n"
              << " 9) Create playlist\n"
              << "10) Add media to playlist\n"
              << "11) Remove media from playlist\n"
              << "12) Show playlist\n"
              << "13) List playlists\n"
              << "14) Play playlist\n"
              << "15) Load sample data\n"
              << " 0) Exit\n";
}

void addSong(Library& library) {
    const std::string title = readLine("Title: ");
    const Duration duration = readDuration();
    const std::string artist = readLine("Artist: ");
    const std::string album = readLine("Album: ");
    const int id = library.add(std::make_shared<Song>(title, duration, artist, album));
    std::cout << "Added song with ID " << id << ".\n";
}

void addVideo(Library& library) {
    const std::string title = readLine("Title: ");
    const Duration duration = readDuration();
    const std::string director = readLine("Director: ");
    const std::string resolution = readLine("Resolution (e.g. 1080p): ");
    const int id = library.add(std::make_shared<Video>(title, duration, director, resolution));
    std::cout << "Added video with ID " << id << ".\n";
}

void addPodcast(Library& library) {
    const std::string title = readLine("Title: ");
    const Duration duration = readDuration();
    const std::string host = readLine("Host: ");
    const int episode = readInt("Episode number: ");
    const int id = library.add(std::make_shared<Podcast>(title, duration, host, episode));
    std::cout << "Added podcast with ID " << id << ".\n";
}

void listLibrary(const Library& library) {
    std::cout << "Library (" << library.size() << " item(s), total time "
              << library.totalDuration().toHMS() << "):\n";
    printMedia(library.all());
}

void searchLibrary(const Library& library) {
    const std::string text = readLine("Search text: ");
    printMedia(library.searchByTitle(text));
}

void removeFromLibrary(Library& library) {
    const int id = readInt("Media ID to remove: ");
    library.remove(id);
    std::cout << "Removed from the library. Playlists that already hold it keep it alive.\n";
}

void createPlaylist(PlaylistManager& playlists) {
    const std::string name = readLine("New playlist name: ");
    playlists.create(name);
    std::cout << "Playlist \"" << name << "\" created.\n";
}

void addToPlaylist(Library& library, PlaylistManager& playlists) {
    const std::string name = readLine("Playlist name: ");
    const int id = readInt("Media ID to add: ");
    playlists.get(name).add(library.findById(id));
    std::cout << "Added. Owners of this item (library + playlists): "
              << library.observe(id).use_count() << "\n";
}

void removeFromPlaylist(PlaylistManager& playlists) {
    const std::string name = readLine("Playlist name: ");
    const int id = readInt("Media ID to remove: ");
    playlists.get(name).removeById(id);
    std::cout << "Removed from playlist \"" << name << "\".\n";
}

void showPlaylist(const PlaylistManager& playlists) {
    const std::string name = readLine("Playlist name: ");
    const Playlist& playlist = playlists.get(name);
    std::cout << playlist.summary() << "\n";
    printMedia(playlist.items());
}

void listPlaylists(const PlaylistManager& playlists) {
    const std::vector<std::string> names = playlists.names();
    if (names.empty()) {
        std::cout << "  (no playlists yet)\n";
        return;
    }
    for (const std::string& name : names) {
        std::cout << "  " << playlists.get(name).summary() << "\n";
    }
}

void playPlaylist(const PlaylistManager& playlists) {
    const std::string name = readLine("Playlist name: ");
    const Playlist& playlist = playlists.get(name);
    if (playlist.empty()) {
        std::cout << "  Playlist is empty.\n";
        return;
    }
    for (const auto& item : playlist.items()) {
        std::cout << "  " << item->play() << "\n";   // virtual call: each type plays differently
    }
}

}  // namespace

int main() {
    Library library;
    PlaylistManager playlists;

    try {
        bool running = true;
        while (running) {
            printMenu();
            const int choice = readInt("Choose an option: ");
            try {
                switch (choice) {
                    case 1:  addSong(library); break;
                    case 2:  addVideo(library); break;
                    case 3:  addPodcast(library); break;
                    case 4:  listLibrary(library); break;
                    case 5:  searchLibrary(library); break;
                    case 6:  printMedia(library.sortedByTitle()); break;
                    case 7:  printMedia(library.sortedByDuration()); break;
                    case 8:  removeFromLibrary(library); break;
                    case 9:  createPlaylist(playlists); break;
                    case 10: addToPlaylist(library, playlists); break;
                    case 11: removeFromPlaylist(playlists); break;
                    case 12: showPlaylist(playlists); break;
                    case 13: listPlaylists(playlists); break;
                    case 14: playPlaylist(playlists); break;
                    case 15:
                        std::cout << "Loaded " << loadSampleData(library) << " sample item(s).\n";
                        break;
                    case 0:  running = false; break;
                    default: std::cout << "Unknown option. Please choose from the menu.\n";
                }
            } catch (const MediaException& e) {
                // Our own errors: report them and go back to the menu instead of crashing.
                std::cout << "Error: " << e.what() << "\n";
            }
        }
        std::cout << "Goodbye!\n";
    } catch (const EndOfInput&) {
        std::cout << "\nInput closed. Goodbye!\n";
    } catch (const std::exception& e) {
        std::cout << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
