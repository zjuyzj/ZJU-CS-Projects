package cn.zjuyzj.qvideo.model;

import com.google.gson.annotations.SerializedName;

public class UploadResponse {
    @SerializedName("result")
    private VideoInfo video_info;

    @SerializedName("url")
    private String url;

    @SerializedName("success")
    private boolean success;

    public boolean isSuccess(){
        return success;
    }

}
