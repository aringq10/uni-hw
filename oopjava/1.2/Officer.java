// https://klevas.mif.vu.lt/~linas1/OP/uzd2.html

public class Officer {
    private int x = 0;
    private int y = 0;
    private int donuts = 0;
    private final int maxHealth = 125;
    private int health = maxHealth;
    private String firstName;
    private String lastName;
    private static int officerCount = 0;

    public Officer() {
        firstName = "";
        lastName = "";
        officerCount++;
    }

    public Officer(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
        officerCount++;
    }

    public Officer(String firstName, String lastName, int x, int y) {
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
        if (health > 0) {
            health -= health > damage ?
                damage : health;
        }
    }

    public void takeDamage(int damage, double multiplier) {
        if (damage < 0 || multiplier < 0) return;

        int totalDamage = (int)(damage * multiplier);
        takeDamage(totalDamage);
    }

    public void heal(int healthPoints) {
        if (healthPoints < 0) return;
        if (health + healthPoints > maxHealth)
            health = maxHealth;
        else
            health += healthPoints;
    }

    public void giveDonut() { donuts++; }

    public void eatDonut() {
        if (donuts > 0) {
            donuts--;
            heal(10);
        }
    }

    public void setFirstName(String firstName) { this.firstName = firstName; }

    public void setLastName(String lastName) { this.lastName = lastName; }

    public void println() {
        System.out.println("Name: " + getFirstName() + " " + getLastName() +
                           " x: " + getX() + " y: " + getY() + " hp: " + getHealth() +
                           " donuts: " + getDonuts());
    }

}
