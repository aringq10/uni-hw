package npc.io;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;

/**
 * Persists and restores {@link GameState} snapshots. Plays the
 * <b>Caretaker</b> role in the Memento pattern.
 *
 * <p>Each call runs the actual I/O on a freshly spawned worker thread
 * named {@code state-io-worker} and immediately {@code join()}s it, so
 * the caller sees a blocking API while the I/O itself stays off any
 * implicit context the caller might care about.
 */
public class StateManager {
    /**
     * Writes the snapshot to {@code path} via Java serialization.
     *
     * @param path  destination file (overwritten)
     * @param state snapshot to persist
     * @throws InterruptedException if the worker thread is interrupted while joining
     */
    public void save(Path path, GameState state) throws InterruptedException {
        Thread t = new Thread(() -> {
            System.out.println("[StateManager] save on " + Thread.currentThread().getName());
            try (ObjectOutputStream oos = new ObjectOutputStream(Files.newOutputStream(path))) {
                oos.writeObject(state);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }, "state-io-worker");
        t.start();
        t.join();
    }

    /**
     * Reads a previously saved snapshot from {@code path}.
     *
     * @param path source file
     * @return the deserialized {@link GameState}
     * @throws InterruptedException if the worker thread is interrupted while joining
     */
    public GameState load(Path path) throws InterruptedException {
        Object[] result = new Object[1];
        Thread t = new Thread(() -> {
            System.out.println("[StateManager] load on " + Thread.currentThread().getName());
            try (ObjectInputStream ois = new ObjectInputStream(Files.newInputStream(path))) {
                result[0] = ois.readObject();
            } catch (IOException | ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
        }, "state-io-worker");
        t.start();
        t.join();
        return (GameState) result[0];
    }
}
