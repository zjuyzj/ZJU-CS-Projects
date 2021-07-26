package com.bytedance.practice5;


import com.bytedance.practice5.model.UploadResponse;

import okhttp3.MultipartBody;
import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.Header;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.Part;
import retrofit2.http.Path;
import retrofit2.http.Query;

public interface IApi {
    @Multipart
    @POST("messages")
    Call<UploadResponse> submitMessage(@Query("student_id") String studentId,
                                       @Query("extra_value") String extraValue,
                                       @Part MultipartBody.Part from,
                                       @Part MultipartBody.Part to,
                                       @Part MultipartBody.Part content,
                                       @Part MultipartBody.Part image,
                                       @Header("token") String token);
}
