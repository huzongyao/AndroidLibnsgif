package com.hzy.libnsgif;

import android.graphics.Bitmap;

/**
 * Created by huzongyao on 16-12-5.
 * https://github.com/huzongyao
 */
public interface IGifView {
    void onLoadFinish(boolean isLoadOk, Bitmap bitmap);

    void onRender();
}
