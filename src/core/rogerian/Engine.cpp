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
        rule.pattern = std::regex(item["pattern"].get<std::string>(), std::regex_constants::icase);
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

std::string Engine::respond(const std::string& userText) {
    if (!m_activePack) {
        return "I'm sorry, I don't have any rules loaded to respond.";
    }

    std::smatch match;
    for (const auto& rule : m_activePack->rules) {
        if (std::regex_search(userText, match, rule.pattern)) {
            std::string captured = match.size() > 1 ? match[1].str() : "";
            std::string reflected = reflect(captured);
            
            int choice = rand() % rule.outs.size();
            std::string responseTemplate = rule.outs[choice].text;

            return std::regex_replace(responseTemplate, std::regex("\\{1\\}"), reflected);
        }
    }

    return pickNeutralProbe();
}

std::string Engine::reflect(const std::string& text) {
    std::string reflected = text;
    for (const auto& pair : m_activePack->reflectPairs) {
        reflected = std::regex_replace(reflected, std::regex("\\b" + pair.first + "\\b", std::regex_constants::icase), pair.second);
    }
    return reflected;
}

std::string Engine::pickNeutralProbe() {
    if (!m_activePack) return "I'm not sure what to say.";

    for (const auto& rule : m_activePack->rules) {
        if (rule.category == "General") {
            int choice = rand() % rule.outs.size();
            return rule.outs[choice].text;
        }
    }
    return "Please, tell me more.";
}

}
