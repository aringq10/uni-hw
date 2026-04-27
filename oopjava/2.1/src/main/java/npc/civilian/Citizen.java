package npc.civilian;

import npc.Enemy;
import npc.GameException;
import npc.InvalidGameParameterException;

public class Citizen extends Enemy implements Controllable {
    private static int count = 0;

    private String profession;
    private int reputation;
    private boolean panicking = false;

    public Citizen(String firstName, String lastName) throws InvalidGameParameterException {
        super(firstName, lastName);
        this.profession = "Unemployed";
        this.reputation = 0;
        count++;
    }

    public Citizen(String firstName, String lastName, String profession) throws InvalidGameParameterException {
        super(firstName, lastName);
        if (profession == null || profession.isEmpty()) {
            throw new InvalidGameParameterException("Profession cannot be empty", "profession", profession);
        }
        this.profession = profession;
        this.reputation = 0;
        count++;
    }

    public Citizen(String firstName, String lastName, String profession, int x, int y) throws InvalidGameParameterException {
        super(firstName, lastName, x, y);
        if (profession == null || profession.isEmpty()) {
            throw new InvalidGameParameterException("Profession cannot be empty", "profession", profession);
        }
        this.profession = profession;
        this.reputation = 0;
        count++;
    }

    public static int getCount() { return count; }

    public String getProfession() { return profession; }

    public int getReputation() { return reputation; }

    public boolean isPanicking() { return panicking; }

    public void moveUp() {
        move(0, 1);
    };

    public void moveDown() {
        move(0, -1);
    };

    public void moveLeft() {
        move(-1, 0);
    };

    public void moveRight() {
        move(1, 0);

    };

    public void changeReputation(int amount) {
        reputation = Math.max(-100, Math.min(100, reputation + amount));
    }

    public void changeProfession(String profession) throws InvalidGameParameterException {
        if (profession == null || profession.isEmpty()) {
            throw new InvalidGameParameterException("Profession cannot be empty", "profession", profession);
        }
        this.profession = profession;
    }

    @Override
    public void takeDamage(int damage) throws GameException {
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
               " prof: " + profession +
               " rep: " + reputation +
               " panic: " + panicking;
    }
}
