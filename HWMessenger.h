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
 * Author : Jianfeng
 * Date   : 2018/02/05
 *
 */

#ifndef ANDROID_HW_MESSENGER_H
#define ANDROID_HW_MESSENGER_H

#include <stdint.h>
#include <sys/types.h>

#include "IHWMessenger.h"
#include <cutils/compiler.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/StrongPointer.h>
#include <media/stagefright/foundation/AHandler.h>



namespace android {

class HWUpdator;
class HWMonitor;
class IHWControllerClient;

class HWMessenger : public BnHWMessenger
{
public:
    static char const* getServiceName() ANDROID_API {
        return "HWMessenger";
    }

    HWMessenger() ANDROID_API;

    virtual status_t dump(int fd, const Vector<String16>& args);
    virtual void updateKey(int keyCode, int value, int flags);

private:
    virtual ~HWMessenger();
    void dumpInternal(String8& result);
protected:
    virtual sp<IHWControllerClient> createHWControllerClient();
    virtual void onFirstRef();
private:
    sp<HWUpdator> mUpdator;
    sp<HWMonitor> mMonitor;
};

} // namespace android

#endif //
