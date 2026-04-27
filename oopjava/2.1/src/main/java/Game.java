import npc.Enemy;
import npc.GameException;
import npc.InvalidGameParameterException;
import npc.law.Officer;
import npc.civilian.Citizen;

public class Game {
    public static void main(String[] args) {
        try {
            Enemy c1 = new Citizen("John", "Doe", "Bartender");
            Enemy o1 = new Officer("Jane", "Smith");

            Citizen c2 = new Citizen("as", "ir tu");
            System.out.println(c2.toString());
            c2.move(5, 1);
            System.out.println(c2.toString());

            System.out.println("enemy count: " + Enemy.getCount());
            System.out.println("citizen count: " + Citizen.getCount());
            System.out.println("officer count: " + Officer.getCount());
            System.out.println("-- Enemies:");
            System.out.println(c1.toString());
            System.out.println(o1.toString());

            System.out.println("-- Dealing 50 damage:");
            c1.takeDamage(50);
            o1.takeDamage(50);
            System.out.println(c1.toString());
            System.out.println(o1.toString());

            System.out.println("-- Dealing 100 damage:");
            c1.takeDamage(100);
            o1.takeDamage(100);
            System.out.println(c1.toString());
            System.out.println(o1.toString());

            System.out.println("-- Attempting to damage a dead character:");
            c1.takeDamage(100);

        } catch (InvalidGameParameterException e) {
            System.err.println("Invalid parameter error: " + e.getMessage());
            System.err.println("  Parameter: " + e.getParameterName());
            System.err.println("  Value: " + e.getInvalidValue());
        } catch (GameException e) {
            System.err.println("Game error: " + e.getMessage());
        }

        System.out.println();
        System.out.println("-- Testing invalid parameter exception:");
        try {
            new Citizen("", "Doe");
        } catch (InvalidGameParameterException e) {
            System.err.println("Invalid parameter error: " + e.getMessage());
            System.err.println("  Parameter: " + e.getParameterName());
            System.err.println("  Value: " + e.getInvalidValue());
        } catch (GameException e) {
            System.err.println("Game error: " + e.getMessage());
        }

        System.out.println("-- Testing negative damage exception:");
        try {
            Citizen c3 = new Citizen("Jonas", "Ponas");
            c3.takeDamage(-10);
        } catch (InvalidGameParameterException e) {
            System.err.println("Invalid parameter error: " + e.getMessage());
            System.err.println("  Parameter: " + e.getParameterName());
            System.err.println("  Value: " + e.getInvalidValue());
        } catch (GameException e) {
            System.err.println("Game error: " + e.getMessage());
        }

        System.out.println("-- Testing empty profession exception:");
        try {
            Citizen c4 = new Citizen("Test", "User");
            c4.changeProfession("");
        } catch (InvalidGameParameterException e) {
            System.err.println("Invalid parameter error: " + e.getMessage());
            System.err.println("  Parameter: " + e.getParameterName());
            System.err.println("  Value: " + e.getInvalidValue());
        } catch (GameException e) {
            System.err.println("Game error: " + e.getMessage());
        }
    }
}
