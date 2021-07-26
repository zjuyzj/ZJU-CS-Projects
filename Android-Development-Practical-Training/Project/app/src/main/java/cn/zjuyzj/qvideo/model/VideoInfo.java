package cn.zjuyzj.qvideo.model;

import android.os.Bundle;

import com.google.gson.annotations.SerializedName;

import java.text.SimpleDateFormat;
import java.util.TimeZone;

public class VideoInfo {
    @SerializedName("_id")
    private String _id;

    @SerializedName("student_id")
    private String student_id;

    @SerializedName("user_name")
    private String user_name;

    @SerializedName("video_url")
    private String video_url;

    @SerializedName("image_url")
    private String image_url;

    @SerializedName("image_w")
    private int image_w;

    @SerializedName("image_h")
    private int image_h;

    @SerializedName("createdAt")
    private String createdAt;

    @SerializedName("updatedAt")
    private String updatedAt;

    public Bundle getInfo(){
        Bundle result=new Bundle();
        result.putString("vid", _id);
        result.putString("uid", student_id);
        result.putString("uname",user_name );
        return result;
    }

    public Bundle getCover(){
        Bundle result=new Bundle();
        result.putInt("width", image_w);
        result.putInt("height", image_h);
        result.putString("url", image_url);
        return result;
    }

    public String getVideo(){
        return video_url;
    }

    public Bundle getTime() {
        long create_timestamp=0,update_timestamp=0;
        Bundle result=new Bundle();
        SimpleDateFormat formatter=new SimpleDateFormat("yyyy-MM-dd'T'HH:mm:ss.SSS'Z'");
        formatter.setTimeZone(TimeZone.getTimeZone("UTC"));
        try{
            create_timestamp=formatter.parse(createdAt).getTime();
            update_timestamp=formatter.parse(updatedAt).getTime();
        }catch (Exception e){
            e.printStackTrace();
        }
        result.putLong("create",create_timestamp);
        result.putLong("update",update_timestamp);
        return result;
    }

}
