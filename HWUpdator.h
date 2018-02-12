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
#include <utils/String16.h>


namespace android {
class HWMessenger;
class HWUpdator : public AHandler {
public:
enum {
    kWhatUpdate = 100,
};
    HWUpdator(sp<HWMessenger> hwMessenger);
    void update(AString deviceName, int32_t keyCode, int32_t keyValue, int32_t keyFlags);

protected:
    virtual ~HWUpdator();
    virtual void onMessageReceived(const sp<AMessage> &msg);
    virtual void onFirstRef();
private:
    sp<ALooper> mLooper;
    sp<HWMessenger> mHWMessenger;
};

}

#endif //HWMESSENGER_IHWSTATEUPDATOR_H
