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

void ComPowsyblPowerFactoryDbDataObjectBuilder::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("com/powsybl/powerfactory/db/DataObjectBuilder");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _createClass = env->GetMethodID(_cls, "createClass", "(Ljava/lang/String;)Z");
    }
}

ComPowsyblPowerFactoryDbDataObjectBuilder::ComPowsyblPowerFactoryDbDataObjectBuilder(JNIEnv *env, jobject obj)
    : JniWrapper<jobject>(env, obj) {
    init(env);
}

bool ComPowsyblPowerFactoryDbDataObjectBuilder::createClass(const std::string& name) const {
    jstring j_name = _env->NewStringUTF(name.c_str());
    (jboolean) _env->CallObjectMethod(_obj, _createClass, j_name);
    return false;
}

void throwPowsyblException(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("com/powsybl/commons/PowsyblException");
    env->ThrowNew(clazz, msg);
}

}  // namespace jni

}  // namespace powsybl
