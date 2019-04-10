package com.wanglei55.ndk;

import android.os.Looper;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Arrays;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "JNI";

    static {
        System.loadLibrary("Test");
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        //传递基本数据类型，数组
        int[] a1 = {12,23,34,45};
        String[] a2 = {"PaDo","你好"};
        arrayTest(789,a1,a2);
        Log.e(TAG, "java数组变为："+ Arrays.toString(a1));

        //
        Log.e(TAG, "---------------------------------");
        objectTest(new Student(),"objectTest");
        //
        Log.e(TAG, "---------------------------------");
        //1、动态注册
        dynamicJavaTest();
        int dd = dynamicJavaTest2(66);
        Log.e(TAG, "dynamic :"+dd);
        //2、native线程
        testThread();
    }

    public void callBack(){
        if (Looper.myLooper() == Looper.getMainLooper()){
            Toast.makeText(this,"MainLooper",Toast.LENGTH_SHORT).show();
        }else{
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Toast.makeText(MainActivity.this,"runOnUiThread",Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    //native
    native int arrayTest(int i,int[] a1, String[] a2);

    native void objectTest(Student s, String str);

    public native String stringFromJNI();

    native void refTest();

    native  void dynamicJavaTest();
    native  int dynamicJavaTest2(int i);

    native void testThread();

}
