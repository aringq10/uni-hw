import npc.Enemy;
import npc.EnemyPrototypeRegistry;
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

        System.out.println();
        System.out.println("== Prototype pattern demo ==");
        try {
            Citizen bartenderTemplate = new Citizen("Generic", "Bartender", "Bartender");
            bartenderTemplate.changeReputation(20);
            bartenderTemplate.addItem("towel");
            bartenderTemplate.addItem("keys");

            Officer officerTemplate = new Officer("Generic", "Officer");

            EnemyPrototypeRegistry registry = new EnemyPrototypeRegistry();
            registry.register("bartender", bartenderTemplate);
            registry.register("officer", officerTemplate);

            System.out.println("-- Spawning two bartenders and two officers from registry:");
            Citizen bar1 = (Citizen) registry.spawn("bartender");
            Citizen bar2 = (Citizen) registry.spawn("bartender");
            Officer off1 = (Officer) registry.spawn("officer");
            Officer off2 = (Officer) registry.spawn("officer");

            bar1.changeProfession("Barista");
            bar1.addItem("knife");
            bar1.moveRight();
            bar1.takeDamage(30);

            off1.takeDamage(80);
            off2.move(10, -3);

            System.out.println("template: " + bartenderTemplate);
            System.out.println("bar1   : " + bar1);
            System.out.println("bar2   : " + bar2);
            System.out.println("template: " + officerTemplate);
            System.out.println("off1   : " + off1);
            System.out.println("off2   : " + off2);

            System.out.println("-- Independence checks:");
            System.out.println("bar1 inventory size = " + bar1.getInventory().size()
                + ", bar2 inventory size = " + bar2.getInventory().size()
                + ", template inventory size = " + bartenderTemplate.getInventory().size());
            System.out.println("bar1 id != bar2 id: " + (bar1.getId() != bar2.getId()));
            System.out.println("off1 armor=" + off1.getArmor() + " hp=" + off1.getHealth()
                + " | off2 armor=" + off2.getArmor() + " hp=" + off2.getHealth());

            System.out.println("-- Counts after spawning:");
            System.out.println("enemy count: " + Enemy.getCount());
            System.out.println("citizen count: " + Citizen.getCount());
            System.out.println("officer count: " + Officer.getCount());
        } catch (InvalidGameParameterException e) {
            System.err.println("Invalid parameter error: " + e.getMessage());
            System.err.println("  Parameter: " + e.getParameterName());
            System.err.println("  Value: " + e.getInvalidValue());
        } catch (GameException e) {
            System.err.println("Game error: " + e.getMessage());
        }
    }
}
