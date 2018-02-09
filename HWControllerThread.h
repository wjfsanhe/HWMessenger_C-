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
#include "HWMessenger.h"

namespace android {

class HWControllerThread : public AHandler {
public:
enum {
    kWhatWriteSysfs = 'WRITE',
    kWhatReadSysfs = 'READ',
};
    HWControllerThread(sp<HWMessenger> hwMessenger);
    status_t start();
    status_t stop();
    status_t readSysfs(const AString &path, AString *value);
    status_t writeSysfs(const AString &path, const AString &setValue, AString *retValue);

protected:
    virtual ~HWControllerThread();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    sp<ALooper> mLooper;
};

}

#endif //HWMESSENGER_HWCONTROLLERTHREAD_H
