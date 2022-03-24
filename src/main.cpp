#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include "Api.h"

namespace pf = powsybl::powerfactory;

void printChildren(pf::Api& api, const api::v2::DataObject& object) {
    std::cout << api.makeValueUniquePtr(object.GetFullName())->GetString() << std::endl;
    auto children = api.getChildren(object);
    for (auto itC = children.begin(); itC != children.end(); ++itC) {
        auto& child = *itC;
//            std::cout << api.makeValueUniquePtr(child->GetClassNameA())->GetString() << std::endl;
//        auto attributeNames = api.getAttributeNames(*child);
//        for (auto itN = attributeNames.begin(); itN != attributeNames.end(); ++itN) {
//            auto& name = *itN;
//            int type = child->GetAttributeType(name.c_str());
//            std::cout << "    " << name << " " << type << std::endl;
//        }
        printChildren(api, *child);
    }
}

int main() {
    try {
        pf::Api api(R"(C:\Program Files\DIgSILENT\PowerFactory 2022 SP1)");
        api.activateProject("Transmission System");
//        auto studyCase = api.makeObjectUniquePtr(api.getApplication()->GetActiveStudyCase());
//        printChildren(api, *studyCase);
        auto project = api.makeObjectUniquePtr(api.getApplication()->GetActiveProject());
        printChildren(api, *project);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
