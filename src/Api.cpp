/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file Api.cpp
 * @author Geoffroy Jamgotchian <geoffroy.jamgotchian at rte-france.com>
 */
#include <stdexcept>
#include "api.h"

extern "C" {

typedef api::v2::Api* (__cdecl *CREATEAPI)(const char* username, const char* password, const char* commandLineArguments);
typedef void (__cdecl *DESTROYAPI)(api::v2::Api*&);

}

namespace powsybl {

namespace powerfactory {

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
        // release objects
        for (auto it = _objectToId.begin(); it != _objectToId.end(); it++) {
            auto object = it->first;
            _api->ReleaseObject(object);
        }

        DESTROYAPI destroyApi = (DESTROYAPI) GetProcAddress ((struct HINSTANCE__*) _dllHandle,
                                                             "DestroyApiInstanceV2");
        if (_api) {
            destroyApi(_api);
        }
        FreeLibrary(_dllHandle);
    }
}

long Api::addObject(api::v2::DataObject* object) {
    if (object) {
        auto it = _objectToId.find(object);
        if (it == _objectToId.end()) {
            long id = _objectToId.size();
            _objectToId.insert({object, id});
            return id;
        }
        return it->second;
    }
    return -1;
}

long Api::getObjectId(api::v2::DataObject* object) const {
    if (object) {
        auto it = _objectToId.find(object);
        if (it == _objectToId.end()) {
            throw std::runtime_error("Object not found");
        }
        return it->second;
    }
    return -1;
}

api::v2::DataObject* Api::activateProject(const std::string& projectName) {
    auto app = _api->GetApplication();
    api::Value nameVal(projectName.c_str());
    auto errorCode = makeValueUniquePtr(app->Execute("ActivateProject", &nameVal));
    if (errorCode->GetInteger() != 0) {
        throw std::runtime_error("Project '" + projectName + "' activation failed");
    }
    auto project = _api->GetApplication()->GetActiveProject();
    addObject(project);
    return project;
}

std::vector<api::v2::DataObject*> Api::getChildren(const api::v2::DataObject& parent) {
    std::vector<api::v2::DataObject*> children;
    auto childrenVal= makeValueUniquePtr(parent.GetChildren(false));
    children.reserve(childrenVal->VecGetSize());
    for (size_t i = 0; i < childrenVal->VecGetSize(); ++i) {
        auto child = static_cast<api::v2::DataObject*>(childrenVal->VecGetDataObject(i));
        addObject(child);
        children.push_back(child);
    }
    return children;
}

std::vector<std::string> Api::getAttributeNames(const api::v2::DataObject& object) const {
    std::vector<std::string> names;
    auto namesVal = makeValueUniquePtr(object.GetAttributeNames());
    names.reserve(namesVal->VecGetSize());
    for (size_t i = 0; i < namesVal->VecGetSize(); ++i) {
        names.emplace_back(namesVal->VecGetString(i));
    }
    return names;
}

}

}
