# Copyright (C) 2015 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_CLANG := true



########################################################################
# build input flinger executable


LOCAL_SRC_FILES:= \
	main.cpp \
	HWMessenger.cpp \
	IHWMessenger.cpp \
	IHWMessengerCallback.cpp \
	IHWControllerClient.cpp \
	HWUpdator.cpp \
	InputDevice.cpp \
	HWMonitor.cpp \
	HWControllerThread.cpp


LOCAL_SHARED_LIBRARIES := \
	libbinder \
	libutils \
    libcrypto \
    libcutils \
    liblog \
    libhardware \
	libstagefright_foundation

LOCAL_MODULE := hwmessenger
LOCAL_INIT_RC := hwmessenger.rc
LOCAL_CFLAGS += -Wno-unused-parameter

include $(BUILD_EXECUTABLE)
