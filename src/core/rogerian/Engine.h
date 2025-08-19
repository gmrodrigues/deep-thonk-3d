#ifndef ENGINE_H
#define ENGINE_H

#include "Rules.h"
#include <string>
#include <vector>
#include <map>

namespace deep_thonk {

class Engine {
public:
    Engine();

    void loadRulesFromString(const std::string& jsonContent);
    void setLocale(const std::string& locale);
    std::string respond(const std::string& userText);
    const std::map<std::string, RulePack>& getRulePacks() const;

private:
    std::string reflect(const std::string& text);
    std::string pickNeutralProbe();

    std::map<std::string, RulePack> m_rulePacks;
    RulePack* m_activePack = nullptr;
};

}

#endif // ENGINE_H
