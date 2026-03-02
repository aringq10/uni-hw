import npc.Enemy;
import npc.law.Officer;
import npc.civilian.Citizen;

public class Game {
    public static void main(String[] args) {
        Enemy e1 = new Citizen("John", "Doe");
        Enemy e2 = new Officer("Jane", "Smith");
    }
}
