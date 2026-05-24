package npc.civilian;

import java.util.ArrayList;
import java.util.List;

import npc.Enemy;

/**
 * Civilian NPC variant. Adds a {@code profession} label and a mutable
 * {@link ArrayList} inventory of item names.
 */
public class Citizen extends Enemy {
    private static final long serialVersionUID = 1L;

    private String profession;
    private List<String> inventory = new ArrayList<>();

    /**
     * @param firstName  given name passed through to {@link Enemy}
     * @param lastName   family name passed through to {@link Enemy}
     * @param profession free-form profession label
     */
    public Citizen(String firstName, String lastName, String profession) {
        super(firstName, lastName);
        this.profession = profession;
    }

    /**
     * Appends an item to the citizen's inventory.
     *
     * @param item item name; duplicates are allowed (it's a list, not a set)
     */
    public void addItem(String item) {
        inventory.add(item);
    }

    /** {@inheritDoc} */
    @Override
    public String toString() {
        return super.toString() + " prof: " + profession + " inv: " + inventory;
    }

    /**
     * Clones the citizen with a deep-copied inventory list so mutations
     * on the spawned instance don't leak back to the prototype.
     *
     * @return a new Citizen with a fresh id and an independent inventory
     */
    @Override
    public Citizen clone() {
        Citizen copy = (Citizen) super.clone();
        copy.inventory = new ArrayList<>(this.inventory);
        return copy;
    }
}
