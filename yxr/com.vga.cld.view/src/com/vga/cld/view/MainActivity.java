package com.vga.cld.view;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MainActivity extends Activity implements View.OnClickListener{

	ImageView mImgLcd;
	Drawable mImg000;
	Drawable mImg001;
	Drawable mImg002;
	Drawable mImg003;
	Drawable mImg004;
	Drawable mImg005;
	Drawable mImgRec;
	Context mContext;
	Resources mRes;
	private byte[] getByteMessage;
	private byte[] getImageNameByte;
	private int byteLong;
	private boolean firstConn = true;
	private static boolean startReadImagFlag = true;
	private int curImageSize = 0;
	private int curImageNameLength = 0;
	Socket socket;
	private static boolean receiveSuccessedFlag = false;
	private static boolean receiveSuccessedBeforeFourByteFlag = false;
	DataOutputStream dos = null;
	DataInputStream dis = null;
	Drawable imgShow;
	Bitmap imgBitmapShow;
	Message msg;
	private static int mImageReceiveCount;
	private TextView mTextIPAddress;
	private String mIpAddress;
	private RelativeLayout mRelMain;
	private long startTime;
	private int mCurImageIndex;
	
	@SuppressLint("NewApi")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mContext = MainActivity.this;
		mRes = mContext.getResources();
		PublicFunction.getScreenWithAndHeight(mContext);
		mImgLcd = (ImageView)findViewById(R.id.img_lcd);
		mImgLcd.setImageResource(R.drawable.yzx001);
		mImgLcd.setClickable(true);
		mImg000 = mRes.getDrawable(R.drawable.yzx001);
		mImg001 = mRes.getDrawable(R.drawable.yzx002);
		mImg002 = mRes.getDrawable(R.drawable.yxr003);
		mImg003 = mRes.getDrawable(R.drawable.yxr004);
		mImg004 = mRes.getDrawable(R.drawable.yxr005);
		mImg005 = mRes.getDrawable(R.drawable.yxr006);
		mTextIPAddress = (TextView)findViewById(R.id.ip_address);
		mRelMain = (RelativeLayout)findViewById(R.id.rel_main);
		mRelMain.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);
		mRelMain.setOnClickListener(this);
		try {
			mIpAddress = PublicFunction.getLocalIPAddress();
		} catch (SocketException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if(mIpAddress != null){
			mTextIPAddress.setText(mIpAddress);
		} else {
			mTextIPAddress.setText("未获取正确的IP地址 ，请检查后在查看。");
		}
		mImgLcd.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				System.out.println("22222");
				
				switch (mCurImageIndex){ 
				case 0 :
//					mImgLcd.setImageDrawable(mImg000);
					mImgLcd.setImageResource(R.drawable.yzx001);
					break; 
				case 1 :
//					mImgLcd.setImageDrawable(mImg001);
					mImgLcd.setImageResource(R.drawable.yzx002);
					break; 
				case 2 :
//					mImgLcd.setImageDrawable(mImg002);	
					mImgLcd.setImageResource(R.drawable.yxr003);
					break; 
				case 3 :
//					mImgLcd.setImageDrawable(mImg003);
					mImgLcd.setImageResource(R.drawable.yxr004);
					break; 
				case 4 :
//					mImgLcd.setImageDrawable(mImg004);
					mImgLcd.setImageResource(R.drawable.yxr005);
					break; 
				case 5 :
//					mImgLcd.setImageDrawable(mImg005);
					mImgLcd.setImageResource(R.drawable.yxr006);
					break; 
				default : 
				break; 
				}
				mCurImageIndex ++;
				if(mCurImageIndex == 6){
					mCurImageIndex = 0;
				}
			}
		});
		if(mIpAddress != null){
			try{
				SocketServer();
			}catch(Throwable e){
				e.printStackTrace();
				System.out.println("SocketServer Throwable");
				SocketServer();
			} finally{
				
			}
		}				
	}
	
	@Override
	protected void onResume() {
		try {
			mIpAddress = PublicFunction.getLocalIPAddress();
		} catch (SocketException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		if(mIpAddress != null){
			mTextIPAddress.setText(mIpAddress);
		} else {
			mTextIPAddress.setText("未获取正确的IP地址，请检查后在查看。");
		}	
		super.onResume();
	}

	

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private Bitmap changeBitmapSize(Bitmap bitmapOrg){
        int width = bitmapOrg.getWidth();
        int height = bitmapOrg.getHeight();
        int newWidth = 560;
        int newHeight = 400;
        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        Matrix matrix = new Matrix();
        matrix.postScale(scaleWidth, scaleHeight);
//        matrix.postRotate(45);     
        bitmapOrg = Bitmap.createBitmap(bitmapOrg, 0, 0,
        width, height, matrix, true);
        return bitmapOrg;
	}
	
	private void SocketServer(){
		byteLong = 0;
		curImageSize = 0;
		curImageNameLength = 0;
		mImageReceiveCount = 0;
		Thread receiveImag = new Thread(run);
		receiveImag.start();
	}
	
	Handler mHandler = new Handler(){

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			if(msg.what == 1){
				System.out.println("图片处理开始时间 mHandler startTime =" + (System.currentTimeMillis()-startTime));
				imgBitmapShow = PublicFunction.bytesToBimap(getByteMessage);
				System.out.println("图片处理结束时间 mHandler endTime = "+(System.currentTimeMillis() - startTime));
//				imgBitmapShow = changeBitmapSize(imgBitmapShow);
				if(imgBitmapShow != null){
					mImgLcd.setImageBitmap(imgBitmapShow);
				}
//				PublicFunction.saveBitmap(imgBitmapShow);
				System.out.println("图片显示完成时间 "+(System.currentTimeMillis() - startTime));
			}
		}
		
	};
	
	Runnable run = new Runnable() {
		
		@Override
		public void run() {
			 ServerSocket ss = null;
	         try {
				 ss = new ServerSocket();
	        	 ss.bind(new InetSocketAddress(mIpAddress,16689)); 
		         //服务器接收到客户端的数据后，创建与此客户端对话的Socket
	        	 System.out.println("ss.acceppt() before");
		         socket = ss.accept();
		         System.out.println("ss.accept() after");
		         //用于向客户端发送数据的输出流
		         dos = new DataOutputStream(socket.getOutputStream());
		         //用于接收客户端发来的数据的输入流
		         dis = new DataInputStream(socket.getInputStream());
//		         System.out.println("服务器接收到客户端的连接请求：" + dis.readUTF());
		         while(!receiveSuccessedFlag){
		        	dealSocket();					
		         }
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				SocketServer();
			}
			
		}
	};
//	Runnable runDealBitmap = new Runnable() {
//			
//			@Override
//			public void run() {
//				
//			}
//		};
//	
//	private void startDealBitmapThread(){
//		HandlerThread receiveImag = new HandlerThread(runDealBitmap);
//		receiveImag.start();
//	}
//	
	
public void receiveMessage(DataInputStream input, Socket s, DataOutputStream output){
		
		int numRead = 0;
		try {
			
			int tempBytelength = input.available();
			if(!receiveSuccessedBeforeFourByteFlag){
	            if(tempBytelength >= 100){
	            	System.out.println("receive image size tempBytelength =="+tempBytelength + "第 mImageReceiveCount张图片" + mImageReceiveCount);
			        startTime = System.currentTimeMillis();
			        System.out.println("第 mImageReceiveCount张图片开始接收" + mImageReceiveCount + "开始时间="+startTime);
	            	long tempSize = input.readLong();
	            	curImageSize = (int)tempSize;
	            	curImageNameLength = (int)input.readLong();
	            	getImageNameByte = new byte[curImageNameLength];
	            	int name = input.read(getImageNameByte, 0, curImageNameLength);
	            	curImageSize = curImageSize - 8 - 8 -curImageNameLength;
	            	getByteMessage = new byte[curImageSize];
	            	receiveSuccessedBeforeFourByteFlag = true;
	            }
	            return;
			}else {
					numRead=input.read(getByteMessage, byteLong, curImageSize - byteLong);
					if(byteLong < curImageSize){
						byteLong += numRead;
						return;
					} else if(byteLong >= curImageSize){
						byteLong = 0;
						curImageSize = 0;
						curImageNameLength = 0;
						tempBytelength = 0;
						msg = new Message();
						msg.what = 1;
						mHandler.sendMessage(msg);
						System.out.println("mImageReceiveCount succsed count：" + mImageReceiveCount);
						System.out.println("第"+mImageReceiveCount+ "张图片接收结束时间endTime =" + (System.currentTimeMillis()-startTime));
						mImageReceiveCount++;
						receiveSuccessedBeforeFourByteFlag = false;
						
					}else{
//						 System.out.println("numRead not read all" + numRead);
				}
			}
		} catch (IOException e) {
			Log.writeErroLogToFile("socket input output异常"
					+ Thread.currentThread().getName());
				Log.writeErroLogToFile("线程连接异常" + Thread.currentThread().getName());
				System.out.println("线程连接异常" + Thread.currentThread().getName());
			
			try {
				if (null != input)
					input.close();
				if (null != output)
					output.close();
				if (null != s)
					s.close();
			} catch (IOException ee) {
				ee.printStackTrace();
			}
			SocketServer();
		}
	}

		public void dealSocket() {
			if(firstConn){
				receiveMessage(dis,socket,dos);
			}
		}

		@Override
		public void finish() {
			super.finish();
			android.os.Process.killProcess(android.os.Process.myPid());
		}

		@SuppressLint("NewApi")
		@Override
		public void onClick(View v) {
//			int i = mRelMain.getSystemUiVisibility();  
//			  if (i == View.SYSTEM_UI_FLAG_HIDE_NAVIGATION) {  
//				  mRelMain.setSystemUiVisibility(View.SYSTEM_UI_FLAG_VISIBLE);  
//			  } else if (i == View.SYSTEM_UI_FLAG_VISIBLE){  
//				  mRelMain.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LOW_PROFILE);  
//			  } else if (i == View.SYSTEM_UI_FLAG_LOW_PROFILE) {  
//				  mRelMain.setSystemUiVisibility(View.SYSTEM_UI_FLAG_HIDE_NAVIGATION);  
//			  }   
		}				
}
