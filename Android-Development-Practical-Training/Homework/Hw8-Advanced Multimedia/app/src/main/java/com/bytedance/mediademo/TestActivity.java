package com.bytedance.mediademo;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.FileProvider;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.View;
import android.widget.ImageView;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

public class TestActivity extends AppCompatActivity {
    File imageFile;
    Uri imagePath;
    String path;
    public static void startUI(Context context){
        Intent intent = new Intent(context,TestActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_test);
    }

    public void click(View view) {
//        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, 0);
        Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        path = getOutputMediaPath();
//        imagePath = FileProvider.getUriForFile(this, getPackageName()+".fileprovider", imageFile);
        cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, PathUtils.getUriForFile(this,path));
        //if (cameraIntent.resolveActivity(getPackageManager()) != null)
        startActivityForResult(cameraIntent, 11);
    }

    private File getOutputMedia(int i){//i=0,image;i=1,video;
        File outputFile=getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        File mediaFile;
        if(i==0) mediaFile = new File(outputFile,"IMG_"+timeStamp+".jpg");
        else mediaFile = new File(outputFile,"VID_"+timeStamp+".mp4");

        if(!mediaFile.exists()){
            mediaFile.getParentFile().mkdirs();
        }
        return mediaFile;
    }

    private String getOutputMediaPath() {
        File mediaStorageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        File mediaFile = new File(mediaStorageDir, "IMG_" + timeStamp + ".jpg");
        if (!mediaFile.exists()) {
            mediaFile.getParentFile().mkdirs();
        }
        return mediaFile.getAbsolutePath();
    }


    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if(requestCode==0&&grantResults[0] == PackageManager.PERMISSION_GRANTED){
            Intent cameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            imageFile =new File(getOutputMediaPath());
            imagePath = FileProvider.getUriForFile(this, getPackageName()+".fileprovider", imageFile);
            cameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, imagePath);
            //if (cameraIntent.resolveActivity(getPackageManager()) != null)
            startActivityForResult(cameraIntent, 11);
        }
    }
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==11&&resultCode==RESULT_OK){
            Bundle imageExtra=data.getExtras();
            Bitmap image=(Bitmap) imageExtra.get("data");
            Drawable drawable = new BitmapDrawable(image);
            ImageView iv = findViewById(R.id.iv_img);
            iv.setImageBitmap(image);
        }
    }
}