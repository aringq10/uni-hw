package npc.io;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.nio.file.Files;
import java.nio.file.Path;

public class StateManager {
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
