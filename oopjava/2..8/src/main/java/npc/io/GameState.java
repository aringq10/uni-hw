package npc.io;

import java.io.Serializable;
import java.util.List;

import npc.Enemy;
import npc.EnemyPrototypeRegistry;

/**
 * Immutable snapshot of the game's world state. Plays the <b>Memento</b>
 * role: bundles the active enemies and the prototype registry so
 * {@link StateManager} can hand the whole picture to disk and back.
 */
public class GameState implements Serializable {
    private static final long serialVersionUID = 1L;

    /** Active enemies at snapshot time. */
    public final List<Enemy> activeEnemies;

    /** Prototype registry contents at snapshot time. */
    public final EnemyPrototypeRegistry registry;

    /**
     * @param activeEnemies enemies currently alive in the game
     * @param registry      prototype registry to serialize alongside them
     */
    public GameState(List<Enemy> activeEnemies, EnemyPrototypeRegistry registry) {
        this.activeEnemies = activeEnemies;
        this.registry = registry;
    }
}
