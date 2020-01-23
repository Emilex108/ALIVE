package threads;

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
	private String val = "";
	
	public DataRD(OutputStream outStream, InputStream inStream, JTextField tfGauche, JTextField tfMilieu, JTextField tfDroite) {
		this.outStream = outStream;
		this.inStream = inStream;
		this.tfGauche = tfGauche;
		this.tfMilieu = tfMilieu;
		this.tfDroite = tfDroite;
	}
	
	public void run() {
		send(5);
		await(20);
		while(true) {
			System.out.println(receive());
			await(30);
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
		String tempval = "";
		try {
			tempval = Jsoup.parse(inStream.read()+"").text();
			return tempval;
		} catch (IOException e) {
			System.out.println("Problem receiving");
			return "Error";
		}
	}
}
