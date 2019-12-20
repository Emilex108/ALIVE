package main;

import java.awt.EventQueue;
import java.awt.Graphics2D;

import javax.swing.JFrame;
import javax.swing.JOptionPane;

import java.awt.Image;

import javax.swing.JPanel;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.border.TitledBorder;

import com.fazecast.jSerialComm.SerialPort;

import threads.AutoPilot;
import threads.DataRD;
import utillities.TextAreaOutputStream;

import javax.swing.border.LineBorder;
import java.awt.Color;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintStream;
import java.net.URL;
import java.awt.event.ActionEvent;
import javax.swing.JScrollPane;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JCheckBox;
import java.awt.event.FocusAdapter;
import java.awt.event.FocusEvent;
import javax.swing.JLabel;
import javax.swing.ScrollPaneConstants;

public class Application {

	private JFrame frame;
	private JTextField txtConsolein;
	private static OutputStream outStream;
	private static InputStream inStream;
	private static AutoPilot ap;
	private static JTextField tfGauche;
	private static JTextField tfMilieu;
	private static JTextField tfDroite;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Application window = new Application();
					window.frame.setVisible(true);
					SerialPort sp = SerialPort.getCommPort("com5");
					sp.setComPortParameters(115200, 8, 1, 0);
					sp.setComPortTimeouts(SerialPort.TIMEOUT_WRITE_BLOCKING, 0, 0);

					if(sp.openPort()) {
						System.out.println("Port open");
					}else {
						System.out.println("Port closed");
						return;
					}

					outStream = sp.getOutputStream();
					inStream = sp.getInputStream();

					ap = new AutoPilot(outStream, inStream);
					System.out.println("Ready");
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 */
	public Application() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 631, 662);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(null);

		JPanel panel = new JPanel();
		panel.requestFocusInWindow();
		panel.setBorder(new TitledBorder(new LineBorder(new Color(0, 0, 0), 1, true), "Control", TitledBorder.LEADING, TitledBorder.TOP, null, null));
		panel.setBounds(10, 0, 333, 310);
		frame.getContentPane().add(panel);
		panel.setLayout(null);



		JButton btnUp = new JButton("Up");
		btnUp.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				System.out.println("up");
			}
		});
		btnUp.setBounds(131, 37, 70, 70);
		btnUp.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseEntered(MouseEvent e) {
				associerBoutonAvecImage(btnUp, "Up2.png",0);
			}public void mouseExited(MouseEvent e) {
				associerBoutonAvecImage(btnUp, "Up.png",0);
			}
			public void mousePressed(MouseEvent e) {
				associerBoutonAvecImage(btnUp, "Up.png",0);
				send(3);
			}public void mouseReleased(MouseEvent e) {
				associerBoutonAvecImage(btnUp, "Up2.png",0);
				send(0);
			}
		});
		panel.add(btnUp);

		JButton btnDown = new JButton("Down");
		btnDown.requestFocusInWindow();
		btnDown.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		btnDown.setBounds(131, 201, 70, 70);
		btnDown.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseEntered(MouseEvent e) {
				associerBoutonAvecImage(btnDown, "Up2.png",2);
			}public void mouseExited(MouseEvent e) {
				associerBoutonAvecImage(btnDown, "Up.png",2);
			}
			public void mousePressed(MouseEvent e) {
				associerBoutonAvecImage(btnDown, "Up.png",2);
				send(1);
			}public void mouseReleased(MouseEvent e) {
				associerBoutonAvecImage(btnDown, "Up2.png",2);
				send(0);
			}
		});
		panel.add(btnDown);

		JButton btnRight = new JButton("Right");
		btnRight.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		btnRight.setBounds(219, 120, 70, 70);
		btnRight.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseEntered(MouseEvent e) {
				associerBoutonAvecImage(btnRight, "Up2.png",1);
			}public void mouseExited(MouseEvent e) {
				associerBoutonAvecImage(btnRight, "Up.png",1);
			}
			public void mousePressed(MouseEvent e) {
				associerBoutonAvecImage(btnRight, "Up.png",1);
				send(2);
			}public void mouseReleased(MouseEvent e) {
				associerBoutonAvecImage(btnRight, "Up2.png",1);
				send(0);
			}
		});
		panel.add(btnRight);

		JButton btnLeft = new JButton("Left");
		btnLeft.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
			}
		});
		btnLeft.setBounds(43, 120, 70, 70);
		btnLeft.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseEntered(MouseEvent e) {
				associerBoutonAvecImage(btnLeft, "Up2.png",3);
			}public void mouseExited(MouseEvent e) {
				associerBoutonAvecImage(btnLeft, "Up.png",3);
			}
			public void mousePressed(MouseEvent e) {
				associerBoutonAvecImage(btnLeft, "Up.png",3);
				send(4);
			}public void mouseReleased(MouseEvent e) {
				associerBoutonAvecImage(btnLeft, "Up2.png",3);
				send(0);
			}
		});
		panel.add(btnLeft);


		associerBoutonAvecImage(btnUp, "Up.png",0);
		associerBoutonAvecImage(btnRight, "Up.png",1);
		associerBoutonAvecImage(btnDown, "Up.png",2);
		associerBoutonAvecImage(btnLeft, "Up.png",3);
		
		JCheckBox chckbxModeClavier = new JCheckBox("Keyboard control mode");
		chckbxModeClavier.setSelected(false);
		chckbxModeClavier.addFocusListener(new FocusAdapter() {
			@Override
			public void focusLost(FocusEvent e) {
				chckbxModeClavier.setSelected(false);
			}
		});
		chckbxModeClavier.addKeyListener(new KeyAdapter() {
			@Override
			public void keyPressed(KeyEvent e) {
				int c = e.getKeyCode ();
				if (c==KeyEvent.VK_UP) {
					send(3);
				} else if (c==KeyEvent.VK_RIGHT) { 
					send(2);
				} else if (c==KeyEvent.VK_DOWN) { 
					send(1);
				} else if (c==KeyEvent.VK_LEFT) { 
					send(4);
				}
			}
			@Override
			public void keyReleased(KeyEvent e) {
				send(0);
				
			}
		});
		chckbxModeClavier.setBounds(6, 280, 146, 23);
		panel.add(chckbxModeClavier);


		JPanel panel_1 = new JPanel();
		panel_1.setBounds(0, 321, 353, 33);
		frame.getContentPane().add(panel_1);

		JButton btnAutopilotmode = new JButton("Activate Auto-pilot");
		btnAutopilotmode.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				ap.start();
			}
		});
		panel_1.add(btnAutopilotmode);

		JPanel panel_2 = new JPanel();
		panel_2.setBounds(0, 365, 609, 247);
		frame.getContentPane().add(panel_2);
		panel_2.setLayout(null);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 5, 589, 209);
		panel_2.add(scrollPane);

		JTextArea txtrConsoleout = new JTextArea();
		scrollPane.setViewportView(txtrConsoleout);
		txtrConsoleout.setEditable(false);
		txtrConsoleout.setBackground(Color.WHITE);

		PrintStream out = new PrintStream( new TextAreaOutputStream( txtrConsoleout ) );

		// redirect standard output stream to the TextAreaOutputStream
		System.setOut( out );

		// redirect standard error stream to the TextAreaOutputStream
		System.setErr( out );

		txtConsolein = new JTextField();
		txtConsolein.addKeyListener(new KeyAdapter() {
			@Override
			public void keyTyped(KeyEvent e) {
				if (e.getKeyChar() == e.VK_ENTER) {
					System.out.println(txtConsolein.getText());
					txtConsolein.setText("");
				}
			}
		});
		txtConsolein.setBounds(10, 225, 500, 20);
		panel_2.add(txtConsolein);
		txtConsolein.setColumns(10);

		JButton btnSend = new JButton("Send");
		btnSend.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				System.out.println(txtConsolein.getText());
				txtConsolein.setText("");
			}
		});
		btnSend.setBounds(520, 225, 79, 23);
		panel_2.add(btnSend);
		
		tfGauche = new JTextField();
		tfGauche.setEditable(false);
		tfGauche.setBounds(353, 11, 96, 20);
		frame.getContentPane().add(tfGauche);
		tfGauche.setColumns(10);
		
		tfMilieu = new JTextField();
		tfMilieu.setEditable(false);
		tfMilieu.setBounds(353, 42, 96, 20);
		frame.getContentPane().add(tfMilieu);
		tfMilieu.setColumns(10);
		
		tfDroite = new JTextField();
		tfDroite.setEditable(false);
		tfDroite.setBounds(353, 73, 96, 20);
		frame.getContentPane().add(tfDroite);
		tfDroite.setColumns(10);
		
		JLabel lblGauche = new JLabel("Gauche");
		lblGauche.setBounds(459, 14, 48, 14);
		frame.getContentPane().add(lblGauche);
		
		JLabel lblMilieu = new JLabel("Milieu");
		lblMilieu.setBounds(459, 45, 48, 14);
		frame.getContentPane().add(lblMilieu);
		
		JLabel lblDroite = new JLabel("Droite");
		lblDroite.setBounds(459, 76, 48, 14);
		frame.getContentPane().add(lblDroite);
		
		JButton btnReceive = new JButton("Receive");
		btnReceive.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				DataRD dataRD = new DataRD(outStream, inStream, tfGauche, tfMilieu, tfDroite);
				dataRD.start();
			}
		});
		btnReceive.setBounds(353, 104, 89, 23);
		frame.getContentPane().add(btnReceive);
	}
	public void associerBoutonAvecImage(JButton leBouton, String fichierImage,int nbRotation) {
		Image imgLue = null;
		URL urlImage = getClass().getClassLoader().getResource(fichierImage);
		if (urlImage == null) {
			JOptionPane.showMessageDialog(null, "Fichier " + fichierImage + " introuvable");
		}
		try {
			imgLue = ImageIO.read(urlImage);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Erreur pendant la lecture du fichier d'image");
		}

		// redimensionner l'image de la même grandeur que le bouton
		Image imgRedim = imgLue.getScaledInstance(leBouton.getWidth(), leBouton.getHeight(), Image.SCALE_SMOOTH);
		BufferedImage img = toBufferedImage(imgRedim);
		for(int i = 0;i<nbRotation;i++) {
			img = rotateCw(img);
		}
		// au cas où le fond de l’image serait transparent
		leBouton.setOpaque(false);
		leBouton.setContentAreaFilled(false);
		leBouton.setBorderPainted(false);

		// associer l'image au bouton
		leBouton.setText("");
		leBouton.setIcon(new ImageIcon(img));

		// on se débarrasse des images intermédiaires
		imgLue.flush();
		imgRedim.flush();
	}
	public BufferedImage rotateCw( BufferedImage img )
	{
		int         width  = img.getWidth();
		int         height = img.getHeight();
		BufferedImage   newImage = new BufferedImage( height, width, img.getType() );

		for( int i=0 ; i < width ; i++ )
			for( int j=0 ; j < height ; j++ )
				newImage.setRGB( height-1-j, i, img.getRGB(i,j) );

		return newImage;
	}
	public static BufferedImage toBufferedImage(Image img)
	{
		if (img instanceof BufferedImage)
		{
			return (BufferedImage) img;
		}

		// Create a buffered image with transparency
		BufferedImage bimage = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);

		// Draw the image on to the buffered image
		Graphics2D bGr = bimage.createGraphics();
		bGr.drawImage(img, 0, 0, null);
		bGr.dispose();

		// Return the buffered image
		return bimage;
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
				System.out.println(val);
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
	public void await(int i) {
		try {
			Thread.sleep(i);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	public static OutputStream getOutStream() {
		return outStream;
	}

	public static void setOutStream(OutputStream outStream) {
		Application.outStream = outStream;
	}

	public static InputStream getInStream() {
		return inStream;
	}

	public static void setInStream(InputStream inStream) {
		Application.inStream = inStream;
	}

	public static JTextField getTfGauche() {
		return tfGauche;
	}

	public static void setTfGauche(JTextField tfGauche) {
		Application.tfGauche = tfGauche;
	}

	public static JTextField getTfMilieu() {
		return tfMilieu;
	}

	public static void setTfMilieu(JTextField tfMilieu) {
		Application.tfMilieu = tfMilieu;
	}

	public static JTextField getTfDroite() {
		return tfDroite;
	}

	public static void setTfDroite(JTextField tfDroite) {
		Application.tfDroite = tfDroite;
	}

	
}
