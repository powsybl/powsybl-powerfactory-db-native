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
#include <map>
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

typedef std::unique_ptr<const api::Value, ValueDeleter> ValueUniquePtr;

class Api {
public:
    explicit Api(const std::string& powerFactoryHome);

    ~Api();

    Api(const Api&) = delete;
    Api& operator=(const Api&) = delete;

    ValueUniquePtr makeValueUniquePtr(const api::Value* value) const {
        return {value, ValueDeleter(_api)};
    }

    long addObject(api::v2::DataObject* object);
    long getObjectId(api::v2::DataObject* object) const;

    std::vector<api::v2::DataObject*> getChildren(const api::v2::DataObject& parent);
    std::vector<std::string> getAttributeNames(const api::v2::DataObject& object) const;

    api::v2::DataObject* activateProject(const std::string& projectName);

public:
    HINSTANCE _dllHandle;
    api::v2::Api* _api;

    // data object pointer to long id only works because we are in default SetObjectReusingEnabled to true mode
    // so data object are only proxy on real object that are reused when asking for same object
    std::map<api::v2::DataObject*, long> _objectToId;
};

}

}

#endif //POWSYBL_POWERFACTORY_DB_NATIVE_API_H
