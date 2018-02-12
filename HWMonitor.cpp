//
// Created by wangjf on 2/9/18.
//
#include "HWMonitor.h"
#include "HWUpdator.h"
#include "InputDevice.h"

namespace android {

HWMonitor::HWMonitor(const sp<HWUpdator> &hwUpdator)
          : mHWUpdator(hwUpdator)
{
}
void HWMonitor::onFirstRef()
{
    mInputDevice = new InputDevice(mHWUpdator);
    run("HWMonitor",ANDROID_PRIORITY_DISPLAY);
}
bool HWMonitor::threadLoop(){

    //initialize read op.
    mInputDevice->init();
    while (true) {
        mInputDevice->monitor();
    }
    return false;
}
}