package com.vga.cld.view;

import com.vga.cld.view.MainActivity.MyHandler;

import android.app.Application;
import android.os.Handler;

public class MyAPP extends Application {  
    // �������  
    private MyHandler handler = null;  
      
    // set����  
    public void setHandler(MyHandler mHandler) {  
        this.handler = mHandler;  
    }  
      
    // get����  
    public MyHandler getHandler() {  
        return handler;  
    }  
}  