#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <windows.h>
#include "v2/Api.hpp"

extern "C" {

typedef api::v2::Api* (__cdecl *CREATEAPI)(const char* username, const char* password, const char* commandLineArguments);
typedef void (__cdecl *DESTROYAPI)(api::v2::Api*&);

}

namespace powsybl {

namespace powerfactory {

struct ValueDeleter {
    explicit ValueDeleter(api::v2::Api* api)
        : _api(api) {
    }

    void operator()(const api::Value* value) const {
        _api->ReleaseValue(value);
    }

    api::v2::Api* _api;
};

struct ObjectDeleter {
    explicit ObjectDeleter(api::v2::Api* api)
        : _api(api) {
    }

    void operator()(api::v2::DataObject* object) const {
        _api->ReleaseObject(object);
    }

    api::v2::Api* _api;
};

typedef std::unique_ptr<const api::Value, ValueDeleter> ValueUniquePtr;
typedef std::unique_ptr<api::v2::DataObject, ObjectDeleter> ObjectUniquePtr;

class Api {
public:
    explicit Api(const std::string& powerFactoryHome);

    ~Api();

    Api(const Api&) = delete;
    Api& operator=(const Api&) = delete;

    ValueUniquePtr makeValueUniquePtr(const api::Value* value) const {
        return {value, ValueDeleter(_api)};
    }

    ObjectUniquePtr makeObjectUniquePtr(api::v2::DataObject* object) const {
        return {object, ObjectDeleter(_api)};
    }

    api::v2::Application* getApplication() const {
        return _api->GetApplication();
    }

    std::vector<ObjectUniquePtr> getChildren(const ObjectUniquePtr& parent) const;
    std::vector<std::string> getAttributeNames(const ObjectUniquePtr& object) const;

    void activateProject(const std::string& projectName);

private:
    HINSTANCE _dllHandle;
    api::v2::Api* _api;
};

Api::Api(const std::string& powerFactoryHome) {
    _dllHandle = LoadLibraryEx(TEXT((powerFactoryHome + R"(\digapi.dll)").c_str()),
                               nullptr,
                               LOAD_WITH_ALTERED_SEARCH_PATH);
    if (!_dllHandle) {
        throw std::runtime_error("digapi.dll could not be loaded");
    }

    CREATEAPI createApi = (CREATEAPI) GetProcAddress(_dllHandle,
                                                    "CreateApiInstanceV2");
    try {
        _api = createApi(nullptr, nullptr, nullptr);
    } catch(const api::v2::ExitError& e) {
        throw std::runtime_error("create get failed with error code " + std::to_string(e.GetCode()));
    }
}

Api::~Api() {
    if (_dllHandle) {
        DESTROYAPI destroyApi = (DESTROYAPI) GetProcAddress ((struct HINSTANCE__*) _dllHandle,
                                                             "DestroyApiInstanceV2");
        if (_api) {
            destroyApi(_api);
        }
        FreeLibrary(_dllHandle);
    }
}

void Api::activateProject(const std::string& projectName) {
    auto app = _api->GetApplication();
    api::Value nameVal(projectName.c_str());
    auto errorCode = makeValueUniquePtr(app->Execute("ActivateProject", &nameVal));
    if (errorCode->GetInteger() != 0) {
        throw std::runtime_error("Project '" + projectName + "' activation failed");
    }
}

std::vector<ObjectUniquePtr> Api::getChildren(const ObjectUniquePtr& parent) const {
    std::vector<ObjectUniquePtr> children;
    auto childrenVal= makeValueUniquePtr(parent->GetChildren(false));
    children.reserve(childrenVal->VecGetSize());
    for (size_t i = 0; i < childrenVal->VecGetSize(); ++i) {
        auto child = makeObjectUniquePtr(static_cast<api::v2::DataObject*>(childrenVal->VecGetDataObject(i)));
        children.push_back(std::move(child));
    }
    return children;
}

std::vector<std::string> Api::getAttributeNames(const ObjectUniquePtr& object) const {
    std::vector<std::string> names;
    auto namesVal = makeValueUniquePtr(object->GetAttributeNames());
    names.reserve(namesVal->VecGetSize());
    for (size_t i = 0; i < namesVal->VecGetSize(); ++i) {
        names.emplace_back(namesVal->VecGetString(i));
    }
    return names;
}

}

}

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
                std::cout << "    " << name << std::endl;
                std::cout << "    " << child->GetAttributeType(name.c_str()) << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
