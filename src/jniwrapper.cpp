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

void ComPowsyblPowerFactoryDbDataObjectBuilder::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("com/powsybl/powerfactory/db/DataObjectBuilder");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _createClass = env->GetMethodID(_cls, "createClass", "(Ljava/lang/String;)Z");
        _createAttribute = env->GetMethodID(_cls, "createAttribute", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)V");
        _createObject = env->GetMethodID(_cls, "createObject", "(JLjava/lang/String;J)V");
    }
}

ComPowsyblPowerFactoryDbDataObjectBuilder::ComPowsyblPowerFactoryDbDataObjectBuilder(JNIEnv *env, jobject obj)
    : JniWrapper<jobject>(env, obj) {
    init(env);
}

bool ComPowsyblPowerFactoryDbDataObjectBuilder::createClass(const std::string& name) const {
    jstring j_name = _env->NewStringUTF(name.c_str());
    return (jboolean) _env->CallObjectMethod(_obj, _createClass, j_name);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::createAttribute(const std::string& className, const std::string& attributeName, int type, const std::string& description) const {
    jstring j_className = _env->NewStringUTF(className.c_str());
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    jstring j_description = _env->NewStringUTF(description.c_str());
    _env->CallObjectMethod(_obj, _createAttribute, j_className, j_attributeName, (jint) type, j_description);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::createObject(long id, const std::string& className, long parentId) const {
    jstring j_className = _env->NewStringUTF(className.c_str());
    _env->CallObjectMethod(_obj, _createObject, (jlong) id, j_className, (jlong) parentId);
}

void throwPowsyblException(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("com/powsybl/commons/PowsyblException");
    env->ThrowNew(clazz, msg);
}

}  // namespace jni

}  // namespace powsybl
