// https://klevas.mif.vu.lt/~linas1/OP/uzd2.html

public class Officer {
    private int x;
    private int y;
    private int donuts;
    private int health;
    private final int maxHealth = 125;
    private String firstName;
    private String lastName;
    private static int officerCount = 0;

    public Officer() {
        x = 0;
        y = 0;
        donuts = 0;
        health = maxHealth;
        firstName = "";
        lastName = "";
        officerCount++;
    }

    public Officer(String firstName, String lastName) {
        x = 0;
        y = 0;
        donuts = 0;
        health = maxHealth;
        this.firstName = firstName;
        this.lastName = lastName;
        officerCount++;
    }

    public Officer(String firstName, String lastName, int x, int y) {
        donuts = 0;
        health = maxHealth;
        this.firstName = firstName;
        this.lastName = lastName;
        this.x = x;
        this.y = y;
        officerCount++;
    }

    public int getX() { return x; }

    public int getY() { return y; }

    public int getHealth() { return health; }

    public int getDonuts() { return donuts; }

    public String getFirstName() { return firstName; }

    public String getLastName() { return lastName; }

    public static int getOfficerCount() { return officerCount; }

    public void setX(int x) { this.x = x; }

    public void setY(int y) { this.y = y; }

    public void takeDamage(int damage) {
        if (damage < 0) return;
        if (this.health > 0) {
            this.health -= this.health > damage ?
                damage : this.health;
        }
    }

    public void takeDamage(int damage, double multiplier) {
        if (damage < 0 || multiplier < 0) return;

        int totalDamage = (int)(damage * multiplier);
        takeDamage(totalDamage);
    }

    public void heal(int healthPoints) {
        if (healthPoints < 0) return;
        if (this.health + healthPoints > this.maxHealth)
            this.health = this.maxHealth;
        else
            this.health += healthPoints;
    }

    public void giveDonut() { this.donuts++; }

    public void eatDonut() {
        if (this.donuts > 0) {
            this.donuts--;
            heal(10);
        }
    }

    public void setFirstName(String firstName) { this.firstName = firstName; }

    public void setLastName(String lastName) { this.lastName = lastName; }

    public void println() {
        System.out.println("Name: " + this.firstName + " " + this.lastName);
        System.out.println("Position: " + this.x + " " + this.y);
        System.out.println("Health: " + this.health);
        System.out.println("Donuts left: " + this.donuts);
    }
}
