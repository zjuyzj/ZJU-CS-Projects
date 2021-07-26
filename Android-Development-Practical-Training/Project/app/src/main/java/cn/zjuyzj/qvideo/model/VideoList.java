package cn.zjuyzj.qvideo.model;

import com.google.gson.annotations.SerializedName;

import java.util.List;

public class VideoList {
    @SerializedName("feeds")
    private List<VideoInfo> list;

    @SerializedName("success")
    private boolean success;

    public List<VideoInfo> getList(){
        return list;
    }

    public boolean isSucceed(){
        return success;
    }
}
