package com.vga.cld.view;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

import android.app.Activity;
import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.View;
import android.widget.ImageView;

public class MainActivity extends Activity {

	ImageView mImgLcd;
	boolean mDisplayFlag = false;
	Drawable mImg001;
	Drawable mImg002;
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
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		mContext = MainActivity.this;
		mRes = mContext.getResources();
		mImgLcd = (ImageView)findViewById(R.id.img_lcd);
		mImgLcd.setImageResource(R.drawable.yzx001);
		mImgLcd.setClickable(true);
		mImg001 = mRes.getDrawable(R.drawable.yzx001);
		mImg002 = mRes.getDrawable(R.drawable.yzx002);
		mImgLcd.setOnClickListener(new View.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				System.out.println("22222");
				if(mDisplayFlag){
					mImgLcd.setImageDrawable(mImg001);
				} else {
					mImgLcd.setImageDrawable(mImg002);
				}
				mDisplayFlag = !mDisplayFlag;
			}
		});
		try{
			SocketServer();
		}catch(Throwable e){
			e.printStackTrace();
			System.out.println("SocketServer Throwable");
			SocketServer();
		} finally{
			
		}
		
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	private Bitmap changeBitmapSize(Bitmap bitmapOrg){
		//��ȡ���ͼƬ�Ŀ�͸�
        int width = bitmapOrg.getWidth();
        int height = bitmapOrg.getHeight();
        
        //����Ԥת���ɵ�ͼƬ�Ŀ�Ⱥ͸߶�
        int newWidth = 480;
        int newHeight = 800;
        
        //���������ʣ��³ߴ��ԭʼ�ߴ�
        float scaleWidth = ((float) newWidth) / width;
        float scaleHeight = ((float) newHeight) / height;
        
        // ��������ͼƬ�õ�matrix����
        Matrix matrix = new Matrix();
        
        // ����ͼƬ����
        matrix.postScale(scaleWidth, scaleHeight);
        
        //��תͼƬ ����
//        matrix.postRotate(45);
        
        // �����µ�ͼƬ
        bitmapOrg = Bitmap.createBitmap(bitmapOrg, 0, 0,
        width, height, matrix, true);
        
        //�����洴����Bitmapת����Drawable����ʹ�������ʹ����ImageView, ImageButton��
//        BitmapDrawable bmd = new BitmapDrawable(resizedBitmap);
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
				System.out.println("sendMessage successed��");
				imgBitmapShow = PublicFunction.bytesToBimap(getByteMessage);
//				imgBitmapShow = changeBitmapSize(imgBitmapShow);
				mImgLcd.setImageBitmap(imgBitmapShow);
			}
		}
		
	};
	Runnable run = new Runnable() {
		
		@Override
		public void run() {
			 ServerSocket ss = null;

	         try {
				 ss = new ServerSocket();
	        	 ss.bind(new InetSocketAddress("192.168.1.100",16689)); 
		         //���������յ��ͻ��˵����ݺ󣬴�����˿ͻ��˶Ի���Socket
	        	 System.out.println("ss.acceppt() before");
		         socket = ss.accept();
		         System.out.println("ss.accept() after");
		         //������ͻ��˷������ݵ������
		         dos = new DataOutputStream(socket.getOutputStream());
		         //���ڽ��տͻ��˷��������ݵ�������
		         dis = new DataInputStream(socket.getInputStream());
//		         System.out.println("���������յ��ͻ��˵���������" + dis.readUTF());
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
	
public void receiveMessage(DataInputStream input, Socket s, DataOutputStream output){
		
		int numRead = 0;
		
		try {
			
			int tempBytelength = input.available();
			if(!receiveSuccessedBeforeFourByteFlag){
	            if(tempBytelength >= 100){
	            	System.out.println("receive image size tempBytelength =="+tempBytelength + "�� mImageReceiveCount��ͼƬ" + mImageReceiveCount);
	            	long tempSize = input.readLong();
	            	curImageSize = (int)tempSize;
//	            	System.out.println("tempCount =="+curImageSize);
	            	curImageNameLength = (int)input.readLong();
//	            	System.out.println("curImageNameLength =="+curImageNameLength);
	            	getImageNameByte = new byte[curImageNameLength];
	            	int name = input.read(getImageNameByte, 0, curImageNameLength);
//	            	System.out.println("curImageName  longth ==" + name);
	            	curImageSize = curImageSize - 8 - 8 -curImageNameLength;
	            	getByteMessage = new byte[curImageSize];
	            	receiveSuccessedBeforeFourByteFlag = true;
	            }
	            return;
			}else {
//				System.out.println("receive image��tempBytelength ==" + tempBytelength);
//				if(tempBytelength >= curImageSize - byteLong){
					numRead=input.read(getByteMessage, byteLong, curImageSize - byteLong);
	
//					System.out.println("numRead is image byte>=��" + numRead);
					if(byteLong < curImageSize){
						byteLong += numRead;
						return;
					} else if(byteLong >= curImageSize){
						byteLong = 0;
						curImageSize = 0;
						curImageNameLength = 0;
						tempBytelength = 0;
//						System.out.println("sendMessage before��");
						msg = new Message();
						msg.what = 1;
						mHandler.sendMessage(msg);
						mImageReceiveCount++;
						System.out.println("mImageReceiveCount succsed count��" + mImageReceiveCount);
//						mHandler.sendMessage(mHandler.obtainMessage(1));
						

						receiveSuccessedBeforeFourByteFlag = false;
						
					}else{
//						 System.out.println("numRead not read all" + numRead);
					}
//				}
			}
		} catch (IOException e) {
			Log.writeErroLogToFile("socket input output�쳣"
					+ Thread.currentThread().getName());
				Log.writeErroLogToFile("�߳������쳣" + Thread.currentThread().getName());
				System.out.println("�߳������쳣" + Thread.currentThread().getName());
			
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
		
}
