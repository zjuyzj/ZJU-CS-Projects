package com.bytedance.practice5;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.bytedance.practice5.model.Message;
import com.facebook.drawee.view.SimpleDraweeView;


import java.util.List;

public class FeedAdapter extends RecyclerView.Adapter<FeedAdapter.VideoViewHolder>{
    private List<Message> data;
    public void setData(List<Message> messageList){
        data = messageList;
        notifyDataSetChanged();
    }

    @NonNull
    @Override
    public VideoViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View root =LayoutInflater.from(parent.getContext()).inflate(R.layout.item_feed,parent,false);
        return new VideoViewHolder(root);
    }

    @Override
    public void onBindViewHolder(@NonNull VideoViewHolder holder, int position) {
        holder.bind(data.get(position));
    }

    @Override
    public int getItemCount() {
        return data==null?0:data.size();
    }

    public static class VideoViewHolder extends RecyclerView.ViewHolder{
        private SimpleDraweeView coverSD;
        private TextView fromTV;
        private TextView toTV;
        private TextView contentTV;
        public VideoViewHolder(@NonNull View itemView) {
            super(itemView);
            fromTV = itemView.findViewById(R.id.tv_from);
            toTV = itemView.findViewById(R.id.tv_to);
            contentTV = itemView.findViewById(R.id.tv_content);
            coverSD = itemView.findViewById(R.id.sd_cover);
        }
        public void bind(Message message){
            coverSD.setImageURI(message.getImageUrl());
            fromTV.setText("FROM: "+message.getFrom());
            contentTV.setText(message.getContent());
            toTV.setText("TO: "+message.getTo());
        }
    }

}
