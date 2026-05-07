package npc.spawner;

import npc.Enemy;
import npc.GameException;
import npc.civilian.Citizen;

public class MarketSpawner extends EnemySpawner {
    @Override
    protected Enemy createEnemy(String firstName, String lastName) throws GameException {
        return new Citizen(firstName, lastName, "Shopper");
    }

    @Override
    public String getLocationName() { return "Market"; }
}
