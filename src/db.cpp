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
#include <iostream>
#include "jniwrapper.hpp"
#include "api.h"

namespace pf = powsybl::powerfactory;

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

        pf::Api api(R"(C:\Program Files\DIgSILENT\PowerFactory 2022 SP1)");
        api.activateProject("Transmission System");
        auto project = api.makeObjectUniquePtr(api.getApplication()->GetActiveProject());

    } catch (const std::exception& e) {
        powsybl::jni::throwPowsyblException(env, e.what());
    } catch (...) {
        powsybl::jni::throwPowsyblException(env, "Unknown exception");
    }
}

#ifdef __cplusplus
}
#endif
