#ifndef DEEPTHONK3D_RULES_H
#define DEEPTHONK3D_RULES_H

#include <string>
#include <vector>
#include <regex>
#include <cstdint>

namespace deep_thonk {

    enum class Locale {
        PT_BR,
        EN_US
    };

    struct RuleTemplate {
        std::string text;
    };

    struct Rule {
        std::string id;
        std::string category;
        std::string patternString;
        std::regex pattern;
        std::vector<RuleTemplate> outs;
        uint64_t hits = 0;
    };

    struct RulePack {
        Locale locale;
        std::vector<Rule> rules;
        std::vector<std::pair<std::string, std::string>> reflectPairs;
    };

}

#endif //DEEPTHONK3D_RULES_H
