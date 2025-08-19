#include "Engine.h"
#include "../../third_party/nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <random>

using json = nlohmann::json;

namespace deep_thonk {

Engine::Engine() {
    // Seed for random number generation
    srand(time(0));
}

void Engine::loadRulesFromString(const std::string& jsonContent) {
    json data = json::parse(jsonContent);
    
    std::string localeStr = data["locale"];
    RulePack pack;
    if (localeStr == "en-US") {
        pack.locale = Locale::EN_US;
    } else if (localeStr == "pt-BR") {
        pack.locale = Locale::PT_BR;
    }

    for (const auto& item : data["reflect"]) {
        pack.reflectPairs.push_back({item[0], item[1]});
    }

    for (const auto& item : data["rules"]) {
        Rule rule;
        rule.id = item["id"];
        rule.category = item["category"];
        rule.patternString = item["pattern"].get<std::string>();
        rule.pattern = std::regex(rule.patternString, std::regex_constants::icase);
        for (const auto& out : item["outs"]) {
            rule.outs.push_back({out.get<std::string>()});
        }
        pack.rules.push_back(rule);
    }

    m_rulePacks[localeStr] = pack;
    std::cout << "Loaded rules for locale: " << localeStr << std::endl;
}

void Engine::setLocale(const std::string& locale) {
    if (m_rulePacks.count(locale)) {
        m_activePack = &m_rulePacks[locale];
        std::cout << "Active locale set to: " << locale << std::endl;
    } else {
        std::cerr << "Locale not found: " << locale << std::endl;
        m_activePack = nullptr;
    }
}

deep_thonk::Response Engine::respond(const std::string& userText) {
    if (!m_activePack) {
        return {"I'm sorry, I don't have any rules loaded to respond.", ""};
    }

    Rule* bestRule = nullptr;
    std::smatch bestMatch;

    for (auto& rule : m_activePack->rules) {
        std::smatch currentMatch;
        if (std::regex_search(userText, currentMatch, rule.pattern)) {
            if (!bestRule || rule.patternString.length() > bestRule->patternString.length()) {
                bestRule = &rule;
                bestMatch = currentMatch;
            }
        }
    }

    if (bestRule) {
        bestRule->hits++;
        std::string captured = bestMatch.size() > 1 ? bestMatch[1].str() : "";
        int choice = rand() % bestRule->outs.size();
        std::string responseTemplate = bestRule->outs[choice].text;

        if (responseTemplate.find("{1}") != std::string::npos) {
            std::string reflected = reflect(captured);
            return {std::regex_replace(responseTemplate, std::regex("\\{1\\}"), reflected), bestRule->id};
        }
        
        return {responseTemplate, bestRule->id};
    }

    return pickNeutralProbe();
}

std::string Engine::reflect(const std::string& text) {
    std::string result;
    std::regex tokenizer("([a-zA-Z']+|[^a-zA-Z']+)");
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), tokenizer);
    auto words_end = std::sregex_iterator();

    std::map<std::string, std::string> reflectionMap;
    for(const auto& pair : m_activePack->reflectPairs) {
        std::string key = pair.first;
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        reflectionMap[key] = pair.second;
    }

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string token = i->str();
        std::string lower_token = token;
        std::transform(lower_token.begin(), lower_token.end(), lower_token.begin(), ::tolower);

        auto it = reflectionMap.find(lower_token);
        if (it != reflectionMap.end()) {
            result += it->second;
        } else {
            result += token;
        }
    }

    return result;
}

deep_thonk::Response Engine::pickNeutralProbe() {
    if (!m_activePack) return {"I'm not sure what to say.", ""};

    for (const auto& rule : m_activePack->rules) {
        if (rule.category == "General") {
            int choice = rand() % rule.outs.size();
            return {rule.outs[choice].text, rule.id};
        }
    }
    return {"Please, tell me more.", ""};
}

const std::map<std::string, RulePack> &Engine::getRulePacks() const {
    return m_rulePacks;
}

}
