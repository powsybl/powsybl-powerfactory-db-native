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

int getRowCount(api::v2::DataObject* object, const std::string& attributeName) {
    int rowCount;
    int columnCount;
    object->GetAttributeSize(attributeName.c_str(), rowCount, columnCount);
    return rowCount;
}

void traverse(Api &api, const jni::ComPowsyblPowerFactoryDbDataObjectBuilder &objectBuilder,
              api::v2::DataObject* object, long parentId, std::map<long, long>& idToParentId,
              std::map<std::string, int>& attributeTypes, bool fillDescription) {
    // create class if not already exist
    std::string className = api.makeValueUniquePtr(object->GetClassNameA())->GetString();
    objectBuilder.createClass(className);

    // create object
    long id = api.getObjectId(object);
    idToParentId.insert({id, parentId});
    objectBuilder.createObject(id, className);

    auto attributeNames = api.getAttributeNames(*object);
    for (auto itN = attributeNames.begin(); itN != attributeNames.end(); ++itN) {
        auto& attributeName = *itN;
        auto itType = attributeTypes.find(attributeName);
        int type;
        if (itType == attributeTypes.end()) {
            type = object->GetAttributeType(attributeName.c_str());
            attributeTypes.insert({attributeName, type});
        } else {
            type = itType->second;
        }
        if (type != api::v2::DataObject::AttributeType::TYPE_INVALID) { // what does it mean?
            // create attribute if not already exist
            std::string description;
            if (fillDescription) {
                auto descriptionValue = object->GetAttributeDescription(attributeName.c_str());
                if (descriptionValue) {
                    description = api.makeValueUniquePtr(descriptionValue)->GetString();
                }
            }
            objectBuilder.createAttribute(className, attributeName, type, description);

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

                case api::v2::DataObject::AttributeType::TYPE_OBJECT: {
                    auto otherObject = object->GetAttributeObject(attributeName.c_str());
                    objectBuilder.setObjectAttributeValue(id, attributeName, api.addObject(otherObject));
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_INTEGER_VEC: {
                    int rowCount = getRowCount(object, attributeName);
                    int col = 0;
                    std::vector<int> values;
                    values.reserve(rowCount);
                    for (int row = 0; row < rowCount; row++) {
                        int value = object->GetAttributeInt(attributeName.c_str(), row, col);
                        values.push_back(value);
                    }
                    objectBuilder.setIntVectorAttributeValue(id, attributeName, values);
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_INTEGER64_VEC: {
                    int rowCount = getRowCount(object, attributeName);
                    int col = 0;
                    std::vector<long> values;
                    values.reserve(rowCount);
                    for (int row = 0; row < rowCount; row++) {
                        long value = object->GetAttributeInt64(attributeName.c_str(), row, col);
                        values.push_back(value);
                    }
                    objectBuilder.setLongVectorAttributeValue(id, attributeName, values);
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_DOUBLE_VEC: {
                    int rowCount = getRowCount(object, attributeName);
                    int col = 0;
                    std::vector<double> values;
                    values.reserve(rowCount);
                    for (int row = 0; row < rowCount; row++) {
                        double value = object->GetAttributeDouble(attributeName.c_str(), row, col);
                        values.push_back(value);
                    }
                    objectBuilder.setDoubleVectorAttributeValue(id, attributeName, values);
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_STRING_VEC: {
                    int rowCount = getRowCount(object, attributeName);
                    if (rowCount >= 0) { // sometimes we get -1 ????
                        std::vector<std::string> values;
                        values.reserve(rowCount);
                        for (int row = 0; row < rowCount; row++) {
                            std::string value = api.makeValueUniquePtr(object->GetAttributeString(attributeName.c_str(), row))->GetString();
                            values.push_back(value);
                        }
                        objectBuilder.setStringVectorAttributeValue(id, attributeName, values);
                    }
                    break;
                }

                case api::v2::DataObject::AttributeType::TYPE_OBJECT_VEC: {
                    int rowCount = getRowCount(object, attributeName);
                    std::vector<long> values;
                    values.reserve(rowCount);
                    for (int row = 0; row < rowCount; row++) {
                        auto otherObject = object->GetAttributeObject(attributeName.c_str(), row);
                        values.push_back(api.addObject(otherObject));
                    }
                    objectBuilder.setObjectVectorAttributeValue(id, attributeName, values);
                    break;
                }
            }
        }
    }

    auto children = api.getChildren(*object);
    for (auto itC = children.begin(); itC != children.end(); ++itC) {
        auto &child = *itC;
        traverse(api, objectBuilder, child, id, idToParentId, attributeTypes, fillDescription);
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
        auto project = api.activateProject(projectName);

        jni::ComPowsyblPowerFactoryDbDataObjectBuilder objectBuilder(env, j_objectBuilder);

        // create objects
        std::map<long, long> idToParentId;
        std::map<std::string, int> attributeTypes;
        pf::traverse(api, objectBuilder, project, -1, idToParentId, attributeTypes, false);

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
