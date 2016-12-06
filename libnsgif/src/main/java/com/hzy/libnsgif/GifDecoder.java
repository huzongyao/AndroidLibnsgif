package com.hzy.libnsgif;

import android.graphics.Bitmap;

/**
 * Created by huzongyao on 16-12-5.
 * https://github.com/huzongyao
 */
public class GifDecoder {

    private int mFrameCount;
    private int mImageWidth;
    private int mImageHeight;
    private int mCurIndex = 0;
    private Bitmap mBitmap = null;
    private int mCurDelay = 0;
    private int mNativeHandler = 0;

    public static final int DEFAULT_DELAY = 100;

    public boolean load(String path) {
        recycle();
        int[] params = new int[4];
        if (nInitByPath(path, params) == 0) {
            mFrameCount = params[0];
            mImageWidth = params[1];
            mImageHeight = params[2];
            mNativeHandler = params[3];
            mBitmap = Bitmap.createBitmap(mImageWidth, mImageHeight, Bitmap.Config.ARGB_8888);
            return true;
        } else {
            throw new RuntimeException("Gif file decode error");
        }
    }

    public boolean load(byte[] buffer) {
        recycle();
        int[] params = new int[4];
        if (nInitByBytes(buffer, params) == 0) {
            mFrameCount = params[0];
            mImageWidth = params[1];
            mImageHeight = params[2];
            mNativeHandler = params[3];
            mBitmap = Bitmap.createBitmap(mImageWidth, mImageHeight, Bitmap.Config.ARGB_8888);
            return true;
        } else {
            throw new RuntimeException("Gif file decode error");
        }
    }

    public void recycle() {
        if (mNativeHandler != 0 && this.nDestory(this.mNativeHandler) != 0) {
            throw new RuntimeException("native destory failed");
        }
        if (mBitmap != null && !mBitmap.isRecycled()) {
            mBitmap.recycle();
        }
        mNativeHandler = 0;
        mCurIndex = 0;
        mCurDelay = 0;
    }

    public Bitmap getBitmap() {
        return this.mBitmap;
    }


    public void setCurIndex(int curFrame) {
        this.mCurIndex = curFrame;
    }

    public int getCurDelay() {
        return mCurDelay;
    }

    public int getFrameCount() {
        return this.mFrameCount;
    }

    public int getWidth() {
        return this.mImageWidth;
    }

    public int getHeight() {
        return this.mImageHeight;
    }

    public int decodeFirstFrame() {
        return decodeFrame(0);
    }

    public int decodeCurrentFrame() {
        return decodeFrame(mCurIndex);
    }

    public int decodeNextFrame() {
        return decodeFrame((mCurIndex + 1) % mFrameCount);
    }

    public int decodeFrame(int index) {
        if (index < 0 || index >= this.mFrameCount) {
            throw new ArrayIndexOutOfBoundsException(index);
        }
        if (mBitmap != null) {
            int delay = nGetFrameBitmap(index, mBitmap, this.mNativeHandler);
            if (delay > 0) {
                this.mCurDelay = delay;
            } else {
                this.mCurDelay = DEFAULT_DELAY;
            }
            this.mCurIndex = index;
            return delay;
        } else {
            throw new NullPointerException("Bitmap is null");
        }
    }

    @Override
    protected void finalize() throws Throwable {
        this.recycle();
        super.finalize();
    }

    /**
     * init the params, get the frame count, width,height
     *
     * @param path   gif image file path
     * @param params returned gif parameters
     * @return error code ,0 if no error
     */
    private native int nInitByPath(String path, int[] params);

    /**
     * init the params, get the frame count, width,height
     *
     * @param buffer
     * @param params
     * @return
     */
    private native int nInitByBytes(byte[] buffer, int[] params);

    /**
     * write image data to bitmap
     *
     * @param index frame mCurIndex
     * @param bmp   target bitmap
     * @return frame delay time(ms) if <= 0 means failed
     */
    private native int nGetFrameBitmap(int index, Object bmp, int handler);

    /**
     * destory the native resources
     *
     * @return
     */
    private native int nDestory(int handler);

    /**
     * load the native library
     */
    static {
        System.loadLibrary("nsgif");
    }
}
