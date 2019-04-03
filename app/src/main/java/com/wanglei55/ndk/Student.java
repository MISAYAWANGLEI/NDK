package com.wanglei55.ndk;

import android.util.Log;

import java.util.List;

public class Student {

    private int num = 100;

    public int getNum() {
        return num;
    }

    public void setNum(int num) {
        this.num = num;
    }

    public static void printMsg(Card card){
        Log.e("JNI","printMsg Card: "+card.id);
    }

    public static void printMsg(String str){
        Log.e("JNI","printMsg: "+str);
    }

}
