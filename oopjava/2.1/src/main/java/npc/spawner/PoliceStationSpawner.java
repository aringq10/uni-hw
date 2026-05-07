package npc.spawner;

import npc.Enemy;
import npc.GameException;
import npc.law.Officer;

public class PoliceStationSpawner extends EnemySpawner {
    @Override
    protected Enemy createEnemy(String firstName, String lastName) throws GameException {
        return new Officer(firstName, lastName);
    }

    @Override
    public String getLocationName() { return "Police Station"; }
}
