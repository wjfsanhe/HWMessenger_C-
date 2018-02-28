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

#ifndef IHW_MESSENGER_H
#define IHW_MESSENGER_H

#include <stdint.h>
#include <sys/types.h>
#include <binder/IInterface.h>
#include "IHWMessengerCallback.h"
#include "IHWControllerClient.h"
#include "HWControllerThread.h"
#include <media/stagefright/foundation/AString.h>

namespace android {


//class IHWControllerClient;
//class IHWMessengerCallback;
/*
 * This class defines the Binder IPC interface for accessing various
 * InputFlinger features.
 */
class IHWMessenger : public IInterface {
public:
    DECLARE_META_INTERFACE(HWMessenger);
};


/**
 * Binder implementation.
 */
class BnHWMessenger : public BnInterface<IHWMessenger>
{
public:
    BnHWMessenger(){};
    enum {
        REGISTER_CALLBACK = IBinder::FIRST_CALL_TRANSACTION,
        UNREGISTER_CALLBACK = IBinder::FIRST_CALL_TRANSACTION + 1,
        CREATE_HW_CONTROLLER_CLIENT = IBinder::FIRST_CALL_TRANSACTION + 2,
    };

    virtual status_t onTransact(uint32_t code, const Parcel& data,
            Parcel* reply, uint32_t flags = 0);
    virtual void updateKey(AString deviceName, int keyCode, int value, int flags) = 0;

protected:
    //we support multi client.
    const uint64_t MAX_CLIENT_SIZE = 2;
    virtual status_t registerCallback(const sp<IBinder> binder) = 0;
    virtual status_t unregisterCallback(const sp<IBinder> binder) = 0;
    virtual sp<IHWControllerClient> createHWControllerClient() = 0;
};

} // namespace android

#endif // _LIBINPUT_IHW_MESSENGER_H
