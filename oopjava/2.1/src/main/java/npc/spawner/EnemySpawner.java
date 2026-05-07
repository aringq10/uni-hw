package npc.spawner;

import npc.Enemy;
import npc.GameException;

public abstract class EnemySpawner {
    private int spawnedCount = 0;

    protected abstract Enemy createEnemy(String firstName, String lastName) throws GameException;

    public abstract String getLocationName();

    public Enemy spawn(String firstName, String lastName, int x, int y) throws GameException {
        Enemy e = createEnemy(firstName, lastName);
        e.setPosition(x, y);
        spawnedCount++;
        System.out.println("[" + getLocationName() + "] spawned " + e.getFirstName()
            + " " + e.getLastName() + " at (" + x + "," + y + ")");
        return e;
    }

    public int getSpawnedCount() { return spawnedCount; }
}
