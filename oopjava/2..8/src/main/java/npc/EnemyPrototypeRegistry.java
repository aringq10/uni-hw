package npc;

import java.io.Serializable;
import java.util.HashMap;
import java.util.Map;

/**
 * Implementation of the <b>Prototype Registry</b> pattern: a keyed
 * {@link HashMap} of {@link Enemy} templates. Callers register a sample
 * enemy under a name and later {@link #spawn(String)} fresh copies of it
 * without knowing the concrete subclass.
 */
public class EnemyPrototypeRegistry implements Serializable {
    private static final long serialVersionUID = 1L;

    private final Map<String, Enemy> prototypes = new HashMap<>();

    /**
     * Stores (or replaces) the prototype associated with {@code key}.
     *
     * @param key       lookup name used by {@link #spawn(String)}
     * @param prototype enemy whose {@code clone()} will produce future copies
     */
    public void register(String key, Enemy prototype) {
        prototypes.put(key, prototype);
    }

    /**
     * Produces a fresh clone of the prototype registered under {@code key}.
     *
     * @param key lookup name previously passed to {@link #register}
     * @return a new {@link Enemy} instance (concrete subtype matches the prototype)
     * @throws NullPointerException if no prototype is registered for {@code key}
     */
    public Enemy spawn(String key) {
        return prototypes.get(key).clone();
    }

    /** @return number of currently registered prototypes. */
    public int size() {
        return prototypes.size();
    }

    /** {@inheritDoc} */
    @Override
    public String toString() {
        if (prototypes.isEmpty()) return "{}";
        StringBuilder sb = new StringBuilder();
        prototypes.forEach((k, v) -> sb.append("\n  ").append(k).append(" -> ").append(v));
        return sb.toString();
    }
}
