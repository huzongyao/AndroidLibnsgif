package com.hzy.gifshow;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

public class PhotoViewActivity extends AppCompatActivity {

    private GifPhotoView mGifView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_photo_view);
        mGifView = (GifPhotoView) findViewById(R.id.demo_view_photo);
        mGifView.loadAssets("test1.gif");
    }
}
