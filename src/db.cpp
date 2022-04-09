/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file db.cpp
 * @author Geoffroy Jamgotchian <geoffroy.jamgotchian at rte-france.com>
 */
#include <jni.h>
#include <map>
#include "jniwrapper.hpp"
#include "api.h"

namespace pf = powsybl::powerfactory;
namespace jni = powsybl::jni;

namespace powsybl {

namespace powerfactory {

void traverse(Api &api, const jni::ComPowsyblPowerFactoryDbDataObjectBuilder &objectBuilder,
              const ObjectUniquePtr& object, long& idCount, long parentId, std::map<long, long>& idToParentId,
              bool fillDescription) {
    // create class if not already exist
    std::string className = api.makeValueUniquePtr(object->GetClassNameA())->GetString();
    objectBuilder.createClass(className);

    // create object
    long id = idCount++;
    idToParentId.insert({id, parentId});
    objectBuilder.createObject(id, className);

    auto attributeNames = api.getAttributeNames(*object);
    for (auto itN = attributeNames.begin(); itN != attributeNames.end(); ++itN) {
        auto& attributeName = *itN;
        int type = object->GetAttributeType(attributeName.c_str());
        if (type != api::v2::DataObject::AttributeType::TYPE_INVALID) { // what does it mean?
            // create attribute if not already exist
            std::string description;
            if (fillDescription) {
                auto descriptionValue = object->GetAttributeDescription(attributeName.c_str());
                if (descriptionValue) {
                    description = api.makeValueUniquePtr(descriptionValue)->GetString();
                    objectBuilder.createAttribute(className, attributeName, type, description);
                }
            }

            // set attribute value to object
            switch (type) {
                case api::v2::DataObject::AttributeType::TYPE_STRING: {
                    std::string value = api.makeValueUniquePtr(object->GetAttributeString(attributeName.c_str()))->GetString();
                    objectBuilder.setStringAttributeValue(id, attributeName, value);
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_INTEGER: {
                    int value = object->GetAttributeInt(attributeName.c_str());
                    objectBuilder.setIntAttributeValue(id, attributeName, value);
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_INTEGER64: {
                    long value = object->GetAttributeInt64(attributeName.c_str());
                    objectBuilder.setLongAttributeValue(id, attributeName, value);
                    break;
                }
                case api::v2::DataObject::AttributeType::TYPE_DOUBLE: {
                    double value = object->GetAttributeDouble(attributeName.c_str());
                    objectBuilder.setDoubleAttributeValue(id, attributeName, value);
                    break;
                }
            }
        }
    }

    auto children = api.getChildren(*object);
    for (auto itC = children.begin(); itC != children.end(); ++itC) {
        auto &child = *itC;
        traverse(api, objectBuilder, child, idCount, id, idToParentId, fillDescription);
    }
}

}

}

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_powsybl_powerfactory_db_JniDatabaseReader
 * Method:    read
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lcom/powsybl/powerfactory/db/DataObjectBuilder;)V
 */
JNIEXPORT void JNICALL Java_com_powsybl_powerfactory_db_JniDatabaseReader_read
(JNIEnv * env, jobject, jstring j_powerFactoryHomeDir, jstring j_projectName, jobject j_objectBuilder) {
    try {
        std::string powerFactoryHomeDir = powsybl::jni::StringUTF(env, j_powerFactoryHomeDir).toStr();
        std::string projectName = powsybl::jni::StringUTF(env, j_projectName).toStr();

        pf::Api api(powerFactoryHomeDir);
        api.activateProject(projectName);
        auto project = api.makeObjectUniquePtr(api.getApplication()->GetActiveProject());

        jni::ComPowsyblPowerFactoryDbDataObjectBuilder objectBuilder(env, j_objectBuilder);

        // create objects
        long idCount = 0;
        std::map<long, long> idToParentId;
        pf::traverse(api, objectBuilder, project, idCount, -1, idToParentId, false);

        // set parents
        for (auto it = idToParentId.begin(); it != idToParentId.end(); ++it) {
            long id = it->first;
            long parentId = it->second;
            if (parentId != -1) {
                objectBuilder.setObjectParent(id, parentId);
            }
        }

    } catch (const std::exception& e) {
        powsybl::jni::throwPowsyblException(env, e.what());
    } catch (...) {
        powsybl::jni::throwPowsyblException(env, "Unknown exception");
    }
}

#ifdef __cplusplus
}
#endif
