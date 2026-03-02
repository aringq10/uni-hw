package npc.civilian;

import npc.Enemy;

public class Citizen extends Enemy {
    private static int citizenCount = 0;

    private String profession;
    private int reputation;
    private boolean panicking = false;

    public Citizen(String firstName, String lastName) {
        super(firstName, lastName);
        this.profession = "Unemployed";
        this.reputation = 0;
        citizenCount++;
    }

    public Citizen(String firstName, String lastName, String profession) {
        super(firstName, lastName);
        this.profession = profession;
        this.reputation = 0;
        citizenCount++;
    }

    public Citizen(String firstName, String lastName, String profession, int x, int y) {
        super(firstName, lastName, x, y);
        this.profession = profession;
        this.reputation = 0;
        citizenCount++;
    }

    public static int getCitizenCount() {
        return citizenCount;
    }

    public String getProfession() {
        return profession;
    }

    public int getReputation() {
        return reputation;
    }

    public boolean isPanicking() {
        return panicking;
    }

    public void changeReputation(int amount) {
        reputation = Math.max(-100, Math.min(100, reputation + amount));
    }

    public void changeProfession(String proffesion) {
        if (!proffesion.isEmpty()) this.profession = proffesion;
    }

    @Override
    public void takeDamage(int damage) {
        super.takeDamage(damage);
        if (isAlive()) {
            panicking = true;
        }
    }

    public void calmDown() {
        panicking = false;
    }

    @Override
    public String toString() {
        return super.toString() +
               " profession: " + profession +
               " reputation: " + reputation +
               " panicking: " + panicking;
    }
}
