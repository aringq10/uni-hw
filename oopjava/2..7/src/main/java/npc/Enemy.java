package npc;

import java.io.Serializable;

public abstract class Enemy implements Cloneable, Serializable {
    private static final long serialVersionUID = 1L;
    private static int nextId = 0;

    protected int x = 0;
    protected int y = 0;
    protected int health = 100;
    protected String firstName;
    protected String lastName;
    private int id;

    public Enemy(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.id = nextId++;
    }

    public boolean isAlive() { return health > 0; }
    public int getId() { return id; }

    public void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        health -= Math.min(damage, health);
    }

    @Override
    public String toString() {
        return "#" + id + " " + firstName + " " + lastName
            + " x: " + x + " y: " + y + " hp: " + health;
    }

    @Override
    public Enemy clone() {
        try {
            Enemy copy = (Enemy) super.clone();
            copy.id = nextId++;
            return copy;
        } catch (CloneNotSupportedException e) {
            throw new AssertionError(e);
        }
    }
}
