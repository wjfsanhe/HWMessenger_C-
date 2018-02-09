//
// Created by wangjf on 2/8/18.
//

#ifndef HWMESSENGER_HWMONITOR_H
#define HWMESSENGER_HWMONITOR_H
#include <utils/Thread.h>
#include "InputDevice.h"
namespace android {

class HWUpdator;

class HWMonitor : public Thread {
public:
    HWMonitor(const sp<HWUpdator>& hwUpdator);
    virtual ~HWMonitor(){};
protected:
    virtual bool threadLoop();
    virtual void onFirstRef();
private:
    sp<HWUpdator> mHWUpdator;
    sp<InputDevice> mInputDevice;

};
}
#endif //HWMESSENGER_HWMONITOR_H
