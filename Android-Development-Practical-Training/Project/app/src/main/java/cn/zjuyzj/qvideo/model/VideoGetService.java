package cn.zjuyzj.qvideo.model;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Query;

public interface VideoGetService {
    @GET("video")
    Call<VideoList> getVideoList(
            @Query("student_id") String student_id,
            @Header("token") String token
    );
}