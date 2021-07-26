package cn.zjuyzj.qvideo.util;

import android.app.Activity;
import android.content.Context;
import java.io.File;
import java.util.List;
import java.util.concurrent.TimeUnit;

import cn.zjuyzj.qvideo.model.UploadResponse;
import cn.zjuyzj.qvideo.model.VideoGetService;
import cn.zjuyzj.qvideo.model.VideoInfo;
import cn.zjuyzj.qvideo.model.VideoList;
import cn.zjuyzj.qvideo.model.VideoUploadService;
import cn.zjuyzj.qvideo.var.variables;
import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.RequestBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class NetworkUtility {
    private static final String netToken="WkpVLWJ5dGVkYW5jZS1hbmRyb2lk";
    private static final int sizeLimitMB=30;
    private Retrofit retrofit=null;
    private VideoGetService videoGetAPI=null;
    private VideoUploadService videoUploadAPI=null;
    private NetCallback mCallback=null;

    public interface NetCallback{
        public void onRequest(List<VideoInfo> videos);
        public void onError(String reason);
        public void onRefresh();
        public void onSuccess();
    }

    public void initNetwork(NetCallback mCallback){
        this.mCallback=mCallback;
        OkHttpClient client = new OkHttpClient.Builder() //Setting User-defined Timeout to 100s for Slow Network
                .connectTimeout(100, TimeUnit.SECONDS)
                .readTimeout(100,TimeUnit.SECONDS).build();
        retrofit=new Retrofit.Builder()
                .baseUrl("https://api-android-camp.bytedance.com/zju/invoke/")
                .client(client)
                .addConverterFactory(GsonConverterFactory.create())
                .build();
        videoGetAPI=retrofit.create(VideoGetService.class);
        videoUploadAPI=retrofit.create(VideoUploadService.class);
    }

    public void getVideos(String student_id){
        mCallback.onRefresh();
        Call<VideoList> request=videoGetAPI.getVideoList(student_id,netToken);
        request.enqueue(new Callback<VideoList>() {
            @Override
            public void onResponse(Call<VideoList> call, Response<VideoList> response) {
                VideoList list=response.body();
                if(list==null||!list.isSucceed())
                    mCallback.onError("服务器异常");
                else
                    mCallback.onRequest(list.getList());
            }

            @Override
            public void onFailure(Call<VideoList> call, Throwable t) {
                mCallback.onError("无网络或网络错误");
                t.printStackTrace();
            }
        });
    }

    public void uploadVideo(Context ctx, String videoPath, String coverPath) {
        File videoFile,coverFile;
        videoFile=new File(videoPath);
        coverFile=new File(coverPath);
        if (videoFile.length()+ coverFile.length() > sizeLimitMB * 1024 * 1024) {
            mCallback.onError("视频和封面超过" + sizeLimitMB + "MB，请重新选择");
            return;
        } else { //Start Uploading
            MultipartBody.Part coverPart = MultipartBody.Part.createFormData("cover_image", coverFile.getName(),
                    RequestBody.create(MediaType.parse("multipart/form-data"),coverFile));
            MultipartBody.Part videoPart = MultipartBody.Part.createFormData("video", videoFile.getName(),
                    RequestBody.create(MediaType.parse("multipart/form-data"), videoFile));
            Call<UploadResponse> submit = videoUploadAPI.submitMessage(variables.myID, variables.myName, "", coverPart, videoPart, netToken);
            submit.enqueue(new Callback<UploadResponse>() {
                @Override
                public void onResponse(Call<UploadResponse> call, Response<UploadResponse> response) {
                    if (!response.isSuccessful()) {
                        mCallback.onError("服务器异常");
                        //Log.d("ResCode",response.code());
                    } else {
                        UploadResponse res = response.body();
                        if (!res.isSuccess()) {
                            mCallback.onError("上传失败");
                        } else {
                            mCallback.onSuccess();
                        }
                    }
                }
                @Override
                public void onFailure(Call<UploadResponse> call, Throwable t) {
                    mCallback.onError("无网络或网络错误");
                    t.printStackTrace();
                }
            });
            return;
        }
    }

}
