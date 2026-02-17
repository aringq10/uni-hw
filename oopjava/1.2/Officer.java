// https://klevas.mif.vu.lt/~linas1/OP/uzd2.html

public class Officer {
    private int x = 0;
    private int y = 0;
    private int donuts = 0;
    private int health = 120;
    private int maxHealth = 125;
    private String firstName;
    private String lastName;

    public Officer(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
    }
    public Officer(String firstName, String lastName, int x, int y) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.x = x;
        this.y = y;
    }

    public void println() {
        System.out.println("Name: " + this.firstName + " " + this.lastName);
        System.out.println("Position: " + this.x + " " + this.y);
        System.out.println("Health: " + this.health);
        System.out.println("Donuts left: " + this.donuts);
    }

    public void setX(int x) { this.x = x; }
    public int getX() { return this.x; }
    public void setY(int y) { this.y = y; }
    public int getY() { return this.y; }
    public void giveDonut() { this.donuts++; }
    public void eatDonut() {
        if (this.donuts > 0) {
            this.donuts--;
            heal(10);
        }
    }
    public void takeDamage(int damage) {
        if (this.health > 0) {
            this.health -= this.health > damage ?
                damage : this.health;
        }
    }
    public void heal(int healthPoints) {
        if (this.health + healthPoints > this.maxHealth)
            this.health = this.maxHealth;
        else
            this.health += healthPoints;
    }
    public int getHealth() { return this.health; }
}
