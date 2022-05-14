/**
 * Copyright (c) 2022, RTE (http://www.rte-france.com)
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @file jniwrapper.cpp
 * @author Geoffroy Jamgotchian <geoffroy.jamgotchian at rte-france.com>
 */

#ifndef JNIWRAPPER_HPP
#define JNIWRAPPER_HPP

#include <string>
#include <vector>
#include <jni.h>

namespace powsybl {

namespace jni {

template<typename T>
class JniWrapper {
public:
    JniWrapper(const JniWrapper&) = delete;

    virtual ~JniWrapper() = default;

    JniWrapper& operator=(const JniWrapper&) = delete;

    JNIEnv* env() const {
        return _env;
    }

    const T& obj() const {
        return _obj;
    }

protected:
    JniWrapper(JNIEnv* env, T obj) :
        _env(env),
        _obj(obj) {
    }

    JNIEnv* _env;
    T _obj;
};

class StringUTF : public JniWrapper<jstring> {
public:
    StringUTF(JNIEnv* env, jstring jstr) :
        JniWrapper<jstring>(env, jstr),
        _ptr(nullptr) {
    }

    ~StringUTF() override {
        if (_ptr) {
            _env->ReleaseStringUTFChars(_obj, _ptr);
        }
    }

    size_t length() const {
        return _env->GetStringUTFLength(_obj);
    }

    const char* get() const {
        if (!_ptr) {
            _ptr = _env->GetStringUTFChars(_obj, nullptr);
        }
        return _ptr;
    }

    std::string toStr() const {
        if (!_ptr) {
            _ptr = _env->GetStringUTFChars(_obj, nullptr);
        }
        return {_ptr};
    }

private:
    mutable const char* _ptr;
};

class JavaLangInteger : public JniWrapper<jobject> {
public:
    JavaLangInteger(JNIEnv* env, int i);

    static void init(JNIEnv* env);

private:
    static jclass _cls;
    static jmethodID _constructor;
};

class JavaLangLong : public JniWrapper<jobject> {
public:
    JavaLangLong(JNIEnv* env, long l);

    static void init(JNIEnv* env);

private:
    static jclass _cls;
    static jmethodID _constructor;
};

class JavaLangDouble : public JniWrapper<jobject> {
public:
    JavaLangDouble(JNIEnv* env, double d);

    static void init(JNIEnv* env);

private:
    static jclass _cls;
    static jmethodID _constructor;
};

class JavaUtilArrayList : public JniWrapper<jobject> {
public:
    JavaUtilArrayList(JNIEnv* env);

    static void init(JNIEnv* env);

    void add(jobject obj);

private:
    static jclass _cls;
    static jmethodID _constructor;
    static jmethodID _add;
};

class ComPowsyblPowerFactoryDbDataObjectBuilder : public JniWrapper<jobject> {
public:
    ComPowsyblPowerFactoryDbDataObjectBuilder(JNIEnv* env, jobject obj);

    static void init(JNIEnv* env);

    void createClass(const std::string& name) const;

    void createAttribute(const std::string& className, const std::string& attributeName, int type, const std::string& description) const;

    void createObject(long id, const std::string& className) const;

    void setObjectParent(long id, long parentId) const;

    void setStringAttributeValue(long objectId, const std::string& attributeName, const std::string& value) const;

    void setIntAttributeValue(long objectId, const std::string& attributeName, int value) const;

    void setLongAttributeValue(long objectId, const std::string& attributeName, long value) const;

    void setDoubleAttributeValue(long objectId, const std::string& attributeName, double value) const;

    void setObjectAttributeValue(long objectId, const std::string& attributeName, long otherObjectId) const;

    void setIntVectorAttributeValue(long objectId, const std::string& attributeName, const std::vector<int>& value) const;

    void setLongVectorAttributeValue(long objectId, const std::string& attributeName, const std::vector<long>& value) const;

    void setDoubleVectorAttributeValue(long objectId, const std::string& attributeName, const std::vector<double>& value) const;

    void setStringVectorAttributeValue(long objectId, const std::string& attributeName, const std::vector<std::string>& value) const;

    void setObjectVectorAttributeValue(long objectId, const std::string& attributeName, const std::vector<long>& otherObjectsIds) const;

    void setDoubleMatrixAttributeValue(long objectId, const std::string& attributeName, int rowCount, int columnCount, const std::vector<double>& value) const;

private:
    static jclass _cls;
    static jmethodID _createClass;
    static jmethodID _createAttribute;
    static jmethodID _createObject;
    static jmethodID _setObjectParent;
    static jmethodID _setStringAttributeValue;
    static jmethodID _setIntAttributeValue;
    static jmethodID _setLongAttributeValue;
    static jmethodID _setDoubleAttributeValue;
    static jmethodID _setObjectAttributeValue;
    static jmethodID _setIntVectorAttributeValue;
    static jmethodID _setLongVectorAttributeValue;
    static jmethodID _setDoubleVectorAttributeValue;
    static jmethodID _setStringVectorAttributeValue;
    static jmethodID _setObjectVectorAttributeValue;
    static jmethodID _setDoubleMatrixAttributeValue;
};

void throwPowsyblException(JNIEnv* env, const char* msg);

}  // namespace jni

}  // namespace powsybl

#endif // JNIWRAPPER_HPP
