package com.hzy.libnsgif;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.AttributeSet;
import android.widget.ImageView;

import java.io.IOException;
import java.io.InputStream;

/**
 * Created by huzongyao on 16-12-5.
 * https://github.com/huzongyao
 */
public class NsGifView extends ImageView implements IGifView {

    private static final int MAX_ASSETS_LENGTH = 0x10000000;
    private NsGifPlayer mGifPlayer;
    private String mAssetName;

    public NsGifView(Context context) {
        this(context, null);
    }

    public NsGifView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public NsGifView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        mGifPlayer = new NsGifPlayer(this);
    }

    public void loadFile(String filePath) {
        mGifPlayer.setFilePath(filePath);
    }

    public void loadAssets(String assetsName) {
        try {
            mAssetName = assetsName;
            InputStream is = getContext().getAssets().open(assetsName);
            if (is.available() <= MAX_ASSETS_LENGTH) {
                byte[] buffer = new byte[is.available()];
                is.read(buffer);
                mGifPlayer.setBuffer(buffer);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
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

    public String getAssetName() {
        return mAssetName;
    }
}
