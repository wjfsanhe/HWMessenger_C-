//
// Created by wangjf on 2/8/18.
//

#include "HWUpdator.h"


namespace android {

HWUpdator::HWUpdator(HWMessenger *hwMessenger)
          : mHWMessenger(hwMessenger)
{
    mLooper = new ALooper();
    mLooper->setName("HWUpdator");
    mLooper->registerHandler(this);
    mLooper->start(false,false,ANDROID_PRIORITY_AUDIO);
}

HWUpdator::~HWUpdator()
{
    if (!mLooper.get()){
        return ;
    }
    mLooper->unregisterHandler(id());
    mLooper->stop();
    mLooper.clear();
}
void HWUpdator::update(int32_t keyCode, int32_t keyValue, int32_t keyFlags)
{
    sp<AMessage> msg = new AMessage(kWhatUpdate, this);
    msg->setInt32("keyCode", keyCode);
    msg->setInt32("keyValue", keyValue);
    msg->setInt32("keyFlags", keyFlags);
    msg->post();
}
void HWUpdator::onMessageReceived(const sp<AMessage> &msg)
{

    switch (msg->what()) {
        case kWhatUpdate:
        {
            int32_t keyCode, keyValue, keyFlags;
            CHECK(msg->findInt32("keyCode", &keyCode));
            CHECK(msg->findInt32("keyValue", &keyValue));
            CHECK(msg->findInt32("keyFlags", &keyFlags));
            mHWMessenger->updateKey(keyCode, keyValue, keyFlags);
            break;
        }
        default:
            TRESPASS();
    }
}
}
