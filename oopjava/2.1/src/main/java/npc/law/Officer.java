package npc.law;

import npc.Enemy;
import npc.GameException;
import npc.InvalidGameParameterException;

public class Officer extends Enemy {
    protected int maxHealth = 125;
    protected int maxArmor = 125;
    protected int armor = maxArmor;
    private static int count = 0;

    public Officer(String firstName, String lastName) throws InvalidGameParameterException {
        super(firstName, lastName);
        count++;
    }

    public Officer(String firstName, String lastName, int x, int y) throws InvalidGameParameterException {
        super(firstName, lastName, x, y);
        count++;
    }

    public int getMaxArmor() { return maxArmor; }

    public int getArmor() { return armor; }

    public static int getCount() { return count; }

    @Override
    public void takeDamage(int damage) throws GameException {
        if (!isAlive()) {
            throw new GameException("Cannot damage a dead character");
        }
        if (damage < 0) {
            throw new InvalidGameParameterException("Damage cannot be negative", "damage", damage);
        }
        int damageLeft = armor > damage ? 0 : damage - armor;
        armor -= (damage - damageLeft);
        health -= Math.min(health, damageLeft);
    }

    public void setArmor(int armorPoints) throws InvalidGameParameterException {
        if (armorPoints < 0 || armorPoints > maxArmor) {
            throw new InvalidGameParameterException(
                "Armor must be between 0 and " + maxArmor, "armorPoints", armorPoints);
        }
        armor = armorPoints;
    }

    public void repairArmor(int armorPoints) throws GameException {
        if (armorPoints < 0) {
            throw new InvalidGameParameterException("Repair amount cannot be negative", "armorPoints", armorPoints);
        }
        setArmor(Math.min(armor + armorPoints, maxArmor));
    }

    @Override
    public String toString() {
        return super.toString() + " armor: " + getArmor();
    }
}
