//
// Created by wangjf on 2/8/18.
//

#ifndef HWMESSENGER_HWUPDATOR_H
#define HWMESSENGER_HWUPDATOR_H
#include <media/stagefright/foundation/AHandler.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/ADebug.h>
#include <stdint.h>
#include <sys/types.h>
#include <utils/Log.h>
#include "HWMessenger.h"

namespace android {

class HWUpdator : public AHandler {
public:
enum {
    kWhatUpdate = 100,
};
    HWUpdator(HWMessenger *hwMessenger);
    void update(int32_t keyCode, int32_t keyValue, int32_t keyFlags);

protected:
    virtual ~HWUpdator();
    virtual void onMessageReceived(const sp<AMessage> &msg);
private:
    sp<ALooper> mLooper;
    sp<HWMessenger> mHWMessenger;
};

}

#endif //HWMESSENGER_IHWSTATEUPDATOR_H
