package npc.law;

import npc.Enemy;

public class Officer extends Enemy {
    private static final long serialVersionUID = 1L;

    protected int armor = 125;

    public Officer(String firstName, String lastName) {
        super(firstName, lastName);
    }

    @Override
    public void takeDamage(int damage) {
        if (!isAlive() || damage < 0) return;
        int absorbed = Math.min(damage, armor);
        armor -= absorbed;
        health -= Math.min(health, damage - absorbed);
    }

    @Override
    public String toString() {
        return super.toString() + " armor: " + armor;
    }

    @Override
    public Officer clone() {
        return (Officer) super.clone();
    }
}
