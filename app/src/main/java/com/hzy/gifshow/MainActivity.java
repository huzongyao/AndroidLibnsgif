package com.hzy.gifshow;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;

import com.hzy.libnsgif.NsGifView;

public class MainActivity extends AppCompatActivity {

    private NsGifView mGifView1;
    private NsGifView mGifView2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mGifView1 = (NsGifView) findViewById(R.id.demo_gif_view1);
        mGifView2 = (NsGifView) findViewById(R.id.demo_gif_view2);
        mGifView1.loadAssets("test1.gif");
        mGifView2.loadAssets("test2.gif");
        findViewById(R.id.button_another_demo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivity(new Intent(MainActivity.this, PhotoViewActivity.class));
            }
        });
    }
}
