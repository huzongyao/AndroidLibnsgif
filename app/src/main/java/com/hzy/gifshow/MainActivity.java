package com.hzy.gifshow;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.hzy.libnsgif.NsGifView;

public class MainActivity extends AppCompatActivity {

    private NsGifView mGifView1;
    private NsGifView mGifView2;
    private NsGifView mGifView3;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mGifView1 = findViewById(R.id.demo_gif_view1);
        mGifView2 = findViewById(R.id.demo_gif_view2);
        mGifView3 = findViewById(R.id.demo_gif_view3);
        mGifView1.loadAssets("test1.gif");
        mGifView2.loadAssets("test2.gif");
        mGifView3.loadAssets("test3.gif");

        mGifView1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startPhotoView("test1.gif");
            }
        });
        mGifView2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startPhotoView("test2.gif");
            }
        });
        mGifView3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startPhotoView("test3.gif");
            }
        });
    }

    private void startPhotoView(String assetName) {
        Intent intent = new Intent(this, PhotoViewActivity.class);
        intent.putExtra("asset_name", assetName);
        startActivity(intent);
    }
}
