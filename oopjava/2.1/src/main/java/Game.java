import npc.Enemy;
import npc.Movable;
import npc.law.Officer;
import npc.civilian.Citizen;

public class Game {
    public static void main(String[] args) {
        Enemy c1= new Citizen("John", "Doe", "Bartender");
        Enemy o1 = new Officer("Jane", "Smith");

        Movable c2 = new Citizen("as", "ir tu");
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

        System.out.println("-- Dealing 100 damage:");
        c1.takeDamage(100);
        o1.takeDamage(100);
        System.out.println(c1.toString());
        System.out.println(o1.toString());

        Citizen c3 = new Citizen("Jonas", "Ponas");

        System.out.println("-- New citizen, changing rep +10");
        c3.changeReputation(10);
        System.out.println(c3.toString());
    }
}
