package com.hzy.gifshow;

import android.os.Bundle;
import android.text.TextUtils;
import android.view.MenuItem;

import androidx.appcompat.app.ActionBar;
import androidx.appcompat.app.AppCompatActivity;

public class PhotoViewActivity extends AppCompatActivity {

    private GifPhotoView mGifView;
    private String mPhotoName;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_photo_view);
        mGifView = findViewById(R.id.demo_view_photo);
        mPhotoName = getIntent().getStringExtra("asset_name");
        if(!TextUtils.isEmpty(mPhotoName)) {
            mGifView.loadAssets(mPhotoName);
        }
        ActionBar actionBar =  getSupportActionBar();
        if(actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                finish();
                break;
        }
        return true;
    }
}
