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
	private boolean quit = false;// �˳�
	public ServerSocket server;
//	public static List<ConnectionHandler> taskList = Collections.synchronizedList(new LinkedList<ConnectionHandler>());// ��������������socket����
//	public static List<ConnectionHandler> taskList = new LinkedList<ConnectionHandler>();// ��������������socket����
	public static Vector<ConnectionHandler> taskList = new Vector<ConnectionHandler>();
	public static Map<String, byte[]> map = new Hashtable<String, byte[]>();
	public static int curListSize =0;
	public static int firststartReceiveCount = 0;
	public static Vector<String> connectedDevices = new Vector<String>();
	public MultithreadedRemoteFileServer(int listenPort,int maxConnections) {
		this.listenPort = listenPort;
		this.maxConnections = maxConnections;
	}

	// ����ͻ������ӵ�������,�ȴ��ͻ�������
	public void acceptConnections() {
		try {
			
			server = new ServerSocket(listenPort,20);
			Socket incomingConnection = null;
			new Thread(runSocket).start();
			while (true) {
				Log.writeSystemLogToFile("�ȴ��û���socket����======================");

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
				Log.writeSystemLogToFile("���յ�һ��socket����");
				handleConnection(incomingConnection);
			}
		} catch (BindException e) {
			Log.writeSystemLogToFile("����socket�쳣��������ֹ��123�ر������߳�");
			if(e.getMessage().indexOf("Address already in use: JVM_Bind")>=0)    
		        System.out.println("��һ̨������ͬʱֻ������һ������(Only one instance allowed)��");      
		      System.exit(0); 
		} catch (IOException ex) {
			System.out.println("Unable to instantiate a ServerSocket on port: "
					+ listenPort);
			 
		} finally {
			Log.writeConnectCountToFile(taskList.size() + "");
			Log.writeSystemLogToFile("����socket���ӽ������߳�������"
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
	// ��ͻ���Socket�����Խ��ͻ�����������ļ������ݷ��͵��ͻ���
	private void handleConnection(Socket connectionToHandle) {
		ConnectionHandler newTask = new ConnectionHandler(connectionToHandle);							
			taskList.add(newTask);
			curListSize = taskList.size();
			Log.writeSystemLogToFile("taskList.size()===" + taskList.size());
			Log.writeConnectCountToFile(taskList.size() + "");

			
		Log.writeSystemLogToFile("����һ���߳̿�ʼ����socket����");
//		new Thread(newTask).start();		
	}

	/**
	 * ����������
	 * 
	 * @throws Exception
	 */
	public void start(){
		Log.writeConnectCountToFile(taskList
				.size() + "");
		acceptConnections();
	}

	/**
	 * ��������ֹ,�ر������߳�
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
