//
// Created by wangjf on 2/9/18.
//
#define LOG_TAG "HWMessenger"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>
#include "InputDevice.h"
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
    int  fd;
    for(int32_t i = 0; i < MAX_INPUT_DEVICE_COUNT; i++) {
        sprintf(name, "%s%d", BASE_PATH,i);
        if ((fd =open(name, O_RDWR | O_CLOEXEC)) < 0) {
            ALOGE("Could not open device %s[%lu]\n" ,name,strlen(name));
            break ;
        }
        ALOGD("Open device %s success\n", name);
        //insert one device fd .
        char *devName = (char*)malloc(strlen(name));
        if ( devName == NULL) {
            ALOGD("Low memory !!!");
            break ;
        }
        strncpy(devName, name, strlen(name));
        setDeviceNonBlock(fd);
        mfdMap.insert(std::make_pair(fd,devName));
    }
    return 0;
    //initialize epoll
}
void InputDevice::init()
{
    prepareInputDevice();
    prepareEpollDevice();
}
void InputDevice::monitor()
{
    struct epoll_event events[mfdMap.size()];
    //timeout 2000ms
    int n = epoll_wait(mEpollFD, events, mfdMap.size(), 2000);
}

}
