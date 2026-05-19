package npc.io;

import java.io.Serializable;
import java.util.List;

import npc.Enemy;
import npc.EnemyPrototypeRegistry;

public class GameState implements Serializable {
    private static final long serialVersionUID = 1L;

    public final List<Enemy> activeEnemies;
    public final EnemyPrototypeRegistry registry;
    public final long savedAtMillis;

    public GameState(List<Enemy> activeEnemies, EnemyPrototypeRegistry registry) {
        this.activeEnemies = activeEnemies;
        this.registry = registry;
        this.savedAtMillis = System.currentTimeMillis();
    }
}
