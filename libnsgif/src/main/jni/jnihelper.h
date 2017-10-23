//
// Created by huzongyao on 17-7-14.
//

#ifndef ANDLIBNSGIF_JNIHELPER_H
#define ANDLIBNSGIF_JNIHELPER_H

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <android/bitmap.h>

#ifdef NATIVE_LOG

#include <android/log.h>

#define LOG_TAG "NATIVE.LOG"

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG,__VA_ARGS__)
#else
#define LOGD(...) do{}while(0)
#define LOGI(...) do{}while(0)
#define LOGW(...) do{}while(0)
#define LOGE(...) do{}while(0)
#define LOGF(...) do{}while(0)
#endif

#endif //ANDLIBNSGIF_JNIHELPER_H
