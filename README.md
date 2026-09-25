# Media Library and Playlists

Console management system for songs, videos, and podcasts, built for the CodeForge Bootcamp
(Full Track) capstone — **Project 7: Media Library and Playlists**.

You can create playlists, search the library by title, sort by title or duration, and see the
total playing time of a playlist. The project's memory spotlight is **`shared_ptr` ownership**:
playlists and the library share the same `Media` objects.

## Build and run

Requirements: a C++17 compiler (g++ / clang++ / MSVC) and CMake 3.10+.

```bash
mkdir build
cd build
cmake ..
cmake --build .

./demo         # runs every feature end to end, no input needed   (Windows: demo.exe)
./media_app    # interactive console menu                          (Windows: media_app.exe)
```

The project compiles with `-Wall -Wextra` (`/W4` on MSVC) and produces no warnings.
Check for leaks on Linux with `valgrind --leak-check=full ./demo`.

## Project structure

```
MediaLibrary/
├── CMakeLists.txt
├── .gitignore
├── README.md
├── include/                 (headers, all with #pragma once)
│   ├── MediaExceptions.h    custom exceptions
│   ├── Duration.h           VALUE CLASS  (minutes:seconds)
│   ├── Media.h              ABSTRACT BASE CLASS
│   ├── Song.h  Video.h  Podcast.h      concrete classes
│   ├── MediaUtils.h         toLower + std::sort comparison functions
│   ├── Library.h            owns the catalogue
│   ├── Playlist.h           HAS-A list of shared Media
│   ├── PlaylistManager.h    playlists by name
│   └── SampleData.h
└── src/                     (.cpp files + main.cpp menu + demo.cpp driver)
```

## Class design

| Class | Role |
|-------|------|
| `Media` (abstract) | Pure virtual `getType()` and `describe()`; virtual `play()` with a default; virtual destructor; `static` ID counter. Copying is deleted because every object has a unique ID. |
| `Song`, `Video`, `Podcast` | Concrete classes, each with its own metadata and its own `describe()` / `play()` (`override`). |
| `Duration` | Value class: `+`, `+=`, `==`, `!=`, `<`, `>`, friend `operator<<`. Rule of 0. |
| `Library` | Owns all media in `std::map<int, shared_ptr<Media>>` plus a `std::map<string,int>` title index. Search, sort, remove. |
| `Playlist` | HAS-A `vector<shared_ptr<Media>>`. Add, remove, total duration, sort. |
| `PlaylistManager` | `std::map<string, Playlist>`: create, get, remove playlists. |
| `MediaException` family | `MediaNotFoundException`, `DuplicateMediaException`, `InvalidDurationException`, `PlaylistNotFoundException` (all derive from `std::exception`). |

The core classes never read input and never print, so the demo driver and the menu both use them the same way.

## Features

- Add songs, videos, and podcasts to the library (unique ID for each)
- Search by title (exact and partial, ignoring case)
- Sort by title or by duration with `std::sort`
- Create playlists; add and remove media; see total playing time
- Play a playlist (each media type plays differently through a `Media` pointer)
- Removing an item from the library never breaks a playlist that still holds it
- Friendly error messages: the program does not crash on bad input

## Memory spotlight: `shared_ptr`

- The `Library` and every `Playlist` hold `shared_ptr<Media>` to the **same** object (no copies).
- `demo.cpp` (section 6) prints `use_count()` as an item is added to and removed from playlists.
  A `weak_ptr` observer is used so that reading the count does not change it.
- After `library.remove(id)`, playlists still hold and can play the item.
- When the last owner lets go, the object is destroyed automatically and the observer reports `expired() == true`.
- There is no raw `new` / `delete` anywhere: objects are created with `std::make_shared`.

## SOLID principles applied

- **Single Responsibility:** `Library` manages the catalogue, `Playlist` manages one list, `PlaylistManager` manages playlist names, `Duration` only handles time.
- **Open/Closed:** to add a new media type (e.g. `Audiobook`), write a new class that derives from `Media`. `Library`, `Playlist`, and the sorting code do not change.
- **Liskov Substitution:** `Song`, `Video`, and `Podcast` can be used anywhere a `Media` is expected (`Library` and `Playlist` only use the `Media` interface).
- **Dependency Inversion:** `Library` and `Playlist` depend on the abstract `Media`, not on concrete classes.

## Manual test checklist

Run `./media_app`. Menu option numbers are in brackets. Start with option `15` (load sample data) for cases 1-13.

| # | Input | Expected output |
|---|-------|-----------------|
| 1 | `15` | `Loaded 7 sample item(s).` (loading again prints `Loaded 0 sample item(s).`) |
| 2 | `4` | 7 items listed, IDs 1-7, header shows total time `2:06:55` |
| 3 | `1` -> title `Test Song`, 3 min 30 sec, artist `A`, album `B` | `Added song with ID 8.` |
| 4 | `1` -> title `Bad`, 3 min **75** sec | `Error: Invalid duration: seconds must be between 0 and 59 (got 75)`; nothing added |
| 5 | `1` -> title `blinding lights` (same title, different case) | `Error: The library already contains a title called "blinding lights"` |
| 6 | `5` -> `smart` | Only `Smart Pointers Weekly` (ID 6) is shown |
| 7 | `5` -> `zzz` | `(no results)` |
| 8 | `7` (sorted by duration) | Order: Blinding Lights 3:20, Habibi 4:30, Bohemian Rhapsody 5:55, Pointers 10:15, OOP 25:40, Tech Talk 32:05, Smart Pointers 45:10 |
| 9 | `6` (sorted by title) | Alphabetical: Blinding Lights, Bohemian Rhapsody, Habibi..., OOP Crash Course, Pointers..., Smart Pointers Weekly, Tech Talk Egypt |
| 10 | `9` -> `Mix`, then `9` -> `Mix` again | First: `Playlist "Mix" created.` Second: `Error: A playlist called "Mix" already exists` |
| 11 | `10` -> playlist `Mix`, media ID `2` | `Added. Owners of this item (library + playlists): 2` |
| 12 | `10` -> `Mix`, ID `2` again | `Error: "Blinding Lights" is already in playlist "Mix"` |
| 13 | `10` -> `Mix`, ID `99` | `Error: No media with ID 99 in the library` |
| 14 | `10` -> `Nope`, ID `2` | `Error: No playlist called "Nope"` |
| 15 | `12` -> `Mix` (after case 11) | `Playlist "Mix": 1 item(s), total time 0:03:20` and the item is listed |
| 16 | `8` -> ID `2`, then `12` -> `Mix` | Removed from library; the playlist **still** lists Blinding Lights |
| 17 | `14` -> `Mix` (after case 16) | `Now playing song: "Blinding Lights" by The Weeknd` (item still works) |
| 18 | `8` -> ID `999` | `Error: No media with ID 999 in the library` |
| 19 | At any prompt type `abc` | `Please enter a valid whole number.` and the prompt repeats; no crash |
| 20 | Close the input (Ctrl+D on Linux/macOS, Ctrl+Z then Enter on Windows) | `Input closed. Goodbye!` and a normal exit |

## Not used (optional stretch goals)

File I/O, templates, lambdas, `std::optional`, unit-test framework. The sort comparators are plain functions in `MediaUtils.cpp`.
