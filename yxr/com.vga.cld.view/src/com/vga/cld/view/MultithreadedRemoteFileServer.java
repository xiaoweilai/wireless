package com.vga.cld.view;

import java.io.IOException;
import java.net.BindException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Vector;

public class MultithreadedRemoteFileServer {
	public int listenPort;
	public int maxConnections;
	private boolean quit = false;// 退出
	public ServerSocket server;
//	public static List<ConnectionHandler> taskList = Collections.synchronizedList(new LinkedList<ConnectionHandler>());// 保存所有启动的socket集合
//	public static List<ConnectionHandler> taskList = new LinkedList<ConnectionHandler>();// 保存所有启动的socket集合
	public static Vector<ConnectionHandler> taskList = new Vector<ConnectionHandler>();
	public static Map<String, byte[]> map = new Hashtable<String, byte[]>();
	public static int curListSize =0;
	public static int firststartReceiveCount = 0;
	public static Vector<String> connectedDevices = new Vector<String>();
	public MultithreadedRemoteFileServer(int listenPort,int maxConnections) {
		this.listenPort = listenPort;
		this.maxConnections = maxConnections;
	}

	// 允许客户机连接到服务器,等待客户机请求
	public void acceptConnections() {
		try {
			
			server = new ServerSocket(listenPort,20);
			Socket incomingConnection = null;
			new Thread(runSocket).start();
			while (true) {
				Log.writeSystemLogToFile("等待用户的socket请求======================");

					if(curListSize>maxConnections){
						try {
							Thread.sleep(300);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
						continue;
					}				
				incomingConnection = server.accept();
				Log.writeSystemLogToFile("接收到一个socket请求");
				handleConnection(incomingConnection);
			}
		} catch (BindException e) {
			Log.writeSystemLogToFile("接收socket异常服务器终止！123关闭所有线程");
			if(e.getMessage().indexOf("Address already in use: JVM_Bind")>=0)    
		        System.out.println("在一台主机上同时只能启动一个进程(Only one instance allowed)。");      
		      System.exit(0); 
		} catch (IOException ex) {
			System.out.println("Unable to instantiate a ServerSocket on port: "
					+ listenPort);
			 
		} finally {
			Log.writeConnectCountToFile(taskList.size() + "");
			Log.writeSystemLogToFile("本次socket连接结束，线程名字是"
					+ Thread.currentThread().getName());
		}
	}

	public  Runnable runSocket = new Runnable() {
		public void run() {
			ConnectionHandler sockettemp =null;
			while(true){	
				curListSize = taskList.size();
				for(int i=0;i<curListSize;i++){	
					taskList.get(i).dealSocket(i);
				}					
//				synchronized (map) {
//					Log.writeErroLogToFile("getmap size()=="+map.size());	
//				}
//				Log.writeErroLogToFile("firststartReceiveCount size()=="+firststartReceiveCount);	
			}
		}
	};
	// 与客户机Socket交互以将客户机所请求的文件的内容发送到客户机
	private void handleConnection(Socket connectionToHandle) {
		ConnectionHandler newTask = new ConnectionHandler(connectionToHandle);							
			taskList.add(newTask);
			curListSize = taskList.size();
			Log.writeSystemLogToFile("taskList.size()===" + taskList.size());
			Log.writeConnectCountToFile(taskList.size() + "");

			
		Log.writeSystemLogToFile("启动一个线程开始处理socket请求");
//		new Thread(newTask).start();		
	}

	/**
	 * 服务器启动
	 * 
	 * @throws Exception
	 */
	public void start(){
		Log.writeConnectCountToFile(taskList
				.size() + "");
		acceptConnections();
	}

	/**
	 * 服务器终止,关闭所有线程
	 */
	public void quit() {
		this.quit = true;
		try {
			for (ConnectionHandler tast : taskList) {
				tast.input.close();
				tast.output.close();
			}
			server.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
