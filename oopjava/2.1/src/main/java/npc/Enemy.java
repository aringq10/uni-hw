package npc;

public abstract class Enemy implements Movable {
    protected boolean alive = true;
    protected int x = 0;
    protected int y = 0;
    protected int maxHealth = 100;
    protected int health = maxHealth;
    protected String firstName;
    protected String lastName;

    private int id;
    private static int nextId = 0;
    private static int count = 0;

    public Enemy(String firstName, String lastName) throws InvalidGameParameterException {
        if (firstName == null || firstName.isEmpty()) {
            throw new InvalidGameParameterException("First name cannot be empty", "firstName", firstName);
        }
        if (lastName == null || lastName.isEmpty()) {
            throw new InvalidGameParameterException("Last name cannot be empty", "lastName", lastName);
        }
        this.firstName = firstName;
        this.lastName = lastName;
        id = nextId;
        nextId++; count++;
    }

    public Enemy(String firstName, String lastName, int x, int y) throws InvalidGameParameterException {
        if (firstName == null || firstName.isEmpty()) {
            throw new InvalidGameParameterException("First name cannot be empty", "firstName", firstName);
        }
        if (lastName == null || lastName.isEmpty()) {
            throw new InvalidGameParameterException("Last name cannot be empty", "lastName", lastName);
        }
        this.firstName = firstName;
        this.lastName = lastName;
        this.x = x;
        this.y = y;
        id = nextId;
        nextId++; count++;
    }

    public boolean isAlive() { return health > 0; }

    public int getX() { return x; }

    public int getY() { return y; }

    public int getMaxHealth() { return maxHealth; }

    public int getHealth() { return health; }

    public String getFirstName() { return firstName; }

    public String getLastName() { return lastName; }

    public int getId() { return id; }

    public static int getCount() { return count; }

    public void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    public void setPosition(int x, int y) {
        this.x = x;
        this.y = y;
    }

    public void setHealth(int healthPoints) throws InvalidGameParameterException {
        if (healthPoints < 0 || healthPoints > maxHealth) {
            throw new InvalidGameParameterException(
                "Health must be between 0 and " + maxHealth, "healthPoints", healthPoints);
        }
        health = healthPoints;
    }

    public void heal(int healthPoints) throws GameException {
        if (!isAlive()) {
            throw new GameException("Cannot heal a dead character");
        }
        if (healthPoints < 0) {
            throw new InvalidGameParameterException("Heal amount cannot be negative", "healthPoints", healthPoints);
        }
        setHealth(Math.min(health + healthPoints, maxHealth));
    }

    public void takeDamage(int damage) throws GameException {
        if (!isAlive()) {
            throw new GameException("Cannot damage a dead character");
        }
        if (damage < 0) {
            throw new InvalidGameParameterException("Damage cannot be negative", "damage", damage);
        }
        health -= Math.min(damage, health);
    }

    public final void takeDamage(int damage, double multiplier) throws GameException {
        if (!isAlive()) {
            throw new GameException("Cannot damage a dead character");
        }
        if (damage < 0) {
            throw new InvalidGameParameterException("Damage cannot be negative", "damage", damage);
        }
        if (multiplier < 0) {
            throw new InvalidGameParameterException("Multiplier cannot be negative", "multiplier", multiplier);
        }
        int totalDamage = (int)(damage * multiplier);
        takeDamage(totalDamage);
    }

    public String toString() {
        return getFirstName() + " " + getLastName() +
               " x: " + getX() + " y: " + getY() + " hp: " + getHealth();
    }
}
