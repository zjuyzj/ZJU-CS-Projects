package cn.zjuyzj.qvideo.util;

import android.Manifest;
import android.app.Activity;
import android.app.DownloadManager;
import android.content.Context;
import android.content.SharedPreferences;
import android.net.Uri;
import android.os.Build;
import android.os.Environment;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;
import androidx.core.content.FileProvider;

import java.io.File;
import java.net.URI;
import java.text.SimpleDateFormat;
import java.util.Date;

import cn.zjuyzj.qvideo.var.variables;

public class StorageUtility {
    public static final int REQUEST_CODE_PERMISSION_EXT_STORAGE=0;
    private Context ctx;
    boolean externalAvail=false;
    public StorageUtility(Context ctx){
        this.ctx=ctx;
        String status = Environment.getExternalStorageState();
        externalAvail=status.equals(Environment.MEDIA_MOUNTED);
    }

    public static void loadSPConfig(Context ctx){
        SharedPreferences sp=ctx.getSharedPreferences("global_settings",Context.MODE_PRIVATE);
        if(!sp.getBoolean("existed", false)){
            SharedPreferences.Editor editor=sp.edit();
            editor.putBoolean("existed",true);
            editor.commit();
            storeSPConfig(ctx);
        }else{
            variables.myID=sp.getString("myID", "");
            variables.myName=sp.getString("myName","");
            variables.settingGlobal[0]=sp.getBoolean("enableDownload", false);
            variables.settingGlobal[1]=sp.getBoolean("enableLike",true);
            variables.settingGlobal[2]=sp.getBoolean("enableGesture",true);
            variables.settingGlobal[3]=sp.getBoolean("enableLoop",true);
            variables.settingGlobal[4]=sp.getBoolean("enableTimeLimit",false);
        }
        return;
    }

    public static void storeSPConfig(Context ctx){
        SharedPreferences.Editor editor=ctx.getSharedPreferences("global_settings",Context.MODE_PRIVATE).edit();
        editor.putString("myID",variables.myID);
        editor.putString("myName",variables.myName);
        editor.putBoolean("enableDownload",variables.settingGlobal[0]);
        editor.putBoolean("enableLike",variables.settingGlobal[1]);
        editor.putBoolean("enableGesture",variables.settingGlobal[2]);
        editor.putBoolean("enableLoop",variables.settingGlobal[3]);
        editor.putBoolean("enableTimeLimit",variables.settingGlobal[4]);
        editor.commit();
        return;
    }

    public static void requestExternalStoragePermission(Activity activity){
        String[] permissions={Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE};
        ActivityCompat.requestPermissions(activity,permissions,REQUEST_CODE_PERMISSION_EXT_STORAGE);
        return;
    }

    public void downloadVideo(String url){
        if(!externalAvail) Toast.makeText(ctx, "下载失败，外部存储不可用", Toast.LENGTH_SHORT).show();
        DownloadManager.Request request=new DownloadManager.Request(Uri.parse(url));
        request.setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS,url.substring(url.lastIndexOf("/") + 1));
        request.setNotificationVisibility(request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED);
        DownloadManager manager=(DownloadManager)ctx.getSystemService(Context.DOWNLOAD_SERVICE);
        manager.enqueue(request);
        Toast.makeText(ctx, "开始下载，完成后在通知栏提示", Toast.LENGTH_SHORT).show();
    }

    public static String getOutputMediaPath(boolean isVideo) {
        File mediaFile;
        File mediaStorageDir = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM+"/QuickVideo");
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        if(isVideo){
            mediaFile = new File(mediaStorageDir, "VID_" + timeStamp + ".mp4");
        }else{
            mediaFile = new File(mediaStorageDir, "IMG_" + timeStamp + ".jpg");
        }
        if (!mediaFile.exists()) {
            mediaFile.getParentFile().mkdirs();
        }
        return mediaFile.getAbsolutePath();
    }

    public static Uri getUriForFile(Context context,String path) {
        if (Build.VERSION.SDK_INT >= 24) {
            return FileProvider.getUriForFile(context.getApplicationContext(), context.getApplicationContext().getPackageName() + ".fileprovider", new File(path));
        } else {
            return Uri.fromFile(new File(path));
        }
    }

}
