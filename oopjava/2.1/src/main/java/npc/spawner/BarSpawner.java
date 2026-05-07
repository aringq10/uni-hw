package npc.spawner;

import npc.Enemy;
import npc.GameException;
import npc.civilian.Citizen;

public class BarSpawner extends EnemySpawner {
    @Override
    protected Enemy createEnemy(String firstName, String lastName) throws GameException {
        return new Citizen(firstName, lastName, "Bartender");
    }

    @Override
    public String getLocationName() { return "Bar"; }
}
