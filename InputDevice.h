//
// Created by wangjf on 2/9/18.
//

#ifndef HWMESSENGER_INPUTDEVICE_H
#define HWMESSENGER_INPUTDEVICE_H
#include <map>
#include <utils/RefBase.h>
#include <utils/String8.h>


namespace android {
class HWUpdator;
class InputDevice : public RefBase {
public:
    InputDevice(const sp<HWUpdator> &hwUpdator );
    void init();
    void monitor();
private:
    int32_t prepareInputDevice();
    int32_t prepareEpollDevice();
protected:
    ~InputDevice();
private:
    int32_t mEpollFD;
    bool mInitOK;
    std::map<int, char*> mfdMap;
    sp<HWUpdator> mUpdator;

};

}
#endif //HWMESSENGER_INPUTDEVICE_H
