package com.hzy.gifshow;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;

import com.github.chrisbanes.photoview.PhotoView;
import com.hzy.libnsgif.IGifView;
import com.hzy.libnsgif.NsGifPlayer;

import java.io.IOException;
import java.io.InputStream;


/**
 * Created by huzongyao on 16-12-6.
 * https://github.com/huzongyao
 */
public class GifPhotoView extends PhotoView implements IGifView {

    private static final int MAX_ASSETS_LENGTH = 0x10000000;
    private NsGifPlayer mGifPlayer;


    public GifPhotoView(Context context) {
        this(context, null);
    }

    public GifPhotoView(Context context, AttributeSet attr) {
        this(context, attr, 0);
    }

    public GifPhotoView(Context context, AttributeSet attr, int defStyle) {
        super(context, attr, defStyle);
        initPlayer();
    }

    private void initPlayer() {
        mGifPlayer = new NsGifPlayer(this);
    }

    public void loadAssets(String assetsPath) {
        try {
            InputStream is = getContext().getAssets().open(assetsPath);
            if (is.available() <= MAX_ASSETS_LENGTH) {
                byte[] buffer = new byte[is.available()];
                is.read(buffer);
                mGifPlayer.setBuffer(buffer);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void pause() {
        mGifPlayer.pause();
    }

    public void resume() {
        mGifPlayer.resume();
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        mGifPlayer.start();
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mGifPlayer.stop();
    }

    @Override
    public void onLoadFinish(boolean isLoadOk, final Bitmap bitmap) {
        if (isLoadOk) {
            post(new Runnable() {
                @Override
                public void run() {
                    setImageBitmap(bitmap);
                }
            });
        }
    }

    @Override
    public void onRender() {
        postInvalidate();
    }
}
