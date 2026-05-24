package npc.law;

import npc.Enemy;

/**
 * Law-enforcement NPC variant. Carries an armor pool that absorbs
 * incoming damage before any of it touches health.
 */
public class Officer extends Enemy {
    private static final long serialVersionUID = 1L;

    protected int armor = 125;

    /**
     * @param firstName given name passed through to {@link Enemy}
     * @param lastName  family name passed through to {@link Enemy}
     */
    public Officer(String firstName, String lastName) {
        super(firstName, lastName);
    }

    /**
     * Armor-aware damage rule: incoming damage drains armor first;
     * only the remainder reaches health. No-op when already dead or
     * when {@code damage} is negative.
     *
     * @param damage non-negative damage amount
     */
    @Override
    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        int absorbed = Math.min(damage, armor);
        armor -= absorbed;
        health -= Math.min(health, damage - absorbed);
    }

    /** {@inheritDoc} */
    @Override
    public String toString() {
        return super.toString() + " armor: " + armor;
    }

    /**
     * Shallow clone — armor is a primitive so the default
     * {@link Enemy#clone()} copy is sufficient.
     *
     * @return a new Officer with a fresh id
     */
    @Override
    public Officer clone() {
        return (Officer) super.clone();
    }
}
