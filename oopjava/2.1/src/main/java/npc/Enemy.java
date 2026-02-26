package npc;

public class Enemy {
    protected boolean alive = true;
    protected int x = 0;
    protected int y = 0;
    protected int speed = 5;
    protected int maxHealth = 100;
    protected int health = maxHealth;
    protected String firstName;
    protected String lastName;

    private int id;
    private static int nextId = 0;
    private static int enemyCount = 0;

    public Enemy(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
        id = nextId;
        nextId++; enemyCount++;
    }

    public Enemy(String firstName, String lastName, int x, int y) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.x = x;
        this.y = y;
        id = nextId;
        nextId++; enemyCount++;
    }

    public boolean isAlive() { return health > 0; }

    public int getX() { return x; }

    public int getY() { return y; }

    public int getMaxHealth() { return maxHealth; }

    public int getHealth() { return health; }

    public String getFirstName() { return firstName; }

    public String getLastName() { return lastName; }

    public int getId() { return id; }

    public static int getEnemyCount() { return enemyCount; }

    public void setX(int x) { this.x = x; }

    public void setY(int y) { this.y = y; }

    public void setFirstName(String firstName) {
        if (firstName == "".strip()) return;
        this.firstName = firstName;
    }

    public void setLastName(String lastName) {
        if (lastName == "".strip()) return;
        this.lastName = lastName;
    }

    public void setHealth(int healthPoints) {
        if (healthPoints < 0 || healthPoints > maxHealth) return;
        health = healthPoints;
    }

    public void heal(int healthPoints) {
        if (!isAlive() || healthPoints < 0) return;
        setHealth(Math.min(health + healthPoints, maxHealth));
    }

    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        health -= Math.min(damage, health);
    }

    public void takeDamage(int damage, double multiplier) {
        if (!isAlive() || damage < 0 || multiplier < 0) return;

        int totalDamage = (int)(damage * multiplier);
        takeDamage(totalDamage);
    }

    public String toString() {
        return "Name: " + getFirstName() + " " + getLastName() +
               " x: " + getX() + " y: " + getY() + " hp: " + getHealth();
    }
}
