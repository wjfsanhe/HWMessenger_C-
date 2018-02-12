/*
 * Copyright (C) 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "HWMessenger"


#include <stdint.h>
#include <unistd.h>

#include <sys/types.h>

#include "HWMessenger.h"
#include "HWUpdator.h"
#include "HWMonitor.h"
#include "IHWControllerClient.h"

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/PermissionCache.h>
#include <hardware/input.h>
#include <cutils/log.h>
#include <private/android_filesystem_config.h>


namespace android {

const String16 sAccessHWMessengerPermission("android.permission.ACCESS_HW_MESSENGER");
const String16 sDumpPermission("android.permission.DUMP");


HWMessenger::HWMessenger() :
        BnHWMessenger() {
    ALOGI("HWMessenger is starting");
    //new key monitor thread.
}
void HWMessenger::onFirstRef(){
    mUpdator = new HWUpdator(this);
    mMonitor = new HWMonitor(mUpdator);

}

HWMessenger::~HWMessenger() {
}
status_t HWMessenger::dump(int fd, const Vector<String16>& args) {
    String8 result;
    const IPCThreadState* ipc = IPCThreadState::self();
    const int pid = ipc->getCallingPid();
    const int uid = ipc->getCallingUid();
    if ((uid != AID_SHELL)
            && !PermissionCache::checkPermission(sDumpPermission, pid, uid)) {
        result.appendFormat("Permission Denial: "
                "can't dump hw messenger from pid=%d, uid=%d\n", pid, uid);
    } else {
        dumpInternal(result);
    }
    write(fd, result.string(), result.size());
    return OK;
}

void HWMessenger::dumpInternal(String8& result) {
    result.append("HW MESSENGER (dumpsys HWMessenger)\n");
}
status_t HWMessenger::registerCallback(const sp<IBinder> binder) {
    if (binder != nullptr) {
        if (mCallbacks.size() > MAX_CLIENT_SIZE){
            ALOGE("Error , client beyond MAX_CLIENT_SIZE (%ld)!", MAX_CLIENT_SIZE);
        } else {
            ALOGD("add one client !");
            mCallbacks.push_back(IHWMessengerCallback::asInterface(binder));
            curback = IHWMessengerCallback::asInterface(binder);
            curback->onKey(String16("AAA"), 0x68, 0x77, 0x88);
            controller->setCallback(curback);
        }
    }
    return BAD_INDEX;

}
status_t HWMessenger::unregisterCallback(const sp<IBinder> binder) {
    if (binder != nullptr) {
       int size = mCallbacks.size();
       sp<IHWMessengerCallback> callback = IHWMessengerCallback::asInterface(binder);
       for (int i = 0; i < size; i++) {
            if( mCallbacks[i] == callback) {
                ALOGD("remove one client !");
                mCallbacks.removeAt(i);
            }
       }
    }
    return NO_ERROR;
}


void HWMessenger::updateKey(AString deviceName, int keyCode, int value, int flags)
{
    int size = mCallbacks.size();
    curback->onKey(String16(deviceName.c_str()), keyCode, value, flags);
    for (int i = 0; i < size; i++) {
        sp<IHWMessengerCallback> callback = mCallbacks[i];
        ALOGI("send key to callback[%d]:[%s] %04x ", i, deviceName.c_str(), keyCode);
        callback->onKey(String16(deviceName.c_str()), keyCode, value, flags);
    }
}
sp<IHWControllerClient> HWMessenger::createHWControllerClient()
{
    sp<IHWControllerClient> bclient;
    sp<BnHWControllerClient> client(new BnHWControllerClient());
    bclient = client;
    return bclient;
}


}; // namespace android
