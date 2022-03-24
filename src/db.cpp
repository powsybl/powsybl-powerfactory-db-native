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

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_powsybl_powerfactory_db_JniDatabaseReader
 * Method:    read
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lcom/powsybl/powerfactory/db/DataObjectBuilder;)V
 */
JNIEXPORT void JNICALL Java_com_powsybl_powerfactory_db_JniDatabaseReader_read
(JNIEnv *, jobject, jstring j_powerFactoryHome, jstring j_projectName, jobject j_objectBuilder) {

}

#ifdef __cplusplus
}
#endif
