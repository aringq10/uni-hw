package npc.io;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

public class StateManager {
    private final ExecutorService executor;
    private final Queue<Runnable> pendingMainThreadCallbacks = new ConcurrentLinkedQueue<>();

    public StateManager() {
        ThreadFactory daemonFactory = r -> {
            Thread t = new Thread(r, "state-io-worker");
            t.setDaemon(true);
            return t;
        };
        this.executor = Executors.newSingleThreadExecutor(daemonFactory);
    }

    public Future<Void> saveAsync(Path path, GameState state) {
        return executor.submit(() -> {
            System.out.println("[StateManager] save started on " + Thread.currentThread().getName());
            try (ObjectOutputStream oos = new ObjectOutputStream(
                    new BufferedOutputStream(Files.newOutputStream(path)))) {
                oos.writeObject(state);
            }
            System.out.println("[StateManager] save finished, wrote " + state.activeEnemies.size()
                + " enemies to " + path);
            return null;
        });
    }

    public Future<GameState> loadAsync(Path path) {
        return executor.submit(() -> {
            System.out.println("[StateManager] load started on " + Thread.currentThread().getName());
            try (ObjectInputStream ois = new ObjectInputStream(
                    new BufferedInputStream(Files.newInputStream(path)))) {
                GameState state = (GameState) ois.readObject();
                System.out.println("[StateManager] load finished, read " + state.activeEnemies.size()
                    + " enemies from " + path);
                return state;
            }
        });
    }

    public Future<?> loadAsync(Path path, Consumer<GameState> onMainThread) {
        return executor.submit(() -> {
            try (ObjectInputStream ois = new ObjectInputStream(
                    new BufferedInputStream(Files.newInputStream(path)))) {
                GameState state = (GameState) ois.readObject();
                pendingMainThreadCallbacks.offer(() -> onMainThread.accept(state));
            } catch (IOException | ClassNotFoundException e) {
                pendingMainThreadCallbacks.offer(() ->
                    System.err.println("[StateManager] load failed: " + e.getMessage()));
            }
            return null;
        });
    }

    public void drainPending() {
        Runnable task;
        while ((task = pendingMainThreadCallbacks.poll()) != null) {
            task.run();
        }
    }

    public void shutdown() {
        executor.shutdown();
        try {
            if (!executor.awaitTermination(5, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
            Thread.currentThread().interrupt();
        }
    }
}
