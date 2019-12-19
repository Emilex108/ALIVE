package main;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import javax.swing.JTextField;

import org.jsoup.Jsoup;

public class DataRD extends Thread{
	private OutputStream outStream;
	private InputStream inStream;
	private JTextField tfGauche;
	private JTextField tfMilieu;
	private JTextField tfDroite;
	
	public DataRD(OutputStream outStream, InputStream inStream, JTextField tfGauche, JTextField tfMilieu, JTextField tfDroite) {
		this.outStream = outStream;
		this.inStream = inStream;
		this.tfGauche = tfGauche;
		this.tfMilieu = tfMilieu;
		this.tfDroite = tfDroite;
	}
	
	public void run() {
		while(true) {
		send(6);
		await(200);
		tfDroite.setText(receive());
		tfMilieu.setText(receive());
		tfGauche.setText(receive());
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
	public void await(int i) {
		try {
			Thread.sleep(i);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
	public String receive() {
		String val = "";
		String temp = "";
		try {
			while(!val.equals("X")){
				val = Jsoup.parse(inStream.read()+"").text();
				System.out.println(val);
				if(!val.equals("X")) {
					temp += val;
				}
			}
			return temp;
		} catch (IOException e) {
			System.out.println("Problem receiving");
			return "Error";
		}
	}
}
