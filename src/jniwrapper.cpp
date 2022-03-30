/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file jniwrapper.cpp
 * @author Geoffroy Jamgotchian <geoffroy.jamgotchian at rte-france.com>
 */

#include "jniwrapper.hpp"

namespace powsybl {

namespace jni {

jclass ComPowsyblPowerFactoryDbDataObjectBuilder::_cls = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createClass = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createAttribute = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createObject = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setObjectParent = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setStringAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setIntAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setDoubleAttributeValue = nullptr;

void ComPowsyblPowerFactoryDbDataObjectBuilder::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("com/powsybl/powerfactory/db/DataObjectBuilder");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _createClass = env->GetMethodID(_cls, "createClass", "(Ljava/lang/String;)V");
        _createAttribute = env->GetMethodID(_cls, "createAttribute", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V");
        _createObject = env->GetMethodID(_cls, "createObject", "(JLjava/lang/String;)V");
        _setObjectParent = env->GetMethodID(_cls, "setObjectParent", "(JJ)V");
        _setStringAttributeValue = env->GetMethodID(_cls, "setStringAttributeValue", "(JLjava/lang/String;Ljava/lang/String;)V");
        _setIntAttributeValue = env->GetMethodID(_cls, "setIntAttributeValue", "(JLjava/lang/String;I)V");
        _setDoubleAttributeValue = env->GetMethodID(_cls, "setDoubleAttributeValue", "(JLjava/lang/String;D)V");
    }
}

ComPowsyblPowerFactoryDbDataObjectBuilder::ComPowsyblPowerFactoryDbDataObjectBuilder(JNIEnv *env, jobject obj)
    : JniWrapper<jobject>(env, obj) {
    init(env);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::createClass(const std::string& name) const {
    jstring j_name = _env->NewStringUTF(name.c_str());
    _env->CallObjectMethod(_obj, _createClass, j_name);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::createAttribute(const std::string& className, const std::string& attributeName, int type, const std::string& description) const {
    jstring j_className = _env->NewStringUTF(className.c_str());
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    jstring j_description = _env->NewStringUTF(description.c_str());
    _env->CallObjectMethod(_obj, _createAttribute, j_className, j_attributeName, (jint) type, j_description);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::createObject(long id, const std::string& className) const {
    jstring j_className = _env->NewStringUTF(className.c_str());
    _env->CallObjectMethod(_obj, _createObject, (jlong) id, j_className);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setObjectParent(long id, long parentId) const {
    _env->CallObjectMethod(_obj, _setObjectParent, (jlong) id, (jlong) parentId);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setStringAttributeValue(long objectId, const std::string &attributeName,
                                                                        const std::string& value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    jstring j_value = _env->NewStringUTF(value.c_str());
    _env->CallObjectMethod(_obj, _setStringAttributeValue, (jlong) objectId, j_attributeName, j_value);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setIntAttributeValue(long objectId, const std::string &attributeName, int value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    _env->CallObjectMethod(_obj, _setIntAttributeValue, (jlong) objectId, j_attributeName, (jint) value);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setDoubleAttributeValue(long objectId, const std::string &attributeName, double value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    _env->CallObjectMethod(_obj, _setDoubleAttributeValue, (jlong) objectId, j_attributeName, (jdouble) value);
}

void throwPowsyblException(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("com/powsybl/commons/PowsyblException");
    env->ThrowNew(clazz, msg);
}

}  // namespace jni

}  // namespace powsybl
