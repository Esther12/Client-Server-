package com.androidsrc.client;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import android.os.AsyncTask;
import android.widget.TextView;
import java.net.*;
import java.io.*;
import java.io.PrintWriter;
public class Client extends AsyncTask<Void, Void, Void> {

String command;

	private PrintWriter out = null;
	String dstAddress;
	int dstPort;
	String response = "";

	TextView textResponse;

	Client(String addr, int port,TextView textResponse,String com) {
		dstAddress = addr;
		dstPort = port;
		this.textResponse=textResponse;
		command=com;
	}

	@Override
	protected Void doInBackground(Void... arg0) {

		Socket socket = null;
	//	DataOutputStream dataOutputStream = null;



		try {
			//testing: send  command  as  string,skip user input part
			String line = "ls ";
			socket = new Socket(dstAddress, dstPort);
			//out = socket.getOutputStream();
			out = new PrintWriter(socket.getOutputStream());
			out.write(this.command);
			out.flush();

//
//			//mcy
//			while(true)
//			{
				//String line = "ls ";
//				char[] strArray;
//				strArray = line.toCharArray();

//			System.out.println("MMMM"+strArray.length);
//			//这里确实是3
//				for( int index = 0; index < strArray.length; index++){
//
//					System.out.println("QQQ"+strArray[index]);
//
//					out.write(strArray[index]);
//				}
			//System.out.println("XIXI"+command);





//for(int i=0;i<100;i++)
//{String line = "ls";
//	dataOutputStream = new DataOutputStream(socket.getOutputStream());
//
//
//	dataOutputStream.writeUTF(line);
//
//}


			ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream(
					1024);
			byte[] buffer = new byte[1024];

			int bytesRead;
			InputStream inputStream = socket.getInputStream();

			//getInputStream from server.c

			/*
			 * notice: inputStream.read() will block if no data return
			 *
			 */
			while ((bytesRead = inputStream.read(buffer)) != -1) {
				byteArrayOutputStream.write(buffer, 0, bytesRead);
				response += byteArrayOutputStream.toString("UTF-8");
				//add a text view to show the excute result from the server

			System.out.println("val returned"+response);
				//this.textResponse.setText(response);
				//ShowResult(response);
			}

			//}

		} catch (UnknownHostException e)
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
			response = "UnknownHostException: " + e.toString();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			response = "IOException: " + e.toString();
		} finally {
			if (socket != null) {
				try {
					socket.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}

		//textResponse.setText(response);

		return null;


	}

	@Override
	protected void onPostExecute(Void result) {


		textResponse.setText(response);
		super.onPostExecute(result);


		//dev mode:
		//String devmode;
		//devmode="";

	}

//	protected void onPreExecute(Void result) {
//		textResponse.setText(response);
//		super.onPostExecute(result);
//	}


//	protected void onProgressUpdate(Void result) {
//		textResponse.setText(response);
//		super.onPostExecute(result);
//	}

//
//	protected void onPostExecute(Result result) {
//		throw new RuntimeException("Stub!");
//	}

//	protected void onProgressUpdate(Progress... values) {
//		throw new RuntimeException("Stub!");
//	}



//	protected void ShowResult(String response) {
//		textResponse.setText(response);
//
//	}

}
