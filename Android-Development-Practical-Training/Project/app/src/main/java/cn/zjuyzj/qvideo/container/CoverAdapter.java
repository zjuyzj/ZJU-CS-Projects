package cn.zjuyzj.qvideo.container;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.bumptech.glide.Glide;
import com.bumptech.glide.load.MultiTransformation;
import com.bumptech.glide.load.resource.bitmap.CenterCrop;
import com.bumptech.glide.load.resource.bitmap.RoundedCorners;
import static com.bumptech.glide.load.resource.drawable.DrawableTransitionOptions.withCrossFade;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import cn.zjuyzj.qvideo.R;
import cn.zjuyzj.qvideo.model.VideoInfo;

public class CoverAdapter extends RecyclerView.Adapter<CoverAdapter.CoverViewHolder>{

    List<VideoInfo> videoList=null;
    private static CoverCallback mCallback=null;

    public interface CoverCallback{
        public void onClick(String videoURL);
        public void onLongClick(String videoURL);
    }

    public static class CoverViewHolder extends RecyclerView.ViewHolder{
        private String videoURL;
        private ImageView coverImg;
        private TextView userName;
        private TextView updateTime;
        private View contentView;

        public CoverViewHolder(View v) {
            super(v);
            contentView=v;
            coverImg=v.findViewById(R.id.img_cover);
            userName=v.findViewById(R.id.txt_uname);
            updateTime=v.findViewById(R.id.txt_date);
        }

        public void onBind(VideoInfo info){
            String coverURL=info.getCover().getString("url");;
            SimpleDateFormat formatter=new SimpleDateFormat("yyyy-MM-dd HH:mm");
            Date updateDate=new Date(info.getTime().getLong("update"));
            MultiTransformation imgOptions=new MultiTransformation(
                    new CenterCrop(),
                    new RoundedCorners(40)
            );
            Glide.with(contentView).load(coverURL)
                    .placeholder(R.drawable.loading)
//                    .transition(withCrossFade())
                    .transform(imgOptions)
                    .into(coverImg);
            userName.setText(info.getInfo().getString("uname"));
            updateTime.setText(formatter.format(updateDate));
            videoURL=info.getVideo();
            contentView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    mCallback.onClick(videoURL);
                }
            });
            contentView.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                    mCallback.onLongClick(videoURL);
                    return true;
                }
            });
        }

    }

    public CoverAdapter(List<VideoInfo> videoList){
        this.videoList=videoList;
        return;
    }

    public void setCallback(CoverCallback callback){
        CoverAdapter.mCallback=callback;
    }

    public void refreshInfo(List<VideoInfo> videoList){
        this.videoList=videoList;
        notifyDataSetChanged();
        return;
    }

    @Override
    public CoverViewHolder onCreateViewHolder(ViewGroup viewGroup, int i) {
        return new CoverViewHolder(LayoutInflater.from(viewGroup.getContext())
                .inflate(R.layout.container_cover, viewGroup,false));
    }

    @Override
    public void onBindViewHolder(CoverAdapter.CoverViewHolder coverViewHolder, int i) {
        coverViewHolder.onBind(videoList.get(i));
    }

    @Override
    public int getItemCount() {
        if(this.videoList==null) return 0;
        else return videoList.size();
    }

}
