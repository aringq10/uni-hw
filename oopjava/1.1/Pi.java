import java.util.Random;
import javax.swing.JOptionPane;

public class Pi
{
    public static void main(String[] args)  
    {
        String arg = JOptionPane.showInputDialog("Enter number of iterations for approximating pi");

        try {
            int iter = Integer.parseInt(arg);
            Random rnd = new Random();
            int total = 0, inside = 0;

            for (int i = 0; i < iter; i++) {
                double x = rnd.nextDouble() * 2;
                double y = rnd.nextDouble() * 2;
                x -= 1;
                y -= 1;
                total++;

                if (Math.pow(x, 2) + Math.pow(y, 2) <= 1) {
                    inside++;
                }
            }

            System.out.println("Approximation of PI: " + (double)inside / (double)total * 4.0);
        }
        catch (NumberFormatException exc) {
            System.out.println("Number of iterations should be an integer");
            System.exit(1);
        }
    }
}

