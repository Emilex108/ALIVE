package threads;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.jsoup.Jsoup;

public class AutoPilot extends Thread {

	private OutputStream outStream;
	private InputStream inStream;
	private String gData, aData, dData = "10";
	private boolean gCollision, aCollision, dCollision;
	private final int DETECTION = 8;

	public AutoPilot(OutputStream outStream, InputStream inStream) {
		this.inStream = inStream;
		this.outStream = outStream;
	}

	public void run() {
		send(5);
		while (true) {
			delay(100);
			receive();
			receive();
			receive();
			gCollision = checkgCollision();
			aCollision = checkaCollision();
			dCollision = checkdCollision();
			if (dCollision && aCollision && gCollision) {
				send(3);
			} else if (gCollision && aCollision && !dCollision) {
				send(2);
			} else if (!gCollision && aCollision && dCollision) {
				send(4);
			} else if (gCollision && !aCollision && dCollision) {
				send(1);
			} else if (!gCollision && !aCollision && dCollision) {
				send(4);
			} else if (gCollision && !aCollision && !dCollision) {
				send(2);
			} else if (!gCollision && !aCollision && !dCollision) {
				send(1);
			} else if (!gCollision && aCollision && !dCollision) {
				if (Integer.parseInt(dData) < Integer.parseInt(gData)) {
					send(4);
				} else if (Integer.parseInt(gData) < Integer.parseInt(dData)) {
					send(2);
				} else {
					send(3);
				}
			}
		}
	}

	public void send(int x) {
		try {
			outStream.write(x);
			outStream.flush();
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Problem writing");
		}

	}

	// TODO : Lire '<' puis chiffre puis '>' (Comprendre ce que Jsoup.parse a tous les niveaux
	
	public String receive(){
		String val = "";
		try {
			val = Jsoup.parse(inStream.read()+ "").text();
		} catch (IOException e) {
			e.printStackTrace();
		}
		System.out.println(val);
		return val;
		/*String val = "";
		try {
			val = Jsoup.parse(inStream.read() + "").text();
			
			place = Jsoup.parse(inStream.read() + "").text();
			if(!(place == null)) {
				placeInt = Integer.parseInt(place);
			}
			if (placeInt == 71) {
				gData = val;
			} else if (placeInt == 65) {
				aData = val;
			} else if (placeInt == 68) {
				dData = val;
			} else {
				System.out.println("VALEUR NON RECONNUE : " + place);
			}
			System.out.println(val);
			return val;
		} catch (IOException e) {
			System.out.println("Problem receiving");
			return "Error";
		}*/
	}

	private boolean checkgCollision() {
		if (!(gData == null)) {
			if (Integer.parseInt(gData) < DETECTION) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	private boolean checkaCollision() {
		if (!(aData == null)) {
			if (Integer.parseInt(aData) < DETECTION) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}

	private boolean checkdCollision() {
		if (!(dData == null)) {
			if (Integer.parseInt(dData) < DETECTION) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
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
