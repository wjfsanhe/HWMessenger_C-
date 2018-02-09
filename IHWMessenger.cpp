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
 *
 * Author: Jianfeng
 * Date  : 2018/02/05
 *
 */
 #define ATRACE_TAG "HWMessenger"

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <cutils/log.h>

#include "IHWMessenger.h"

namespace android {

class BpHWMessenger : public BpInterface<IHWMessenger> {
public:
    BpHWMessenger(const sp<IBinder>& impl) :
            BpInterface<IHWMessenger>(impl) { }

    virtual status_t registerCallback(const sp<IHWMessengerCallback>& callback) {
        Parcel data, reply;
        data.writeInterfaceToken(IHWMessenger::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(callback));
        remote()->transact(BnHWMessenger::REGISTER_CALLBACK, data, &reply);
        return reply.readInt32();
    }
    virtual status_t unregisterCallback(const sp<IHWMessengerCallback>& callback) {
        Parcel data, reply;
        data.writeInterfaceToken(IHWMessenger::getInterfaceDescriptor());
        data.writeStrongBinder(IInterface::asBinder(callback));
        remote()->transact(BnHWMessenger::UNREGISTER_CALLBACK, data, &reply);
        return reply.readInt32();
    }
};

IMPLEMENT_META_INTERFACE(HWMessenger, "com.qiyi.hwmessenger.IHWMessenger");


status_t BnHWMessenger::onTransact(
        uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags) {
    switch(code) {
    case REGISTER_CALLBACK: {
        CHECK_INTERFACE(IHWMessenger, data, reply);
        sp<IBinder> binder = data.readStrongBinder();
        registerCallback(binder);
        reply->writeInt32(0);
        break;
    }
    case UNREGISTER_CALLBACK: {
        CHECK_INTERFACE(IHWMessenger, data, reply);
        sp<IBinder> binder = data.readStrongBinder();
        status_t err = unregisterCallback(binder);
        reply->writeInt32(err);
        break;
    }
    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
    return NO_ERROR;
}
status_t BnHWMessenger::registerCallback(const sp<IBinder> binder) {
    if (binder != nullptr) {
        if (mCallbacks.size() > MAX_CLIENT_SIZE){
            ALOGE("Error , client beyond MAX_CLIENT_SIZE (%ld)!", MAX_CLIENT_SIZE);
        } else {
            ALOGD("add one client !");
            mCallbacks.push_back(IHWMessengerCallback::asInterface(binder));
        }
    }
    return BAD_INDEX;

}
status_t BnHWMessenger::unregisterCallback(const sp<IBinder> binder) {
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

};
