#pragma once

/**
@file
@brief Built-in Action Replay cheats for the Xbox shell.

The cheats are plain Action Replay codes, but they are not run through an emulated Action Replay cartridge. The
`CheatEngine` writes the values straight into the Saturn's RAM after every emulated frame, the same thing the
cartridge does in its interrupt hook. Because of that the "master code" (the `F6.../B6...` lines that install the
cartridge's hook into the game) is not needed and is ignored. Master codes are the usual reason Saturn codes fail in
emulators, so skipping them makes the codes work on more discs.

Supported code types (the first hex digit of the address word):
- `1` and `0`: write a 16-bit value (`0` is used by the "NOP patch" codes, value `0009`)
- `3`: write an 8-bit value (the low byte of the value word)
- `D`: only apply the next code when the 16-bit value at the address equals the value word
- `E` (this app's own, not an Action Replay type): hold the 16-bit value at the highest it has been since the cheat was
  switched on. For bars whose full value differs between discs: switch it on while the bar is full and it stays full
- `C` (this app's own): write an 8-bit value once, when the cheat is switched on (for flags the game clears itself)
- `F` and `B`: master code lines, ignored

Only compiled when YMIR_PLATFORM_UWP is set.
*/

#include <ymir/core/types.hpp>
#include <ymir/sys/bus.hpp>

#include <atomic>
#include <filesystem>
#include <optional>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace platform::uwp::shell {

/// @brief One cheat of a game, shown as an on/off switch.
struct CheatCode {
    const char *name;  ///< label of the switch
    const char *codes; ///< Action Replay codes, as printed in the guides (space separated hex words)
    int group = 0;     ///< cheats with the same nonzero group are exclusive: turning one on turns the others off
};

/// @brief A cheat the game has built in, entered with the buttons on the pause screen. The app presses them itself.
struct ButtonCode {
    const char *name;     ///< label of the menu item
    const char *sequence; ///< Saturn buttons, space separated: Up Down Left Right A B C X Y Z L R Start
    const char *note;     ///< what the player must do first, shown as the help text
};

/// @brief The cheats of one game.
struct CheatGame {
    const char *name;    ///< game name shown in the menu
    const char *version; ///< the disc the codes were made for, e.g. "USA"
    /// Words that must all appear in the disc's title (uppercase, letters and digits only) for an automatic match.
    std::vector<const char *> titleKeys;
    /// Words that must not appear in the disc's title (to tell apart sequels and spin-offs).
    std::vector<const char *> excludeKeys;
    std::vector<CheatCode> cheats;
    std::vector<ButtonCode> buttonCodes{}; ///< the game's own button-code cheats
};

/// @brief Presses a button-code cheat on the Saturn's port 1 for the player: pauses the game with Start, presses the
/// buttons one by one, then unpauses. Started from the GUI thread, stepped by the emulator thread after every frame.
class ButtonMacro {
public:
    /// @brief Parses a sequence ("Right Down A Start ...") and queues it.
    /// @return `false` if a button name is unknown or a macro is already running
    bool Start(std::string_view sequence, std::string name);

    [[nodiscard]] bool Running() const {
        return m_running.load();
    }

    /// @brief Holds the next button (emulator thread).
    /// @param[out] held receives the override for the port 1 pad (see SharedContext::shellPadOverride)
    void Step(std::atomic<uint32> &held);

    /// @brief The name of the code that just finished, once (GUI thread).
    [[nodiscard]] std::optional<std::string> TakeFinished();

private:
    struct Press {
        uint16 buttons; // Saturn buttons, active low (0xFFFF = none)
        int frames;
    };
    std::atomic_bool m_running{false};
    std::mutex m_mutex;
    std::vector<Press> m_presses;
    std::size_t m_index = 0;
    int m_frame = 0;
    std::string m_name;
    std::optional<std::string> m_finished;
};

/// @brief The built-in cheat list.
const std::vector<CheatGame> &CheatGames();

/// @brief Finds the game whose codes fit a disc, from the title in the disc header.
/// @param[in] headerTitle the game title from the disc header
/// @return the index into `CheatGames()`, or -1 when no game matches
int MatchCheatGame(std::string_view headerTitle);

/// @brief Finds a game by the product number in the disc header (every region and disc of it, from the Redump list).
/// @param[in] productNumber e.g. "MK-81022" or "T-15911H"
/// @return the index into `CheatGames()`, or -1 when the number is not known
int MatchCheatGameBySerial(std::string_view productNumber);

/// @brief Finds a game by product number first (exact), then by title.
int MatchCheatGame(std::string_view productNumber, std::string_view headerTitle);

/// @brief The name of the box art built into the app for a disc (e.g. "bug-too" for covers/bug-too.jpg).
/// @param[in] headerTitle the game title from the disc header
/// @return the name without extension, or an empty string when the disc is not one of the built-in games
std::string BundledCoverName(std::string_view headerTitle);

/// @brief Same, finding the game by product number first, then by title.
std::string BundledCoverName(std::string_view productNumber, std::string_view headerTitle);

/// @brief Applies the active codes to the Saturn's memory. Shared by the GUI thread (which sets the codes) and the
/// emulator thread (which applies them after every frame).
class CheatEngine {
public:
    /// @brief One parsed code line.
    struct Op {
        enum class Kind { Write8, Write16, IfEqual16, HoldHighest16, WriteOnce8 };
        Kind kind;
        uint32 address;
        uint16 value;
        uint16 highest = 0; // HoldHighest16: the highest value seen so far
        bool seen = false; // HoldHighest16: a value was learned; WriteOnce8: already written
    };

    /// @brief Parses Action Replay codes.
    /// @param[in] codes the codes, as hex words separated by spaces or new lines
    /// @param[out] out receives the parsed lines (master code lines are skipped)
    /// @return `false` if a word is not a valid code
    static bool Parse(std::string_view codes, std::vector<Op> &out);

    /// @brief Replaces the active codes. Called from the GUI thread.
    void SetActive(std::vector<Op> ops);

    /// @brief Writes the active codes into memory. Called from the emulator thread after every frame.
    void Apply(ymir::sys::SH2Bus &bus);

private:
    std::mutex m_mutex;
    std::vector<Op> m_ops;
};

/// @brief A simple cheat finder: snapshots the Saturn's work RAM and narrows it down to the 16-bit values that changed
/// the way the player describes (for example "my energy went down"), so a code can be made for a game whose published
/// codes do not work. Used from the GUI thread while the emulator is paused.
class CodeFinder {
public:
    enum class Filter { Decreased, Increased, Unchanged, Changed };

    /// @brief Starts a new search with every 16-bit value of both work RAM areas.
    void Start(ymir::sys::SH2Bus &bus);

    /// @brief Keeps only the addresses whose value changed as described since the last step.
    void Keep(ymir::sys::SH2Bus &bus, Filter filter);

    void Clear();

    [[nodiscard]] bool Active() const {
        return m_active;
    }
    [[nodiscard]] std::size_t Count() const {
        return m_addresses.size();
    }

    /// @brief Action Replay codes that hold every remaining address at the value it had when the search started.
    [[nodiscard]] std::string LockCodes() const;

private:
    bool m_active = false;
    std::vector<uint32> m_addresses;
    std::vector<uint16> m_first; // values when the search started (e.g. full energy)
    std::vector<uint16> m_last;  // values at the previous step
};

/// @brief The hands-free version of the code finder, for a number that only goes down when the player is hurt (like
/// energy). It runs on the emulator thread: while the player plays it drops every value that goes up or keeps falling
/// on its own, and each time the player presses the "I was hit" button it keeps only the values that went down since
/// the last press. When few enough are left it makes a code that holds them at their starting (full) value.
class AutoFinder {
public:
    /// @brief What the player sees on screen.
    struct Status {
        bool searching = false;
        std::size_t count = 0;
        int hits = 0;
        std::string message;      // the latest big message
        std::uint64_t serial = 0; // goes up with every new message
    };

    /// @brief Starts a search (GUI thread). The snapshot is taken on the next emulated frame.
    void Start();
    /// @brief Stops the search (GUI thread).
    void Stop();
    /// @brief The player was just hit (GUI thread).
    void Hit();
    [[nodiscard]] bool Searching() const {
        return m_searching.load() || m_startRequested.load();
    }

    /// @brief Runs one step (emulator thread, after every frame).
    void Step(ymir::sys::SH2Bus &bus);

    [[nodiscard]] Status GetStatus();
    /// @brief The code made when the search finished, once (GUI thread).
    [[nodiscard]] std::optional<std::string> TakeLockCodes();

private:
    void Snapshot(ymir::sys::SH2Bus &bus);
    void SetMessage(std::string message);

    // Lock only a handful, after several hits: locking a wrong value can break the game (a boss that never appears)
    static constexpr std::size_t kLockAt = 4; // lock when this many or fewer are left
    static constexpr int kMinHits = 3;        // ...and the player was hit at least this many times
    static constexpr int kCheckEveryFrames = 30;

    std::atomic_bool m_startRequested{false};
    std::atomic_bool m_stopRequested{false};
    std::atomic_int m_hitsRequested{0};
    std::atomic_bool m_searching{false};

    // Emulator thread only
    std::vector<uint32> m_addresses;
    std::vector<uint8> m_first; // value when the search started (full energy), one byte per address
    std::vector<uint8> m_ref;   // value at the last "I was hit" press
    std::vector<uint8> m_last;  // value at the last check
    std::vector<uint8> m_drops;  // times it went down since the last press
    int m_frames = 0;
    int m_hits = 0;

    std::mutex m_mutex; // guards the fields below
    Status m_status;
    std::optional<std::string> m_lockCodes;
};

/// @brief The player's cheat choices, saved in `cheats.txt` in the profile folder (on the external drive when there
/// is one).
class CheatChoices {
public:
    void Load(const std::filesystem::path &path);
    void Save() const;

    [[nodiscard]] bool IsOn(const std::string &game, const std::string &cheat) const;
    void Set(const std::string &game, const std::string &cheat, bool on);

    /// @brief The game picked by hand for a disc, by product number; empty when the automatic match is used.
    /// "-" means cheats are off for that disc.
    [[nodiscard]] std::string GetOverride(const std::string &productNumber) const;
    void SetOverride(const std::string &productNumber, const std::string &game);

    /// @brief A cheat made with the code finder.
    struct CustomCheat {
        std::string name;
        std::string codes;
    };
    [[nodiscard]] const std::vector<CustomCheat> &GetCustom(const std::string &game) const;
    void AddCustom(const std::string &game, CustomCheat cheat);
    void RemoveCustom(const std::string &game, const std::string &name);

private:
    std::filesystem::path m_path;
    std::map<std::string, std::set<std::string>> m_on;   // game -> cheats switched on
    std::map<std::string, std::string> m_overrides;      // product number -> game
    std::map<std::string, std::vector<CustomCheat>> m_custom; // game -> cheats made with the code finder
};

} // namespace platform::uwp::shell
