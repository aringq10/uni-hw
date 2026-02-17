import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.*;


public class graph extends JPanel {
    Color dark = new Color(91, 183, 84);
    Color light = new Color(255, 255, 255);

    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.setSize(800, 800);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(new graph());

        frame.setVisible(true);
    }
    public void paintComponent (Graphics g) {

        int xCoord = 0;
        int yCoord = 0;
        int squareSize = super.getWidth() > super.getHeight() ? super.getHeight() / 8 : super.getWidth() / 8;
        super.paintComponent(g);
        g.setColor(light);
        for (int j = 1 ; j <= 8 ; j++) {
            for (int i = 1 ; i <= 8 ; i++) {
                if (g.getColor() == dark) {
                    g.setColor(light);
                } else {
                    g.setColor(dark);
                }

                g.fillRect(xCoord, yCoord, squareSize, squareSize);
                xCoord += squareSize;
            }
            xCoord = 0;
            yCoord += squareSize;
            if (g.getColor() == dark) {
                    g.setColor(light);
                } else {
                    g.setColor(dark);
                }
        }
    }



}
