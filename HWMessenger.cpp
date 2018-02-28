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
using namespace std;
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
    IPCThreadState* ipc = IPCThreadState::self();
    int pid = ipc->getCallingPid();
    const int uid = ipc->getCallingUid();
    ALOGD("pid = %d, uid = %d", pid, uid);
    result.append("HW MESSENGER (dumpsys HWMessenger)\n");
}

void HWMessenger::binderDied(const wp<IBinder>& binder)
{
      // woah, callback died!
      ALOGD("detect one callback died.bw[%p]-bs[%p]", binder.unsafe_get(), binder.promote().get());
      unregisterCallback(binder.promote());

}
status_t HWMessenger::registerCallback(const sp<IBinder> binder) {

    if (binder != nullptr) {
        if (mCallbackMap.size() > MAX_CLIENT_SIZE){
            ALOGE("Error , client beyond MAX_CLIENT_SIZE (%ld)!", MAX_CLIENT_SIZE);
        } else {
            sp<IHWMessengerCallback> callback = IHWMessengerCallback::asInterface(binder);
            //1
            //mCallbacks.push_back(callback);
            //2
            mCallbackMap.insert(std::make_pair((long)binder.get(),callback));

            ALOGD("add one client [%zu]! c[%p]-b[%p]", mCallbackMap.size(), callback.get(),binder.get());
            binder->linkToDeath(this);
        }
    }
    return BAD_INDEX;

}
status_t HWMessenger::unregisterCallback(const sp<IBinder> binder) {
    if (binder != nullptr) {
       int size = mCallbackMap.size();
       sp<IHWMessengerCallback> callback = IHWMessengerCallback::asInterface(binder);
       ALOGD("want to unregister c[%p]-b[%p] --- size[%d]", callback.get(), binder.get(), size);
       if (mCallbackMap.count((long)binder.get())) {
            ALOGD("remove one client [%p]", binder.get());
            mCallbackMap.erase((long)binder.get());
       }

#if 0
       for (int i = 0; i < size; i++) {
            ALOGD("try [%d] : m[%p] vs [%p]", i, mCallbacks[i].get(), callback.get());
            if( mCallbacks[i].get() == callback.get()) {
                ALOGD("remove one client !");
                mCallbacks.removeAt(i);
            }
       }
#endif
    }
    return NO_ERROR;
}


void HWMessenger::updateKey(AString deviceName, int keyCode, int value, int flags)
{
#if 0
    int size = mCallbacks.size();
    for (int i = 0; i < size; i++) {
        sp<IHWMessengerCallback> callback = mCallbacks[i];
        ALOGI("send key to callback[%d]:[%s] %04x ", i, deviceName.c_str(), keyCode);
        callback->onKey(String16(deviceName.c_str()), keyCode, value, flags);
    }
#endif
    std::map<long, sp<IHWMessengerCallback>>::iterator it;
    for (it = mCallbackMap.begin(); it != mCallbackMap.end(); ++it) {
        it->second->onKey(String16(deviceName.c_str()), keyCode, value, flags);
    }

}
sp<IHWControllerClient> HWMessenger::createHWControllerClient()
{
    sp<IHWControllerClient> bclient;
    ALOGI("[HWMessenger] --- createHWControllerClient");
    sp<BnHWControllerClient> client(new BnHWControllerClient());
    bclient = client;
    return bclient;
}


}; // namespace android
