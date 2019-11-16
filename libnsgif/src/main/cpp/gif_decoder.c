/*
 ============================================================================
 Name        : gif_decoder.c
 Author      : huzongyao
 Version     : 1.3
 Copyright   : https://github.com/huzongyao
 Description : these code is called by java and used to decode gif file
 ============================================================================
 */
#include <string.h>
#include "libnsgif.h"
#include "jnihelper.h"

#define FUNC(f) Java_com_hzy_libnsgif_GifDecoder_##f

void *bitmap_create(int width, int height) {
    return calloc((size_t) (width * height), 4);
}

void bitmap_set_opaque(void *bitmap, bool opaque) {
    (void) opaque; /* unused */
}

bool bitmap_test_opaque(void *bitmap) {
    return false;
}

unsigned char *bitmap_get_buffer(void *bitmap) {
    return bitmap;
}

void bitmap_destroy(void *bitmap) {
    free(bitmap);
}

void bitmap_modified(void *bitmap) {
    return;
}

gif_bitmap_callback_vt bitmap_callbacks = {bitmap_create, bitmap_destroy, bitmap_get_buffer,
                                           bitmap_set_opaque, bitmap_test_opaque, bitmap_modified};

void showError(const char *context, gif_result code) {
    switch (code) {
        case GIF_INSUFFICIENT_FRAME_DATA:
            LOGE("%s failed: GIF_INSUFFICIENT_FRAME_DATA", context);
            break;
        case GIF_FRAME_DATA_ERROR:
            LOGE("%s failed: GIF_FRAME_DATA_ERROR", context);
            break;
        case GIF_INSUFFICIENT_DATA:
            LOGE("%s failed: GIF_INSUFFICIENT_DATA", context);
            break;
        case GIF_DATA_ERROR:
            LOGE("%s failed: GIF_DATA_ERROR", context);
            break;
        case GIF_INSUFFICIENT_MEMORY:
            LOGE("%s failed: GIF_INSUFFICIENT_MEMORY", context);
            break;
        default:
            LOGE("%s failed: unknown code %i", context, code);
            break;
    }
}

int getFrame(gif_animation *gif, int index) {
    gif_result code;
    if (gif == NULL) {
        LOGE("gif is null");
        return -1;
    }
    if (index < 0 || index >= gif->frame_count) {
        LOGE("mCurIndex error");
        return -1;
    }
    code = gif_decode_frame(gif, index);
    if (code != GIF_OK) {
        showError("gif_decode_frame", code);
        return -1;
    }
    return gif->frames[index].frame_delay;
}

/**
 * load gif file to buffer
 */
unsigned char *loadFile(const char *path, size_t *pFileLength) {
    FILE *fd;
    struct stat sb;
    size_t size;
    unsigned char *data = NULL;
    size_t n;

    fd = fopen(path, "rbe");
    if (!fd) {
        LOGE("File open error[%s]", path);
        return NULL;
    }
    if (stat(path, &sb)) {
        fclose(fd);
        LOGE("File size error[%s]", path);
        return NULL;
    }
    size = (size_t) sb.st_size;
    data = malloc(size);
    if (data == NULL) {
        LOGE("Unable to allocate [%lld] bytes", (long long) size);
        fclose(fd);
        return NULL;
    }
    n = fread(data, 1, size, fd);
    fclose(fd);
    if (n != size) {
        LOGE("Read to buffer from [%s] error", path);
        free(data);
        return NULL;
    }
    *pFileLength = size;
    return data;
}

/**
 * init the decoder and scan get the gif information
 */
int initDecoder(const char *filePath, unsigned char *buffer, size_t bufferLength, jlong *params) {
    gif_result code;
    //malloc gif space
    gif_animation *gif = (gif_animation *) malloc(sizeof(gif_animation));
    if (gif == NULL) {
        LOGE("Unable to allocate gif_animation");
        return -1;
    }
    /* create our gif animation */
    gif_create(gif, &bitmap_callbacks);
    if (buffer == NULL) {
        /* load file into memory */
        buffer = loadFile(filePath, &bufferLength);
    }
    if (buffer == NULL) {
        free(gif);
        LOGE("gif_data read failed");
        return -1;
    }
    /* begin decoding */
    do {
        code = gif_initialise(gif, bufferLength, buffer);
        if (code != GIF_OK && code != GIF_WORKING) {
            showError("gif_initialise", code);
            return -1;
        }
    } while (code != GIF_OK);
    params[0] = gif->frame_count;
    params[1] = gif->width;
    params[2] = gif->height;
    params[3] = (jlong) gif;
    LOGI("Init Decoder ok! [0x%llx]", params[3]);
    return 0;
}


JNIEXPORT jint JNICALL
FUNC(nInitByPath)(JNIEnv *env, jclass thiz, jstring filePath, jlongArray params) {
    int ret = 0;
    char *cfilePath = (char *) (*env)->GetStringUTFChars(env, filePath, NULL);
    jlong *cParams = (*env)->GetLongArrayElements(env, params, NULL);
    ret = initDecoder(cfilePath, NULL, 0, cParams);
    (*env)->ReleaseLongArrayElements(env, params, cParams, 0);
    (*env)->ReleaseStringUTFChars(env, filePath, cfilePath);
    return ret;
}

JNIEXPORT jint JNICALL
FUNC(nInitByBytes)(JNIEnv *env, jclass thiz, jbyteArray buffer, jlongArray params) {
    int ret = 0;
    jsize bufferLength = ((*env)->GetArrayLength(env, buffer));
    jlong *cParams = (*env)->GetLongArrayElements(env, params, NULL);
    jbyte *cBuffer = malloc((size_t) bufferLength);
    (*env)->GetByteArrayRegion(env, buffer, 0, bufferLength, cBuffer);
    ret = initDecoder(NULL, (unsigned char *) cBuffer, (size_t) bufferLength, cParams);
    (*env)->ReleaseLongArrayElements(env, params, cParams, 0);
    return ret;
}

JNIEXPORT jint JNICALL
FUNC(nGetFrameBitmap)(JNIEnv *env, jclass thiz, jint index, jobject jbitmap, jlong handler) {
    int ret = 0;
    int delay = 0;
    void *bmpAddr = NULL;
    gif_animation *gif = NULL;
    size_t buf_size = 0;

    gif = (gif_animation *) handler;
    if (gif == NULL) {
        LOGE("gif is null");
        return -1;
    }
    buf_size = gif->width * gif->height * 4;
    delay = getFrame(gif, index);
    if ((ret = AndroidBitmap_lockPixels(env, jbitmap, &bmpAddr)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return ret;
    }
    memcpy(bmpAddr, gif->frame_image, buf_size);
    AndroidBitmap_unlockPixels(env, jbitmap);
    return delay * 10;
}

JNIEXPORT jint JNICALL
FUNC(nDestory)(JNIEnv *env, jclass thiz, jlong handler) {
    gif_animation *gif = (gif_animation *) handler;
    if (gif == NULL) {
        LOGE("gif is null");
        return -1;
    } else {
        free(gif->gif_data);
        gif_finalise(gif);
        free(gif);
    }
    return 0;
}

