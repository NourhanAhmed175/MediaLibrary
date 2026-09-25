# Walkthrough notes (for your own preparation — not part of the grade)

Practice answering these OUT LOUD in your own words before the walkthrough.

**1. Why is `~Media()` virtual? What if it were not?**
The library holds `shared_ptr<Media>` that really point to `Song`/`Video`/`Podcast`. Deleting a derived object
through a base pointer without a virtual destructor is undefined behaviour: the derived part
(e.g. the `m_artist` string) would not be destroyed properly. (`shared_ptr` created with `make_shared<Song>`
remembers the real type, but the rule still matters for any `Media*` / `unique_ptr<Media>`.)

**2. Why `shared_ptr` and not `unique_ptr` for `Media`?**
Several owners need the same object: the Library and one or more playlists. `unique_ptr` allows exactly one owner.

**3. What does `use_count()` mean and why use `weak_ptr` to read it?**
It is the number of `shared_ptr` owners. Holding a second `shared_ptr` in the demo would add 1 and hide the real number;
`weak_ptr::use_count()` reads the count without owning anything.

**4. What happens when an item is removed from the library but is still in a playlist?**
`Library::remove` only drops the library's `shared_ptr`. The count goes down by 1, the object stays alive because the playlist still holds it.
When the last owner releases it, the destructor runs automatically.

**5. Is copying a `Playlist` a deep copy or a shallow copy?**
The vector is copied (a new list), but the elements are `shared_ptr`s, so both playlists point to the SAME `Media` objects. That is intended.
The class follows the Rule of 0: no destructor / copy / move is written because every member already manages itself.

**6. Why is copying `Media` deleted?**
Each object has a unique ID from the `static` counter. A copy would have the same ID. Objects are shared with `shared_ptr` instead.

**7. Why does `Duration` follow the Rule of 0?**
It holds only one `int`. The compiler-generated copy/move/destructor are already correct.

**8. Why is `operator<<` a friend?**
It is a non-member function (its left operand is `std::ostream`, so it cannot be a member of `Duration`) and it needs private access to `m_totalSeconds`.

**9. How does `std::sort` know how to compare two media?**
I pass a comparison function (`compareByDuration`) that returns true if `a` should come before `b`. It uses `Duration::operator<` and breaks ties by ID so the order is stable.

**10. Which SOLID principle does adding `Audiobook` show?**
Open/Closed: write one new class deriving from `Media` and implement `getType()` and `describe()`. Nothing in `Library` or `Playlist` changes.

**11. What is the purpose of the `= 0` and what makes `Media` abstract?**
`getType()` and `describe()` are pure virtual, so `Media m("x", Duration(1,0));` does not compile. Each concrete class must override both or it stays abstract.

**12. Live-change practice:** add a new `Audiobook` class (author + narrator), add one sample item, and check that it appears in search, sorting, and playlists with no changes to other files.
