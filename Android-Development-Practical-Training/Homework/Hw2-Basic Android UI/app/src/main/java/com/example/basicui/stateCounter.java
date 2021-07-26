package com.example.basicui;

import android.util.Log;

public class stateCounter{
    private static int state=0;
    public int stateInc(){
        state++;
        if(state>=4) state=0;
        Log.d("StateCounter",String.valueOf(state));
        return state;
    }
    public void stateRst(){
        state=0;
        Log.d("StateCounter","Reseted");
    }
}