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

class ComPowsyblPowerFactoryDbDataObjectBuilder : public JniWrapper<jobject> {
public:
    ComPowsyblPowerFactoryDbDataObjectBuilder(JNIEnv* env, jobject obj);

    static void init(JNIEnv* env);

    bool createClass(const std::string& name) const;
private:
    static jclass _cls;
    static jmethodID _createClass;
};

void throwPowsyblException(JNIEnv* env, const char* msg);

}  // namespace jni

}  // namespace powsybl

#endif // JNIWRAPPER_HPP
