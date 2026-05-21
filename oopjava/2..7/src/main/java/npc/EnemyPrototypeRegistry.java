package npc;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

public class EnemyPrototypeRegistry implements Serializable {
    private static final long serialVersionUID = 1L;

    private final Map<String, Enemy> prototypes = new HashMap<>();

    public void register(String key, Enemy prototype) {
        prototypes.put(key, prototype);
    }

    public Enemy spawn(String key) {
        return prototypes.get(key).clone();
    }

    public int size() {
        return prototypes.size();
    }

    @Override
    public String toString() {
        if (prototypes.isEmpty()) return "{}";
        StringBuilder sb = new StringBuilder();
        prototypes.forEach((k, v) -> sb.append("\n  ").append(k).append(" -> ").append(v));
        return sb.toString();
    }
}
