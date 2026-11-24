#include "cheats.hpp"

#include <algorithm>
#include <array>
#include <cstdio>
#include <cctype>
#include <fstream>

namespace platform::uwp::shell {

// The codes come from the Action Replay lists of Almar's Guides (almarsguides.com) and ConsoleDatabase, for the USA
// discs. Master codes are left out on purpose; see cheats.hpp.
const std::vector<CheatGame> &CheatGames() {
    static const std::vector<CheatGame> games{
        {
            .name = "Bug Too!",
            .version = "USA",
            .titleKeys = {"BUG", "TOO"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited energy", "16055366 0006"},
                    {"Unlimited lives", "1605C5D2 0003"},
                    {"Unlimited continues", "1605C5D6 0003"},
                    {"Max items", "1605C5E2 0063"},
                    {"Invincible", "160553F6 0041 160553FC 004B"},
                },
        },
        {
            .name = "Bug!",
            .version = "USA",
            .titleKeys = {"BUG"},
            .excludeKeys = {"TOO", "DEBUG"},
            .cheats =
                {
                    {"Unlimited lives", "0602556E 0009"},
                    {"Unlimited energy", "0602552E 0009"},
                    {"Unlimited zap", "060272E0 0009 16053F30 00FF"},
                    {"256 gems", "16053F14 0100"},
                    {"Always get special stages", "16053F3C 0001"},
                    {"Level skip", "16053F4A 0001"},
                },
        },
        {
            .name = "Clockwork Knight 2",
            .version = "USA",
            .titleKeys = {"CLOCKWORKKNIGHT2"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited lives", "160413D8 0002"},
                    {"Unlimited energy", "160414D0 0303"},
                    {"Max gears", "16036AA8 0505"},
                    {"Timer stays at zero", "160590E0 0000"},
                },
        },
        {
            .name = "Clockwork Knight",
            .version = "USA",
            .titleKeys = {"CLOCKWORKKNIGHT"},
            .excludeKeys = {"KNIGHT2"},
            .cheats =
                {
                    {"Unlimited lives", "16043CBC 0004"},
                    {"Unlimited health", "36043CB4 0005"},
                    {"Unlimited time", "360587A2 00FE"},
                },
        },
        {
            .name = "Daytona USA Championship Circuit Edition",
            .version = "USA",
            .titleKeys = {"DAYTONA", "CIRCUIT"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited time", "16050F6C 0046"},
                    {"Always in 1st place", "160511F8 0100"},
                    {"Lap timer stays at zero", "16050E6E 0000"},
                },
        },
        {
            .name = "Daytona USA",
            .version = "USA",
            .titleKeys = {"DAYTONA"},
            .excludeKeys = {"CIRCUIT"},
            .cheats =
                {
                    {"Unlimited time", "1607EAAE 0318 1607EBCE 06F0"},
                    {"Always in 1st place", "16048072 0001 16048074 0001 16048076 0001"},
                    {"Race backwards", "1600B056 0000"},
                },
        },
        {
            .name = "Mr. Bones",
            .version = "USA",
            .titleKeys = {"BONES"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited bones meter", "160252FE 000E"},
                },
        },
        {
            .name = "Myst",
            .version = "USA",
            .titleKeys = {"MYST"},
            .excludeKeys = {"MYSTARIA"},
            .cheats = {}, // no Action Replay codes exist; listed for its built-in cover
        },
        // Games without codes, listed for their built-in covers
        {.name = "Road Rash", .version = "USA", .titleKeys = {"ROADRASH"}, .excludeKeys = {}, .cheats = {}},
        {.name = "Sega Rally Championship",
         .version = "USA",
         .titleKeys = {"SEGARALLY"},
         .excludeKeys = {},
         .cheats = {}},
        {.name = "Spot Goes to Hollywood",
         .version = "USA",
         .titleKeys = {"SPOT", "HOLLYWOOD"},
         .excludeKeys = {},
         .cheats = {}},
        {.name = "Street Fighter Collection",
         .version = "USA",
         .titleKeys = {"STREETFIGHTER", "COLLECTION"},
         .excludeKeys = {},
         .cheats = {}},
        {.name = "WWF In Your House", .version = "USA", .titleKeys = {"WWF"}, .excludeKeys = {}, .cheats = {}},
        {
            .name = "Panzer Dragoon II Zwei",
            .version = "USA",
            .titleKeys = {"PANZERDRAGOON"},
            .excludeKeys = {"SAGA"},
            .cheats =
                {
                    {"Unlimited energy", "160730A8 0080 160730A4 0080"},
                    {"Unlimited berserk meter", "160730AC 00B4"},
                    {"100% kills", "160730EC 0064 160730EE 0064"},
                    {"Always 3-way shot", "1607335C 0100", 1},
                    {"Always 5-way shot", "1607335C 0200", 1},
                    {"Always homing shot", "1607335C 0500", 1},
                    {"Always graviton shot", "1607335C 0600", 1},
                },
        },
        {
            .name = "Panzer Dragoon",
            .version = "USA",
            .titleKeys = {"PANZERDRAGOON"},
            .excludeKeys = {"SAGA", "ZWEI", "DRAGOONII", "DRAGOON2"},
            .cheats =
                {
                    {"Unlimited energy", "1607FB08 0100"},
                    {"Unlimited credits", "16084BAA 0005"},
                },
        },
        {
            .name = "Solar Eclipse",
            .version = "USA",
            .titleKeys = {"SOLARECLIPSE"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited lives and missiles", "10265374 0909"},
                    {"Unlimited energy", "102652B8 0069 10265278 0069"},
                },
        },
        {
            .name = "Virtua Cop 2",
            .version = "USA",
            .titleKeys = {"VIRTUACOP2"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Player 1 unlimited energy", "1604DDA2 0909"},
                    {"Player 1 unlimited ammo", "16052C38 0600"},
                    {"Player 2 unlimited energy", "1604DDE6 0909"},
                    {"Player 2 unlimited ammo", "16052C88 0600"},
                    {"Unlimited credits", "1604DD1C 0109"},
                    {"Player 1 always automatic gun", "16052C4A 0100 16052C4E AC20", 1},
                    {"Player 1 always rifle", "16052C4A 0200 16052C4E AEA8", 1},
                    {"Player 1 always machine gun", "16052C4A 0300 16052C4E ACF8", 1},
                    {"Player 1 always shotgun", "16052C4A 0400 16052C4E B0FC", 1},
                    // The guide prints the first line as 16052C4E, a typo: every other gun uses 16052C4A
                    {"Player 1 always magnum", "16052C4A 0500 16052C4E B094", 1},
                    {"Player 2 always automatic gun", "16052C9A 0100 16052C9E AC20", 2},
                    {"Player 2 always rifle", "16052C9A 0200 16052C9E AEA8", 2},
                    {"Player 2 always machine gun", "16052C9A 0300 16052C9E ACF8", 2},
                    {"Player 2 always shotgun", "16052C9A 0400 16052C9E B0FC", 2},
                    {"Player 2 always magnum", "16052C9A 0500 16052C9E B094", 2},
                },
        },
        {
            .name = "Virtua Cop",
            .version = "USA",
            .titleKeys = {"VIRTUACOP"},
            .excludeKeys = {"VIRTUACOP2"},
            .cheats =
                {
                    {"Player 1 unlimited energy", "16057912 0505"},
                    {"Player 1 unlimited bullets", "1606A398 0006"},
                    {"Player 2 unlimited energy", "16057952 0505"},
                    {"Player 2 unlimited bullets", "1606A3E8 0006"},
                },
        },
        {
            .name = "Virtua Fighter 2",
            .version = "USA",
            .titleKeys = {"VIRTUAFIGHTER2"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Unlimited time", "160E0032 0782"},
                    {"Player 1 unlimited energy", "160621B8 00A0"},
                    {"Player 1 dies in one hit", "D60621B8 00A0 160621B8 0001"},
                    {"Fight outside the ring", "160E0068 004F"},
                    {"Mega kick", "160E007C 0000"},
                    {"Fight under water", "160E0038 0010 160E001A 0002"},
                    {"Low gravity", "160E007A 0024", 1},
                    {"No gravity", "160E007A 0000", 1},
                    {"Ultra gravity", "160E007A FFFF", 1},
                },
        },
        {
            .name = "Virtua Fighter Remix",
            .version = "USA",
            .titleKeys = {"VIRTUAFIGHTER", "REMIX"},
            .excludeKeys = {},
            .cheats =
                {
                    {"Player 1 unlimited energy", "16092072 00A0"},
                    {"Play the bonus level", "1609EA90 0909"},
                },
        },
        {
            .name = "Virtua Fighter",
            .version = "USA",
            .titleKeys = {"VIRTUAFIGHTER"},
            .excludeKeys = {"VIRTUAFIGHTER2", "REMIX", "KIDS"},
            // Each code carries the game's own "enable code" (0601EDCE 0009), which the guide says must be on
            .cheats =
                {
                    {"Player 1 unlimited energy", "0601EDCE 0009 16092072 FFFF"},
                    {"Player 2 unlimited energy", "0601EDCE 0009 160940CE FFFF"},
                    {"Ranking mode", "0601EDCE 0009 302FF229 0001"},
                    {"Extra options (below Exit)", "0601EDCE 0009 302FF22A 0001"},
                },
        },
    };
    return games;
}

namespace {

    /// @brief Uppercase letters and digits only, so "Virtua Cop 2" and "VIRTUA COP2" compare the same.
    std::string Normalize(std::string_view text) {
        std::string out;
        out.reserve(text.size());
        for (const char c : text) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                out.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
            }
        }
        return out;
    }

    bool ParseHex(std::string_view word, uint32 &out) {
        if (word.empty() || word.size() > 8) {
            return false;
        }
        uint32 value = 0;
        for (const char c : word) {
            const int digit = std::isdigit(static_cast<unsigned char>(c)) ? c - '0'
                              : (c >= 'A' && c <= 'F')                    ? c - 'A' + 10
                              : (c >= 'a' && c <= 'f')                    ? c - 'a' + 10
                                                                          : -1;
            if (digit < 0) {
                return false;
            }
            value = (value << 4) | static_cast<uint32>(digit);
        }
        out = value;
        return true;
    }

} // namespace

std::string BundledCoverName(std::string_view headerTitle) {
    const int index = MatchCheatGame(headerTitle);
    if (index < 0) {
        return {};
    }
    // "Bug Too!" -> "bug-too": the file names under uwp/shell/assets/covers
    std::string slug;
    for (const char c : std::string_view{CheatGames()[index].name}) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            slug.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        } else if (!slug.empty() && slug.back() != '-') {
            slug.push_back('-');
        }
    }
    while (!slug.empty() && slug.back() == '-') {
        slug.pop_back();
    }
    return slug;
}

int MatchCheatGame(std::string_view headerTitle) {
    const std::string title = Normalize(headerTitle);
    if (title.empty()) {
        return -1;
    }
    const auto &games = CheatGames();
    // The list is ordered with sequels before the games their names contain, so the first match wins
    for (std::size_t i = 0; i < games.size(); ++i) {
        const CheatGame &game = games[i];
        const bool all = std::all_of(game.titleKeys.begin(), game.titleKeys.end(),
                                     [&](const char *key) { return title.find(key) != std::string::npos; });
        const bool none = std::none_of(game.excludeKeys.begin(), game.excludeKeys.end(),
                                       [&](const char *key) { return title.find(key) != std::string::npos; });
        if (all && none) {
            // Panzer Dragoon II has no "II" in some headers; it is only chosen when the title says so
            if (std::string_view{game.name} == "Panzer Dragoon II Zwei" &&
                title.find("ZWEI") == std::string::npos && title.find("DRAGOONII") == std::string::npos &&
                title.find("DRAGOON2") == std::string::npos) {
                continue;
            }
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool CheatEngine::Parse(std::string_view codes, std::vector<Op> &out) {
    std::vector<std::string_view> words;
    std::size_t pos = 0;
    while (pos < codes.size()) {
        while (pos < codes.size() && std::isspace(static_cast<unsigned char>(codes[pos]))) {
            ++pos;
        }
        const std::size_t start = pos;
        while (pos < codes.size() && !std::isspace(static_cast<unsigned char>(codes[pos]))) {
            ++pos;
        }
        if (pos > start) {
            words.push_back(codes.substr(start, pos - start));
        }
    }
    if (words.size() % 2 != 0) {
        return false;
    }

    for (std::size_t i = 0; i < words.size(); i += 2) {
        uint32 addressWord = 0;
        uint32 valueWord = 0;
        if (words[i].size() != 8 || words[i + 1].size() != 4 || !ParseHex(words[i], addressWord) ||
            !ParseHex(words[i + 1], valueWord)) {
            return false;
        }
        const uint32 type = addressWord >> 28;
        const uint32 address = addressWord & 0x0FFF'FFFF;
        const auto value = static_cast<uint16>(valueWord);
        switch (type) {
        case 0x0:
        case 0x1: out.push_back({Op::Kind::Write16, address, value}); break;
        case 0x3: out.push_back({Op::Kind::Write8, address, value}); break;
        case 0xD: out.push_back({Op::Kind::IfEqual16, address, value}); break;
        case 0xB:
        case 0xF: break; // master code: not needed, see cheats.hpp
        default: return false;
        }
    }
    return true;
}

void CheatEngine::SetActive(std::vector<Op> ops) {
    std::lock_guard lock{m_mutex};
    m_ops = std::move(ops);
}

void CheatEngine::Apply(ymir::sys::SH2Bus &bus) {
    std::lock_guard lock{m_mutex};
    bool skipNext = false;
    for (const Op &op : m_ops) {
        if (skipNext) {
            skipNext = false;
            continue;
        }
        switch (op.kind) {
        case Op::Kind::Write16: bus.Poke<uint16>(op.address, op.value); break;
        case Op::Kind::Write8: bus.Poke<uint8>(op.address, static_cast<uint8>(op.value)); break;
        case Op::Kind::IfEqual16: skipNext = bus.Peek<uint16>(op.address) != op.value; break;
        }
    }
}

// -----------------------------------------------------------------------------
// Saved choices
//
// cheats.txt holds one line per choice:
//   on<TAB><game><TAB><cheat>     a cheat that is switched on
//   game<TAB><product><TAB><game> the game picked by hand for a disc ("-" = cheats off)

void CheatChoices::Load(const std::filesystem::path &path) {
    m_path = path;
    m_on.clear();
    m_overrides.clear();
    std::ifstream in{path};
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        const std::size_t a = line.find('\t');
        const std::size_t b = a == std::string::npos ? std::string::npos : line.find('\t', a + 1);
        if (b == std::string::npos) {
            continue;
        }
        const std::string kind = line.substr(0, a);
        const std::string first = line.substr(a + 1, b - a - 1);
        const std::string second = line.substr(b + 1);
        if (kind == "on") {
            m_on[first].insert(second);
        } else if (kind == "game") {
            m_overrides[first] = second;
        } else if (kind == "custom") {
            // custom<TAB>game<TAB>name<TAB>codes
            const std::size_t c = second.find('\t');
            if (c != std::string::npos) {
                m_custom[first].push_back({second.substr(0, c), second.substr(c + 1)});
            }
        }
    }
}

void CheatChoices::Save() const {
    if (m_path.empty()) {
        return;
    }
    std::ofstream out{m_path, std::ios::trunc};
    for (const auto &[game, cheats] : m_on) {
        for (const std::string &cheat : cheats) {
            out << "on\t" << game << '\t' << cheat << '\n';
        }
    }
    for (const auto &[product, game] : m_overrides) {
        out << "game\t" << product << '\t' << game << '\n';
    }
    for (const auto &[game, cheats] : m_custom) {
        for (const CustomCheat &cheat : cheats) {
            out << "custom\t" << game << '\t' << cheat.name << '\t' << cheat.codes << '\n';
        }
    }
}

const std::vector<CheatChoices::CustomCheat> &CheatChoices::GetCustom(const std::string &game) const {
    static const std::vector<CustomCheat> none;
    const auto it = m_custom.find(game);
    return it != m_custom.end() ? it->second : none;
}

void CheatChoices::AddCustom(const std::string &game, CustomCheat cheat) {
    RemoveCustom(game, cheat.name);
    m_custom[game].push_back(std::move(cheat));
}

void CheatChoices::RemoveCustom(const std::string &game, const std::string &name) {
    if (auto it = m_custom.find(game); it != m_custom.end()) {
        std::erase_if(it->second, [&](const CustomCheat &c) { return c.name == name; });
        if (it->second.empty()) {
            m_custom.erase(it);
        }
    }
    Set(game, name, false);
}

// -----------------------------------------------------------------------------
// Code finder

namespace {
    // The two work RAM areas, where games keep their variables
    constexpr std::array<std::pair<uint32, uint32>, 2> kWorkRam{{
        {0x0020'0000, 0x0010'0000}, // low work RAM, 1 MiB
        {0x0600'0000, 0x0010'0000}, // high work RAM, 1 MiB
    }};
} // namespace

void CodeFinder::Start(ymir::sys::SH2Bus &bus) {
    m_addresses.clear();
    m_first.clear();
    m_last.clear();
    for (const auto &[base, size] : kWorkRam) {
        for (uint32 offset = 0; offset < size; offset += 2) {
            const uint32 address = base + offset;
            const uint16 value = bus.Peek<uint16>(address);
            m_addresses.push_back(address);
            m_first.push_back(value);
            m_last.push_back(value);
        }
    }
    m_active = true;
}

void CodeFinder::Keep(ymir::sys::SH2Bus &bus, Filter filter) {
    if (!m_active) {
        return;
    }
    std::size_t kept = 0;
    for (std::size_t i = 0; i < m_addresses.size(); ++i) {
        const uint16 now = bus.Peek<uint16>(m_addresses[i]);
        const uint16 before = m_last[i];
        bool keep = false;
        switch (filter) {
        case Filter::Decreased: keep = now < before; break;
        case Filter::Increased: keep = now > before; break;
        case Filter::Unchanged: keep = now == before; break;
        case Filter::Changed: keep = now != before; break;
        }
        if (keep) {
            m_addresses[kept] = m_addresses[i];
            m_first[kept] = m_first[i];
            m_last[kept] = now;
            ++kept;
        }
    }
    m_addresses.resize(kept);
    m_first.resize(kept);
    m_last.resize(kept);
}

void CodeFinder::Clear() {
    m_active = false;
    m_addresses = {};
    m_first = {};
    m_last = {};
}

std::string CodeFinder::LockCodes() const {
    std::string codes;
    char line[24];
    for (std::size_t i = 0; i < m_addresses.size(); ++i) {
        std::snprintf(line, sizeof(line), "%s1%07X %04X", codes.empty() ? "" : " ", m_addresses[i] & 0x0FFF'FFFF,
                      m_first[i]);
        codes += line;
    }
    return codes;
}

bool CheatChoices::IsOn(const std::string &game, const std::string &cheat) const {
    const auto it = m_on.find(game);
    return it != m_on.end() && it->second.contains(cheat);
}

void CheatChoices::Set(const std::string &game, const std::string &cheat, bool on) {
    if (on) {
        m_on[game].insert(cheat);
    } else if (auto it = m_on.find(game); it != m_on.end()) {
        it->second.erase(cheat);
        if (it->second.empty()) {
            m_on.erase(it);
        }
    }
}

std::string CheatChoices::GetOverride(const std::string &productNumber) const {
    const auto it = m_overrides.find(productNumber);
    return it != m_overrides.end() ? it->second : std::string{};
}

void CheatChoices::SetOverride(const std::string &productNumber, const std::string &game) {
    if (game.empty()) {
        m_overrides.erase(productNumber);
    } else {
        m_overrides[productNumber] = game;
    }
}

} // namespace platform::uwp::shell
