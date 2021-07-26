package com.bytedance.mediademo;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.core.content.FileProvider;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class SystemCameraActivity extends AppCompatActivity {
    private int REQUEST_CODE_TAKE_PHOTO = 1001;
    private int REQUEST_CODE_TAKE_PHOTO_PATH = 1002;
    private int PERMISSION_REQUEST_CAMERA_CODE = 1003;
    private int PERMISSION_REQUEST_CAMERA_PATH_CODE = 1004;

    private ImageView imageView;
    private String takeImagePath;

    public static void startUI(Context context) {
        Intent intent = new Intent(context, SystemCameraActivity.class);
        context.startActivity(intent);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_system_camera);
        imageView = findViewById(R.id.iv_img);
    }

    public void takePhoto(View view) {
        requestCameraPermission();
    }

    public void takePhotoUsePath(View view) {
        requestCameraAndSDCardPermission();
    }

    private void requestCameraAndSDCardPermission() {
        boolean hasCameraPermission = ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED;
        if (hasCameraPermission) {
            takePhotoUsePathHasPermission();
        } else {
            String[] permissions = new String[]{Manifest.permission.CAMERA};
            ActivityCompat.requestPermissions(this, permissions, PERMISSION_REQUEST_CAMERA_PATH_CODE);
        }
    }

    private void takePhotoUsePathHasPermission() {
        Intent intent=new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        takeImagePath=getOutputMediaPath();
        intent.putExtra(MediaStore.EXTRA_OUTPUT, PathUtils.getUriForFile(this,takeImagePath));
        startActivityForResult(intent, REQUEST_CODE_TAKE_PHOTO_PATH);
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

    private void requestCameraPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            String[] permissions = new String[]{Manifest.permission.CAMERA};
            ActivityCompat.requestPermissions(this, permissions, PERMISSION_REQUEST_CAMERA_CODE);
        } else {
            takePhotoHasPermission();
        }
    }

    private void takePhotoHasPermission() {
        Intent intent=new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        startActivityForResult(intent,REQUEST_CODE_TAKE_PHOTO);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == PERMISSION_REQUEST_CAMERA_CODE) {
            if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                takePhotoHasPermission();
            } else {
                Toast.makeText(this, "权限获取失败", Toast.LENGTH_SHORT).show();
            }
        } else if (requestCode == PERMISSION_REQUEST_CAMERA_PATH_CODE) {
            boolean hasPermission = true;
            for (int grantResult : grantResults) {
                if (grantResult != PackageManager.PERMISSION_GRANTED) {
                    hasPermission = false;
                    break;
                }
            }
            if (hasPermission) {
                takePhotoUsePathHasPermission();
            } else {
                Toast.makeText(this, "权限获取失败", Toast.LENGTH_SHORT).show();
            }
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST_CODE_TAKE_PHOTO && resultCode == RESULT_OK) {
            Bundle extras=data.getExtras();
            Bitmap bitmap=(Bitmap)extras.get("data");
            imageView.setImageBitmap(bitmap);
        } else if (requestCode == REQUEST_CODE_TAKE_PHOTO_PATH && resultCode == RESULT_OK) {
            int targetWidth=imageView.getWidth();
            int targetHeight=imageView.getHeight();
            BitmapFactory.Options options=new BitmapFactory.Options();
            options.inJustDecodeBounds=true;
            BitmapFactory.decodeFile(takeImagePath,options);
            int photoWidth=options.outWidth;
            int photoHeight=options.outHeight;
            int scaleFactor=Math.min(photoWidth/targetWidth,photoHeight/targetHeight);
            options.inJustDecodeBounds=false;
            options.inSampleSize=scaleFactor;
            Bitmap bitmap=BitmapFactory.decodeFile(takeImagePath,options);
            Bitmap rotatedBitmap=PathUtils.rotateImage(bitmap, takeImagePath);
            imageView.setImageBitmap(rotatedBitmap);
        }
    }


}