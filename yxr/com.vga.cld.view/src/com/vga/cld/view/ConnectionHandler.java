package com.vga.cld.view;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;

public class ConnectionHandler {
	private String TAG = "ConnectionHandler  ";
	private Socket s;
	public DataInputStream input;
	public DataOutputStream output;
	private byte[] getByteMessage = new byte[1704];
	private String medNumStr = null;
	private String monitorIDStr = null;
	private byte first = (byte) 0X9C;
	private byte secend = (byte) 0XFF;
	private boolean firstConn = true;
	private boolean firsrin = true;
	int byteLong = 0;
	String threadName;
	long startTime = 0;
	long lastEndTime = 0;
	public ConnectionHandler(Socket socketToHandle) {
		this.s = socketToHandle;
		try {
			s.setSoTimeout(10000);
		} catch (SocketException e1) {
			e1.printStackTrace();
		}
		try {
			input = new DataInputStream(s.getInputStream());
			output = new DataOutputStream(s.getOutputStream());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}	

	public void receiveMessage(){
		
		int numRead = 0;
		
		try {
			if(firsrin){
				lastEndTime =  System.currentTimeMillis();
				firsrin = false;
			}
			startTime = System.currentTimeMillis();
			if((startTime-lastEndTime)>5000){
				throw new IOException();
			}
			int tmepBytelength = input.available();
//			Log.writeErroLogToFile("本次接收数据的个数==="+tmepBytelength);
//			Log.writeErroLogToFile("本次getByteMessage.length==="+getByteMessage.length);
			if(tmepBytelength>0){
				if(tmepBytelength>=(getByteMessage.length-byteLong)){
					if(byteLong < getByteMessage.length&& (numRead=input.read(getByteMessage, byteLong, getByteMessage.length-byteLong)) >= 0){
//						Log.writeErroLogToFile("本次接收数据的成功的个数@@@@@numRead==="+numRead);
						byteLong+=	numRead;
					}
				}
				else{
					if(byteLong < getByteMessage.length&& (numRead=input.read(getByteMessage, byteLong, byteLong+tmepBytelength)) >= 0){
//						Log.writeErroLogToFile("本次接收数据的成功的个数numRead==="+numRead);					
						byteLong+=	numRead;					
					}
				}
				lastEndTime =  System.currentTimeMillis();
			}
			
		} catch (IOException e) {
			Log.writeErroLogToFile("socket input output异常"
					+ Thread.currentThread().getName());
				Log.writeErroLogToFile("线程连接异常" + Thread.currentThread().getName());

				MultithreadedRemoteFileServer.taskList.remove(this);
				MultithreadedRemoteFileServer.curListSize = MultithreadedRemoteFileServer.taskList.size();
				Log.writeErroLogToFile("taskList.size()===" + MultithreadedRemoteFileServer.taskList.size());
				Log.writeConnectCountToFile(MultithreadedRemoteFileServer.taskList
						.size() + "");
				MultithreadedRemoteFileServer.connectedDevices.remove(monitorIDStr);			
			
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
		}
	}
	public void dealSocket(int mainid) {
		try {
			
		}catch (Exception e) {
			
		}
	}
}
