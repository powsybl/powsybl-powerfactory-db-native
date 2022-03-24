/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file Api.h
 * @author Geoffroy Jamgotchian <geoffroy.jamgotchian at rte-france.com>
 */
#ifndef POWSYBL_POWERFACTORY_DB_NATIVE_API_H
#define POWSYBL_POWERFACTORY_DB_NATIVE_API_H

#include <memory>
#include <string>
#include <vector>
#include <Windows.h>
#include "v2/Api.hpp"

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

    std::vector<ObjectUniquePtr> getChildren(const api::v2::DataObject& parent) const;
    std::vector<std::string> getAttributeNames(const api::v2::DataObject& object) const;

    void activateProject(const std::string& projectName);

private:
    HINSTANCE _dllHandle;
    api::v2::Api* _api;
};

}

}

#endif //POWSYBL_POWERFACTORY_DB_NATIVE_API_H
