package npc;

import java.io.Serializable;

/**
 * Abstract base class for every NPC in the game. Holds shared state
 * (position, health, name, unique id) and the default damage/movement
 * behaviour that subclasses can override.
 *
 * <p>Acts as the <b>Prototype</b> in the Prototype pattern: implements
 * {@link Cloneable} so {@link EnemyPrototypeRegistry} can stamp out new
 * instances from a registered template via {@link #clone()}.
 */
public abstract class Enemy implements Cloneable, Serializable {
    private static final long serialVersionUID = 1L;
    private static int nextId = 0;

    protected int x = 0;
    protected int y = 0;
    protected int health = 100;
    protected String firstName;
    protected String lastName;
    private int id;

    /**
     * Creates an enemy at the origin with full health and a freshly
     * assigned unique id.
     *
     * @param firstName given name shown in {@link #toString()}
     * @param lastName  family name shown in {@link #toString()}
     */
    public Enemy(String firstName, String lastName) {
        this.firstName = firstName;
        this.lastName = lastName;
        this.id = nextId++;
    }

    /** @return {@code true} while {@code health > 0}. */
    public boolean isAlive() { return health > 0; }

    /** @return the unique id assigned at construction (or clone) time. */
    public int getId() { return id; }

    /**
     * Translates the enemy by the given delta.
     *
     * @param dx change applied to {@code x}
     * @param dy change applied to {@code y}
     */
    public void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    /**
     * Default damage handling: ignored when already dead or when
     * {@code damage} is negative; otherwise subtracts up to the
     * remaining health. Subclasses (e.g. {@link npc.law.Officer}) may
     * override to add absorption layers.
     *
     * @param damage non-negative damage amount
     */
    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        health -= Math.min(damage, health);
    }

    /** {@inheritDoc} */
    @Override
    public String toString() {
        return "#" + id + " " + firstName + " " + lastName
            + " x: " + x + " y: " + y + " hp: " + health;
    }

    /**
     * Shallow clone with a fresh id from the static counter so cloned
     * instances stay addressable independently of the prototype.
     *
     * @return a new Enemy that is a shallow copy of {@code this} with a
     *         newly assigned id
     */
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
