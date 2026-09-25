#pragma once
#include <string>
#include "Duration.h"

// Abstract base class: the "contract" every kind of media must fulfil.
// Song, Video and Podcast provide getType() and describe().
class Media {
public:
    Media(const std::string& title, const Duration& duration);
    virtual ~Media() = default;               // virtual: objects are deleted through Media pointers

    // Every media object has a unique ID, so copying one would create a duplicate ID.
    // Media objects are shared through shared_ptr instead of being copied.
    Media(const Media&) = delete;
    Media& operator=(const Media&) = delete;

    int getId() const { return m_id; }
    const std::string& getTitle() const { return m_title; }
    const Duration& getDuration() const { return m_duration; }

    // --- Pure virtual: every concrete class MUST implement these ---
    virtual std::string getType() const = 0;
    virtual std::string describe() const = 0;

    // --- Non-pure virtual: default behaviour that derived classes MAY override ---
    virtual std::string play() const;

    // --- Static member: class-level counter ---
    static int getCreatedCount();             // how many Media objects were created so far

protected:
    // Regular helper used by every describe(): "[Type #id] Title (m:ss)"
    std::string baseInfo() const;

private:
    static int s_nextId;

    int m_id;
    std::string m_title;
    Duration m_duration;
};
