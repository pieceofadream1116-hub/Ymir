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
- `F` and `B`: master code lines, ignored

Only compiled when YMIR_PLATFORM_UWP is set.
*/

#include <ymir/core/types.hpp>
#include <ymir/sys/bus.hpp>

#include <filesystem>
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

/// @brief The cheats of one game.
struct CheatGame {
    const char *name;    ///< game name shown in the menu
    const char *version; ///< the disc the codes were made for, e.g. "USA"
    /// Words that must all appear in the disc's title (uppercase, letters and digits only) for an automatic match.
    std::vector<const char *> titleKeys;
    /// Words that must not appear in the disc's title (to tell apart sequels and spin-offs).
    std::vector<const char *> excludeKeys;
    std::vector<CheatCode> cheats;
};

/// @brief The built-in cheat list.
const std::vector<CheatGame> &CheatGames();

/// @brief Finds the game whose codes fit a disc, from the title in the disc header.
/// @param[in] headerTitle the game title from the disc header
/// @return the index into `CheatGames()`, or -1 when no game matches
int MatchCheatGame(std::string_view headerTitle);

/// @brief Applies the active codes to the Saturn's memory. Shared by the GUI thread (which sets the codes) and the
/// emulator thread (which applies them after every frame).
class CheatEngine {
public:
    /// @brief One parsed code line.
    struct Op {
        enum class Kind { Write8, Write16, IfEqual16 };
        Kind kind;
        uint32 address;
        uint16 value;
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

private:
    std::filesystem::path m_path;
    std::map<std::string, std::set<std::string>> m_on;   // game -> cheats switched on
    std::map<std::string, std::string> m_overrides;      // product number -> game
};

} // namespace platform::uwp::shell
