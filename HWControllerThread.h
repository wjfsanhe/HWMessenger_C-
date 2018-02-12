//
// Created by wangjf on 2/6/18.
//



#ifndef HWMESSENGER_HWCONTROLLERTHREAD_H
#define HWMESSENGER_HWCONTROLLERTHREAD_H
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/ADebug.h>
#include <stdint.h>
#include <sys/types.h>
#include <utils/Log.h>
//#include "IHWMessengerCallback.h"
//#include "HWMessenger.h"

namespace android {

class IHWMessengerCallback;
class HWMessenger;
class HWControllerThread : public AHandler {
public:
enum {
    kWhatWriteSysfs = 101,
    kWhatReadSysfs = 102,
};
    HWControllerThread();
    status_t start();
    status_t stop();
    status_t readSysfs(const AString &path, AString *value);
    status_t writeSysfs(const AString &path, const AString &setValue, AString *retValue);
    status_t setCallback(sp<IHWMessengerCallback> back);
    status_t update();

protected:
    virtual ~HWControllerThread();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    sp<ALooper> mLooper;
    sp<IHWMessengerCallback>  curback;
};

}

#endif //HWMESSENGER_HWCONTROLLERTHREAD_H
