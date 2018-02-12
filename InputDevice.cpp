//
// Created by wangjf on 2/9/18.
//
#define LOG_TAG "HWMessenger"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <sys/epoll.h>
#include "InputDevice.h"
#include "HWUpdator.h"
#include <cutils/log.h>

namespace android {
#define BASE_PATH "/dev/input/event"
#define MAX_INPUT_DEVICE_COUNT 100

int32_t epollRegister(int32_t eFd, int32_t fd)
{
    struct epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = fd;

    int32_t ret = -1;
    int32_t count = 0;
    do {
        ret = epoll_ctl(eFd, EPOLL_CTL_ADD, fd, &ev);
        count ++;
        //EINTR op interrupt by signal (io, exception etc.)
        //retry 3 times.
    } while(ret < 0 && errno == EINTR && count < 4);
    return ret;
}
int32_t epollUnregister(int32_t eFd, int32_t fd)
{
    int32_t ret = -1;
    int32_t count = 0;
    struct epoll_event ev;

    do {
        ret = epoll_ctl(eFd, EPOLL_CTL_DEL, fd, &ev);
        count ++;
        //EINTR op interrupt by signal (io, exception etc.)
        //retry 3 times.
    } while(ret < 0 && errno == EINTR && count < 4);
    return ret;
}

InputDevice::InputDevice(const sp<HWUpdator> &hwUpdator )
    :mUpdator(hwUpdator)
{
    mInitOK = false;
}
InputDevice::~InputDevice(){
    std::map<int32_t, char*>::iterator it;

    for (it = mfdMap.begin(); it != mfdMap.end(); ++it) {
        epollUnregister(mEpollFD, it->first);
        close(it->first);
        if (it->second) free(it->second);

    }
    mfdMap.clear();
}
int32_t setDeviceNonBlock(int32_t fd) {
    int32_t opts;
    opts = fcntl(fd, F_GETFL);
    if (opts < 0) {
        ALOGE("Get block option error :(%s) .", strerror(errno));
        return -1;
    }
    opts |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, opts) < 0) {
        ALOGE("Set block option error :(%s) .", strerror(errno));
        return -1;
    }
    return 0;
}

int32_t InputDevice::prepareEpollDevice()
{
    mEpollFD = epoll_create(mfdMap.size());
    if (mEpollFD < 0) {
        ALOGE("Create epoll failed.");
        return -1;
    }
    std::map<int32_t, char*>::iterator it;
    for (it = mfdMap.begin(); it != mfdMap.end(); ++it) {
        if (epollRegister(mEpollFD, it->first) < 0) {
            ALOGE("register epoll failed.");
        }
    }
    return 0;
}
int32_t InputDevice::prepareInputDevice()
{
    char name[80];
    int32_t  fd;
    for(int32_t i = 0; i < MAX_INPUT_DEVICE_COUNT; i++) {
        sprintf(name, "%s%d", BASE_PATH,i);
        if ((fd =open(name, O_RDWR | O_CLOEXEC)) < 0) {
            ALOGE("Could not open device %s[%lu]\n" ,name,strlen(name));
            break ;
        }
        ALOGD("Open device %s success\n", name);
        char extName[80];
        if (ioctl(fd, EVIOCGNAME(sizeof(extName) - 1), &extName) < 1) {
            extName[0] = '\0';
        }
        //insert one device fd .
        char *devName = (char*)malloc(strlen(name) + strlen(extName) + 1);//1 used for special char.
        if ( devName == NULL) {
            ALOGD("Low memory !!!");
            break ;
        }
        strncpy(devName, name, strlen(name));
        strcat(devName,"#");
        strcat(devName, extName);
        setDeviceNonBlock(fd);
        mfdMap.insert(std::make_pair(fd,devName));
    }
    return 0;
    //initialize epoll
}
void InputDevice::init()
{
    if ( false == mInitOK) {
        prepareInputDevice();
        prepareEpollDevice();
    }
    mInitOK = true;
}
/*
37  #define EV_SYN			0x00
38  #define EV_KEY			0x01
39  #define EV_REL			0x02
40  #define EV_ABS			0x03
41  #define EV_MSC			0x04
42  #define EV_SW			0x05
43  #define EV_LED			0x11
44  #define EV_SND			0x12
45  #define EV_REP			0x14
46  #define EV_FF			0x15
47  #define EV_PWR			0x16
48  #define EV_FF_STATUS    0x17
49  #define EV_MAX			0x1f
50  #define EV_CNT			(EV_MAX+1)
*/
void InputDevice::monitor()
{
    struct epoll_event events[mfdMap.size()];
    //timeout 2000ms
    //ALOGE("InputDevice epoll monitor");
    int32_t n = epoll_wait(mEpollFD, events, mfdMap.size(), 2000);
    if (n == -1) {
        ALOGE("Error epoll wait");
        return;
    }
    for (int32_t i = 0; i < n; i++){
        int32_t res;
        struct input_event iev;
        //if it is valid fd.
        if (mfdMap.count(events[i].data.fd) > 0) {
            res = read(events[i].data.fd, &iev, sizeof(iev));
            if (res < (int32_t)sizeof(iev)) {
                ALOGE("Error get input event");
                continue;
            }
            const char *type;
            switch (iev.type){
                case EV_SYN:
                type = "SYNC";
                break;
                case EV_KEY:
                type = "KEY";
                break;
                case EV_REL:
                type = "REL";
                break;
                case EV_ABS:
                type = "ABS";
                break;
                default:
                type = "UNKNOWN";
                break;
            }
            ALOGD("%s:  code[%04x] value[%08x]", type, iev.code, iev.value);
            mUpdator->update(AString(mfdMap.find(events[i].data.fd)->second), iev.code, iev.value, 0);
        }
    }
}

}
