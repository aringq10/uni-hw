package npc;

public class Citizen extends Enemy {
    private static int citizenCount = 0;

    public Citizen(String firstName, String lastName) {
        super(firstName, lastName);
        citizenCount++;
    }

    public Citizen(String firstName, String lastName, int x, int y) {
        super(firstName, lastName, x, y);
        citizenCount++;
    }

    public static int getCitizenCount() { return citizenCount; }
}
