//author : Jianfeng
//date   : 2018/02/05
#include <binder/Parcel.h>
#include <utils/Log.h>
#include "IHWMessengerCallback.h"

namespace android {
enum {
        ON_KEY = IBinder::FIRST_CALL_TRANSACTION,
};
//IBinder::FLAG_ONEWAY asynchronous call
class BpHWMessengerCallback:public BpInterface<IHWMessengerCallback>
{
public:
        BpHWMessengerCallback(const sp<IBinder>&impl)
        :BpInterface<IHWMessengerCallback>(impl){}
        virtual void onKey(int keyCode, int value, int flags){
                Parcel data,reply ;
                data.writeInterfaceToken(IHWMessengerCallback::getInterfaceDescriptor());
                data.writeInt32(keyCode);
                data.writeInt32(value);
                data.writeInt32(flags);
                remote()->transact(ON_KEY, data, &reply, IBinder::FLAG_ONEWAY);
                int32_t err = reply.readExceptionCode();
                if(err<0){
                        ALOGD("onkey return %d\n",err);
                        return;
                }
                ALOGD("onkey Bp side called\n");
        }
};
IMPLEMENT_META_INTERFACE(HWMessengerCallback, "com.qiyi.hwmessenger.IHWMessengerCallback")

}
