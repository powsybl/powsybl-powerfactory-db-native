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

jclass JavaLangDouble::_cls = nullptr;
jmethodID JavaLangDouble::_constructor = nullptr;

void JavaLangDouble::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("java/lang/Double");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _constructor = env->GetMethodID(_cls, "<init>", "(D)V");
    }
}

JavaLangDouble::JavaLangDouble(JNIEnv* env, double d)
    : JniWrapper<jobject>(env, nullptr) {
    init(env);
    _obj = env->NewObject(_cls, _constructor, d);
}

jclass JavaLangInteger::_cls = nullptr;
jmethodID JavaLangInteger::_constructor = nullptr;

void JavaLangInteger::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("java/lang/Integer");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _constructor = env->GetMethodID(_cls, "<init>", "(I)V");
    }
}

JavaLangInteger::JavaLangInteger(JNIEnv* env, int i)
    : JniWrapper<jobject>(env, nullptr) {
    init(env);
    _obj = env->NewObject(_cls, _constructor, i);
}

jclass JavaLangLong::_cls = nullptr;
jmethodID JavaLangLong::_constructor = nullptr;

void JavaLangLong::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("java/lang/Long");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _constructor = env->GetMethodID(_cls, "<init>", "(J)V");
    }
}

JavaLangLong::JavaLangLong(JNIEnv* env, long l)
    : JniWrapper<jobject>(env, nullptr) {
    init(env);
    _obj = env->NewObject(_cls, _constructor, l);
}

jclass JavaUtilArrayList::_cls = nullptr;
jmethodID JavaUtilArrayList::_constructor = nullptr;
jmethodID JavaUtilArrayList::_add = nullptr;

void JavaUtilArrayList::init(JNIEnv* env) {
    if (!_cls) {
        jclass localCls = env->FindClass("java/util/ArrayList");
        _cls = reinterpret_cast<jclass>(env->NewGlobalRef(localCls));
        _constructor = env->GetMethodID(_cls, "<init>", "()V");
        _add = env->GetMethodID(_cls, "add", "(Ljava/lang/Object;)Z");
    }
}

JavaUtilArrayList::JavaUtilArrayList(JNIEnv* env)
    : JniWrapper<jobject>(env, nullptr) {
    init(env);
    _obj = env->NewObject(_cls, _constructor);
}

void JavaUtilArrayList::add(jobject obj) {
    _env->CallObjectMethod(_obj, _add, obj);
}

jclass ComPowsyblPowerFactoryDbDataObjectBuilder::_cls = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createClass = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createAttribute = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_createObject = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setObjectParent = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setStringAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setIntAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setLongAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setDoubleAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setObjectAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setIntVectorAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setLongVectorAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setDoubleVectorAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setStringVectorAttributeValue = nullptr;
jmethodID ComPowsyblPowerFactoryDbDataObjectBuilder::_setObjectVectorAttributeValue = nullptr;

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
        _setLongAttributeValue = env->GetMethodID(_cls, "setLongAttributeValue", "(JLjava/lang/String;J)V");
        _setDoubleAttributeValue = env->GetMethodID(_cls, "setDoubleAttributeValue", "(JLjava/lang/String;D)V");
        _setObjectAttributeValue = env->GetMethodID(_cls, "setObjectAttributeValue", "(JLjava/lang/String;J)V");
        _setIntVectorAttributeValue = env->GetMethodID(_cls, "setIntVectorAttributeValue", "(JLjava/lang/String;Ljava/util/List;)V");
        _setLongVectorAttributeValue = env->GetMethodID(_cls, "setLongVectorAttributeValue", "(JLjava/lang/String;Ljava/util/List;)V");
        _setDoubleVectorAttributeValue = env->GetMethodID(_cls, "setDoubleVectorAttributeValue", "(JLjava/lang/String;Ljava/util/List;)V");
        _setStringVectorAttributeValue = env->GetMethodID(_cls, "setStringVectorAttributeValue", "(JLjava/lang/String;Ljava/util/List;)V");
        _setObjectVectorAttributeValue = env->GetMethodID(_cls, "setObjectVectorAttributeValue", "(JLjava/lang/String;Ljava/util/List;)V");
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

void ComPowsyblPowerFactoryDbDataObjectBuilder::setLongAttributeValue(long objectId, const std::string &attributeName, long value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    _env->CallObjectMethod(_obj, _setLongAttributeValue, (jlong) objectId, j_attributeName, (jlong) value);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setDoubleAttributeValue(long objectId, const std::string &attributeName, double value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    _env->CallObjectMethod(_obj, _setDoubleAttributeValue, (jlong) objectId, j_attributeName, (jdouble) value);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setObjectAttributeValue(long objectId, const std::string &attributeName, long otherObjectId) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    _env->CallObjectMethod(_obj, _setObjectAttributeValue, (jlong) objectId, j_attributeName, (jlong) otherObjectId);
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setIntVectorAttributeValue(long objectId,
                                                                           const std::string& attributeName,
                                                                           const std::vector<int>& value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    JavaUtilArrayList list(_env);
    for (auto i : value) {
        list.add(JavaLangInteger(_env, i).obj());
    }
    _env->CallObjectMethod(_obj, _setIntVectorAttributeValue, (jlong) objectId, j_attributeName, list.obj());
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setLongVectorAttributeValue(long objectId,
                                                                            const std::string& attributeName,
                                                                            const std::vector<long>& value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    JavaUtilArrayList list(_env);
    for (auto l : value) {
        list.add(JavaLangLong(_env, l).obj());
    }
    _env->CallObjectMethod(_obj, _setLongVectorAttributeValue, (jlong) objectId, j_attributeName, list.obj());
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setDoubleVectorAttributeValue(long objectId,
                                                                              const std::string& attributeName,
                                                                              const std::vector<double>& value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    JavaUtilArrayList list(_env);
    for (auto d : value) {
        list.add(JavaLangDouble(_env, d).obj());
    }
    _env->CallObjectMethod(_obj, _setDoubleVectorAttributeValue, (jlong) objectId, j_attributeName, list.obj());
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setStringVectorAttributeValue(long objectId,
                                                                              const std::string& attributeName,
                                                                              const std::vector<std::string>& value) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    JavaUtilArrayList list(_env);
    for (auto str : value) {
        list.add(_env->NewStringUTF(str.c_str()));
    }
    _env->CallObjectMethod(_obj, _setStringVectorAttributeValue, (jlong) objectId, j_attributeName, list.obj());
}

void ComPowsyblPowerFactoryDbDataObjectBuilder::setObjectVectorAttributeValue(long objectId,
                                                                              const std::string& attributeName,
                                                                              const std::vector<long>& otherObjectsIds) const {
    jstring j_attributeName = _env->NewStringUTF(attributeName.c_str());
    JavaUtilArrayList list(_env);
    for (auto otherObjectId : otherObjectsIds) {
        list.add(JavaLangLong(_env, otherObjectId).obj());
    }
    _env->CallObjectMethod(_obj, _setObjectVectorAttributeValue, (jlong) objectId, j_attributeName, list.obj());
}

void throwPowsyblException(JNIEnv* env, const char* msg) {
    jclass clazz = env->FindClass("com/powsybl/commons/PowsyblException");
    env->ThrowNew(clazz, msg);
}

}  // namespace jni

}  // namespace powsybl
