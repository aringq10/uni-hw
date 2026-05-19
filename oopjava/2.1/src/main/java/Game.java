import java.nio.file.Path;
import java.util.List;
import java.util.concurrent.ExecutionException;

import npc.Enemy;
import npc.EnemyPrototypeRegistry;
import npc.civilian.Citizen;
import npc.io.GameState;
import npc.io.StateManager;
import npc.law.Officer;

public class Game {
    public static void main(String[] args) {
        Citizen alice = new Citizen("Alice", "A", "Bartender");
        alice.addItem("towel");
        alice.move(3, 2);

        Officer bob = new Officer("Bob", "B");
        bob.takeDamage(40);

        EnemyPrototypeRegistry registry = new EnemyPrototypeRegistry();
        registry.register("citizen", alice);
        registry.register("officer", bob);
        Enemy clone = registry.spawn("citizen");

        GameState state = new GameState(List.of(alice, bob, clone), registry);

        StateManager sm = new StateManager();
        Path path = Path.of("save.bin");
        try {
            sm.saveAsync(path, state).get();

            GameState loaded = sm.loadAsync(path).get();
            System.out.println("[main] loaded " + loaded.activeEnemies.size()
                + " enemies, registry size=" + loaded.registry.size()
                + ", savedAt=" + loaded.savedAtMillis);
            for (Enemy e : loaded.activeEnemies) System.out.println("  " + e);

            sm.loadAsync(path, gs ->
                System.out.println("[callback on " + Thread.currentThread().getName()
                    + "] received " + gs.activeEnemies.size() + " enemies")).get();
            sm.drainPending();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.err.println("Save/load interrupted: " + e.getMessage());
        } catch (ExecutionException e) {
            System.err.println("Save/load failed: " + e.getCause());
        } finally {
            sm.shutdown();
        }
    }
}
