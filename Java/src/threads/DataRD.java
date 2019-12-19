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
		receive();
		receive();
		receive();
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
		String place = "";
		try {
			val = Jsoup.parse(inStream.read()+"").text();
			place = Jsoup.parse(inStream.read()+"").text();
			if(place.equals("G")) {
				tfGauche.setText(val);
			}else if(place.equals("A")) {
				tfMilieu.setText(val);
			}else if(place.equals("D")) {
				tfDroite.setText(val);
			}else {
				System.out.println("VALEUR NON RECONNUE");
			}
			System.out.println(val);
			return val;
		} catch (IOException e) {
			System.out.println("Problem receiving");
			return "Error";
		}
	}
}
