/*
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
 *  @author   Jianfeng
 *  @version  1.0
 *  @date     2018/02/05
 *
 *
 */

#ifndef ANDROID_IHW_MESSENGER_CALLBACK_H
#define  ANDROID_IHW_MESSENGER_CALLBACK_H


#include <utils/Errors.h>
#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <utils/String8.h>
#include <utils/String16.h>

namespace android {

// ----------------------------------------------------------------------------

class IHWMessengerCallback : public IInterface
{
public:
    DECLARE_META_INTERFACE(HWMessengerCallback)
public:
    virtual void onKey(String16 deviceName, int keyCode, int value, int flags)=0;
};


// ----------------------------------------------------------------------------

}; // namespace android

#endif


