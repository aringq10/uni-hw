package npc;

public class Officer extends Enemy {
    protected int maxHealth = 125;
    protected int maxArmor = 125;
    protected int armor = maxArmor;
    private static int officerCount = 0;

    public Officer(String firstName, String lastName) {
        super(firstName, lastName);
        officerCount++;
    }

    public Officer(String firstName, String lastName, int x, int y) {
        super(firstName, lastName, x, y);
        officerCount++;
    }

    public int getMaxArmor() { return maxArmor; }

    public int getArmor() { return armor; }

    public static int getOfficerCount() { return officerCount; }

    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        int damageLeft = armor > damage ? 0 : damage - armor;
        armor -= (damage - damageLeft);
        health -= Math.min(health, damageLeft);
    }

    public void setArmor(int armorPoints) {
        if (armorPoints < 0 || armorPoints > maxArmor) return;
        armor = armorPoints;
    }

    public void repairArmor(int armorPoints) {
        if (armorPoints < 0) return;
        setArmor(Math.min(armor + armorPoints, maxArmor));
    }

    public String toString() {
        return super.toString() + " armor: " + getArmor();
    }
}
