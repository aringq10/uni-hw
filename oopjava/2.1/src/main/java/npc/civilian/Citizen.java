package npc.civilian;

import java.util.ArrayList;
import java.util.List;

import npc.Enemy;

public class Citizen extends Enemy {
    private static final long serialVersionUID = 1L;

    private String profession;
    private List<String> inventory = new ArrayList<>();

    public Citizen(String firstName, String lastName, String profession) {
        super(firstName, lastName);
        this.profession = profession;
    }

    public void addItem(String item) {
        inventory.add(item);
    }

    @Override
    public String toString() {
        return super.toString() + " prof: " + profession + " inv: " + inventory;
    }

    @Override
    public Citizen clone() {
        Citizen copy = (Citizen) super.clone();
        copy.inventory = new ArrayList<>(this.inventory);
        return copy;
    }
}
