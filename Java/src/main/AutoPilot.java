package main;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class AutoPilot extends Thread{
	
	private OutputStream outStream;
	private InputStream inStream;
	public AutoPilot(OutputStream outStream, InputStream inStream) {
		this.inStream = inStream;
		this.outStream = outStream;
	}
	
	public void run() {
		
		while(true) {
			send(5);
		try {
		System.out.println(receive());
		}catch(Exception e) {
			System.out.println("Nothing to read");
			}
		}
	}
	
	public void send(int x){
		try {
			outStream.write(x);
			outStream.flush();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Problem writing");
		}

	}
	
	public int receive() {
		int val = 0;
		String temp = "";
		try {
			while(val != 40){
				val = inStream.read()-48;
				if(val != 40) {
				temp += val+"";
				}
			}
			return Integer.parseInt(temp);
		} catch (IOException e) {
			System.out.println("Problem receiving");
			return 999999999;
		}
	}
	
	
	public void delay(long millis) {
		try {
			Thread.sleep(millis);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	
}
