#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "Api.h"

namespace pf = powsybl::powerfactory;

int main() {
    try {
        pf::Api api(R"(C:\Program Files\DIgSILENT\PowerFactory 2022 SP1)");
        api.activateProject("Transmission System");
        auto studyCase = api.makeObjectUniquePtr(api.getApplication()->GetActiveStudyCase());
        std::cout << api.makeValueUniquePtr(studyCase->GetFullName())->GetString() << std::endl;
        auto children = api.getChildren(studyCase);
        for (auto itC = children.begin(); itC != children.end(); ++itC) {
            auto& child = *itC;
            std::cout << api.makeValueUniquePtr(child->GetFullName())->GetString() << std::endl;
            std::cout << api.makeValueUniquePtr(child->GetClassNameA())->GetString() << std::endl;
            auto attributeNames = api.getAttributeNames(child);
            for (auto itN = attributeNames.begin(); itN != attributeNames.end(); ++itN) {
                auto& name = *itN;
                int type = child->GetAttributeType(name.c_str());
                std::cout << "    " << name << " " << type << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
