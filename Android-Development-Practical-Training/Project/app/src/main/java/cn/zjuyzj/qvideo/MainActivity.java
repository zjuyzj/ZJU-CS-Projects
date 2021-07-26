package cn.zjuyzj.qvideo;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentPagerAdapter;
import androidx.viewpager.widget.ViewPager;

import androidx.appcompat.widget.Toolbar;

import android.Manifest;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.media.MediaMetadataRetriever;
import android.net.Uri;
import android.os.Bundle;

import android.provider.MediaStore;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.EditText;
import android.widget.SearchView;
import android.widget.Toast;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.tabs.TabLayout;

import java.io.File;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.List;

import cn.zjuyzj.qvideo.container.InfoFragment;
import cn.zjuyzj.qvideo.model.VideoInfo;
import cn.zjuyzj.qvideo.util.NetworkUtility;
import cn.zjuyzj.qvideo.util.StorageUtility;
import cn.zjuyzj.qvideo.var.variables;

public class MainActivity extends AppCompatActivity {
    private final static int REQUEST_CODE_PERMISSION_CAMERA=1;
    private final static int REQUEST_CODE_RECORD = 1;
    private final static int REQUEST_CODE_PICK_VIDEO = 2;
    private final static int REQUEST_CODE_TAKE_PICTURE = 3;
    private final static int REQUEST_CODE_PICK_PICTURE = 4;
    private boolean[] settingLocal={false,true,true,true,false};
    String videoPath,coverPath;
    NetworkUtility netUtil;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        StorageUtility.requestExternalStoragePermission(MainActivity.this);
        netUtil=new NetworkUtility();
        netUtil.initNetwork(new NetworkUtility.NetCallback() {
            @Override
            public void onRequest(List<VideoInfo> videos) { return; }

            @Override
            public void onRefresh() { return; }

            @Override
            public void onError(String reason) {
                Toast.makeText(MainActivity.this,reason,Toast.LENGTH_SHORT).show();
                return;
            }

            @Override
            public void onSuccess() {
                Toast.makeText(MainActivity.this,"视频上传成功",Toast.LENGTH_SHORT).show();
                ViewPager viewpager=findViewById(R.id.pager_info);
                viewpager.getAdapter().notifyDataSetChanged();
                return;
            }
        });
        FloatingActionButton upload=findViewById(R.id.fab_upload);
        upload.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                final String[] items={"使用相机录制","从本地视频中选取"};
                AlertDialog.Builder builder;
                builder=new AlertDialog.Builder(v.getContext())
                        .setIcon(R.drawable.ic_baseline_video_library_24)
                        .setTitle("请选择视频来源：")
                        .setItems(items, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                switch(which){
                                    case 0:
                                        requestCamaraPermission();
                                        break;
                                    case 1:
                                        Intent intent=new Intent(Intent.ACTION_PICK, MediaStore.Video.Media.EXTERNAL_CONTENT_URI);
                                        startActivityForResult(intent,REQUEST_CODE_PICK_VIDEO);
                                        break;
                                }
                            }
                        });
                builder.show();
            }
        });
        setSupportActionBar(findViewById(R.id.toolbar_func));
        getSupportActionBar().setDisplayShowTitleEnabled(false);
        Toolbar toolbar=findViewById(R.id.toolbar_func);
        toolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder builder;
                View view= LayoutInflater.from(MainActivity.this).inflate(R.layout.dialog_account, null);
                EditText inputID=view.findViewById(R.id.input_id);
                EditText inputName=view.findViewById(R.id.input_name);
                inputID.setText(variables.myID);
                inputName.setText(variables.myName);
                builder=new AlertDialog.Builder(MainActivity.this)
                        .setView(view)
                        .setTitle("修改个人信息")
                        .setIcon(R.drawable.ic_baseline_account_circle_24)
                        .setPositiveButton("保存", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                if(inputID.getText().toString().isEmpty()||inputName.getText().toString().isEmpty()){
                                    Toast.makeText(MainActivity.this, "请填写学号和昵称", Toast.LENGTH_SHORT).show();
                                }else{
                                    variables.myID=inputID.getText().toString();
                                    variables.myName=inputName.getText().toString();
                                    StorageUtility.storeSPConfig(MainActivity.this);
                                    ViewPager viewpager=findViewById(R.id.pager_info);
                                    viewpager.getAdapter().notifyDataSetChanged();
                                }
                            }
                        })
                        .setNegativeButton("取消",new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                return;
                            }
                        });
                builder.create().show();
            }
        });
        SearchView searchView=findViewById(R.id.search_uname);
        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String s) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String s) {
                variables.searchKey=s;
                ViewPager viewpager=findViewById(R.id.pager_info);
                viewpager.getAdapter().notifyDataSetChanged();
                return true;
            }
        });
        ViewPager infoPager=findViewById(R.id.pager_info);
        TabLayout idTab=findViewById(R.id.tab_id);
        infoPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public int getCount() {
                return variables.PAGE_COUNT;
            }

            @Override
            public Fragment getItem(int position) {
                Fragment fragment=null;
                switch(position){
                    case 0:
                        fragment= InfoFragment.newInstance(false);
                        break;
                    case 1:
                        fragment=InfoFragment.newInstance(true);
                        break;
                }
                return fragment;
            }

            @Override
            public int getItemPosition(Object object) {
                //When adapter.notifyDataSetChanged() executed,
                //Force to update(destroy-create) all pages in ViewPager
                return POSITION_NONE;
            }

            @Override
            public CharSequence getPageTitle(int position){
                String title="";
                switch (position){
                    case 0:
                        title="全部视频";
                        break;
                    case 1:
                        title="我的视频";
                        break;
                }
                return title;
            }
        });
        idTab.setupWithViewPager(infoPager);
    }

    private void requestCamaraPermission() {
        boolean hasCameraPermission = ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED;
        boolean hasAudioPermission = ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED;
        if (!hasCameraPermission||!hasAudioPermission) {
            List<String> permission = new ArrayList<String>();
            if (!hasCameraPermission) {
                permission.add(Manifest.permission.CAMERA);
            }
            if (!hasAudioPermission) {
                permission.add(Manifest.permission.RECORD_AUDIO);
            }
            ActivityCompat.requestPermissions(MainActivity.this, permission.toArray(new String[permission.size()]), REQUEST_CODE_PERMISSION_CAMERA);
        }
    }

    public String saveFirstFrame(Bitmap firstFrame) {
        String path=StorageUtility.getOutputMediaPath(false);
        File file;
        try {
            file=new File(path);
            if (!file.exists()) file.createNewFile();
            FileOutputStream fos=new FileOutputStream(file);
            firstFrame.compress(Bitmap.CompressFormat.JPEG, 100, fos);
            fos.flush();
            fos.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return path;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.toolbar, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.refresh_global:
                ViewPager viewpager=findViewById(R.id.pager_info);
                viewpager.getAdapter().notifyDataSetChanged();
                break;
            case R.id.setting_global:
                final String[] items={"长按封面下载视频","视频内双击点赞","视频内下滑退出","视频循环播放","录制10s短视频"};
                for(int i=0;i<variables.settingGlobal.length;i++)
                    settingLocal[i]=variables.settingGlobal[i];
                AlertDialog.Builder builder;
                builder=new AlertDialog.Builder(this)
                        .setIcon(R.drawable.ic_baseline_settings_24)
                        .setTitle("全局设置")
                        .setMultiChoiceItems(items, settingLocal, new DialogInterface.OnMultiChoiceClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                                if(isChecked) settingLocal[which]=true;
                                else settingLocal[which]=false;
                                return;
                            }
                        })
                        .setPositiveButton("保存", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                for(int i=0;i<variables.settingGlobal.length;i++)
                                    variables.settingGlobal[i]=settingLocal[i];
                                StorageUtility.storeSPConfig(MainActivity.this);
                                return;
                            }
                        })
                        .setNegativeButton("取消", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                return;
                            }
                        });
                builder.create().show();
                break;
        }
        return true;
    }

    private void refreshGallery(String path){
        Intent intent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE,Uri.fromFile(new File(path)));
        sendBroadcast(intent);
    }

    private void chooseCover(Context ctx){
        final String[] items={"使用相机拍摄","从本地图片中选取","使用视频首帧"};
        AlertDialog.Builder builder;
        builder=new AlertDialog.Builder(ctx)
                .setIcon(R.drawable.ic_baseline_photo_library_24)
                .setTitle("请选择封面来源：")
                .setItems(items, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        Intent intent;
                        switch(which){
                            case 0:
                                intent=new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
                                coverPath=StorageUtility.getOutputMediaPath(false);
                                intent.putExtra(MediaStore.EXTRA_OUTPUT, StorageUtility.getUriForFile(MainActivity.this,coverPath));
                                startActivityForResult(intent, REQUEST_CODE_TAKE_PICTURE);
                                break;
                            case 1:
                                intent = new Intent(Intent.ACTION_PICK);
                                intent.setType("image/*");
                                startActivityForResult(intent,REQUEST_CODE_PICK_PICTURE);
                                break;
                            case 2:
                                Bitmap firstFrame=null;
                                MediaMetadataRetriever retriever = new MediaMetadataRetriever();
                                try {
                                    retriever.setDataSource(videoPath);
                                    firstFrame=retriever.getFrameAtTime();
                                } catch (IllegalArgumentException e) {
                                    e.printStackTrace();
                                } finally {
                                    retriever.release();
                                }
                                coverPath=saveFirstFrame(firstFrame);
                                netUtil.uploadVideo(MainActivity.this,videoPath,coverPath);
                                break;
                        }
                    }
                });
        builder.show();
    }
    private String contentUri2Path(Uri contentUri){
        String filePath;
        Cursor cursor = getContentResolver().query(contentUri,new String[]{MediaStore.MediaColumns.DATA},null,null,null);
        cursor.moveToFirst();
        filePath = cursor.getString(cursor.getColumnIndexOrThrow(MediaStore.MediaColumns.DATA));
        cursor.close();
        return filePath;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode,Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode==RESULT_OK){
            if(requestCode==REQUEST_CODE_RECORD){
                refreshGallery(videoPath);
                chooseCover(MainActivity.this);
            }else if(requestCode==REQUEST_CODE_PICK_VIDEO){
                videoPath=contentUri2Path(data.getData());
//                Log.d("VideoPath",videoPath);
                chooseCover(MainActivity.this);
            }else if(requestCode==REQUEST_CODE_TAKE_PICTURE){
                refreshGallery(coverPath);
                netUtil.uploadVideo(this,videoPath,coverPath);
            }else if(requestCode==REQUEST_CODE_PICK_PICTURE){
                coverPath=contentUri2Path(data.getData());
//                Log.d("PhotoPath",coverPath);
                netUtil.uploadVideo(this,videoPath,coverPath);
            }
        }else{
            if(requestCode==REQUEST_CODE_RECORD){
                if(new File(videoPath).exists())
                    refreshGallery(videoPath);
            }else if(requestCode==REQUEST_CODE_TAKE_PICTURE){
                if(new File(coverPath).exists())
                    refreshGallery(coverPath);
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        boolean requestOK=true;
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        for(int singleResult:grantResults){
            if(singleResult!=PackageManager.PERMISSION_GRANTED) requestOK=false;
        }
        if(!requestOK) Toast.makeText(this,"权限获取失败",Toast.LENGTH_SHORT).show();
        else{
            switch (requestCode){
                case StorageUtility.REQUEST_CODE_PERMISSION_EXT_STORAGE:
                    StorageUtility.loadSPConfig(MainActivity.this);
                    break;
                case REQUEST_CODE_PERMISSION_CAMERA:
                    Intent intent=new Intent(MediaStore.ACTION_VIDEO_CAPTURE);
                    videoPath=StorageUtility.getOutputMediaPath(true);
                    intent.putExtra(MediaStore.EXTRA_OUTPUT, StorageUtility.getUriForFile(MainActivity.this,videoPath));
                    if(variables.settingGlobal[4])
                        intent.putExtra(MediaStore.EXTRA_DURATION_LIMIT,10);
                    intent.putExtra(MediaStore.EXTRA_VIDEO_QUALITY,1);
                    startActivityForResult(intent,REQUEST_CODE_RECORD);
                    break;
            }
        }
    }
}