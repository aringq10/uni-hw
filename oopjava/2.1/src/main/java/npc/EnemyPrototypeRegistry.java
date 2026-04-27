package npc;

import java.util.HashMap;
import java.util.Map;

public class EnemyPrototypeRegistry {
    private final Map<String, Enemy> prototypes = new HashMap<>();

    public void register(String key, Enemy prototype) {
        if (key == null || key.isEmpty()) {
            throw new IllegalArgumentException("Prototype key cannot be empty");
        }
        if (prototype == null) {
            throw new IllegalArgumentException("Prototype cannot be null");
        }
        prototypes.put(key, prototype);
    }

    public Enemy spawn(String key) {
        Enemy template = prototypes.get(key);
        if (template == null) {
            throw new IllegalArgumentException("No prototype registered for key: " + key);
        }
        return template.clone();
    }

    public boolean contains(String key) {
        return prototypes.containsKey(key);
    }
}
