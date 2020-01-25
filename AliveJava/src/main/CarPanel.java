package main;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.net.URL;

import javax.imageio.ImageIO;
import javax.swing.JOptionPane;
import javax.swing.JPanel;

public class CarPanel extends JPanel{
	Image imgLue = null;
	public CarPanel(int left,int front,int right) {
		URL urlImage = getClass().getClassLoader().getResource("carUpView.png");
		if (urlImage == null) {
			JOptionPane.showMessageDialog(null, "Fichier " + "carUpView.PNG" + " introuvable");
		}
		try {
			imgLue = ImageIO.read(urlImage);
		} catch (IOException e) {
			JOptionPane.showMessageDialog(null, "Erreur pendant la lecture du fichier d'image");
		}
	}
	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g.create();
		g2d.drawLine(0, 0, 50, 50);
		g2d.drawImage(imgLue, 0, 0, null);
		
	}
}
