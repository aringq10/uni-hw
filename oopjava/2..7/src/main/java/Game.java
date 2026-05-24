import java.nio.file.Path;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import npc.Enemy;
import npc.EnemyPrototypeRegistry;
import npc.civilian.Citizen;
import npc.io.GameState;
import npc.io.StateManager;
import npc.law.Officer;

/**
 * REPL entry point for the NPC manager. Holds the live
 * {@link List} of {@link Enemy} instances and the
 * {@link EnemyPrototypeRegistry}, and dispatches user commands
 * (add / damage / move / register / spawn / save / load / print).
 *
 * <p>Plays the <b>Originator</b> role in the Memento pattern by
 * handing its state to {@link StateManager} as a {@link GameState}
 * snapshot.
 */
public class Game {
    private static final Path SAVE_PATH = Path.of("save.bin");
    private static final StateManager sm = new StateManager();

    private static List<Enemy> enemies = new ArrayList<>();
    private static EnemyPrototypeRegistry registry = new EnemyPrototypeRegistry();

    /**
     * Starts the REPL. Reads whitespace-separated commands from stdin
     * until {@code quit}/{@code exit} or EOF.
     *
     * @param args ignored
     * @throws Exception propagates unexpected errors from save/load workers
     */
    public static void main(String[] args) throws Exception {
        printHelp();
        Scanner in = new Scanner(System.in);
        while (true) {
            System.out.print("> ");
            System.out.flush();
            if (!in.hasNextLine()) break;
            String[] p = in.nextLine().trim().split("\\s+");
            if (p[0].isEmpty()) continue;
            try {
                switch (p[0]) {
                    case "help"            -> printHelp();
                    case "print", "list"   -> printState();
                    case "add"             -> add(p);
                    case "damage"          -> damage(p);
                    case "move"            -> move(p);
                    case "register"        -> register(p);
                    case "spawn"           -> spawn(p);
                    case "save"            -> save();
                    case "load"            -> load();
                    case "quit", "exit"    -> { return; }
                    default                -> System.out.println("unknown: " + p[0] + " (try 'help')");
                }
            } catch (Exception e) {
                System.out.println("error: " + e);
            }
        }
    }

    private static void printHelp() {
        System.out.println("""
            commands:
              add citizen <first> <last> <profession>
              add officer <first> <last>
              damage <id> <amount>
              move <id> <dx> <dy>
              register <key> <id>
              spawn <key>
              print
              save
              load
              help
              quit""");
    }

    private static void printState() {
        System.out.println("enemies (" + enemies.size() + "):");
        for (Enemy e : enemies) System.out.println("  " + e);
        System.out.println("registry (" + registry.size() + "):" + registry);
    }

    private static void add(String[] p) {
        switch (p[1]) {
            case "citizen" -> {
                Citizen c = new Citizen(p[2], p[3], p[4]);
                enemies.add(c);
                System.out.println("added " + c);
            }
            case "officer" -> {
                Officer o = new Officer(p[2], p[3]);
                enemies.add(o);
                System.out.println("added " + o);
            }
            default -> System.out.println("usage: add citizen|officer ...");
        }
    }

    private static void damage(String[] p) {
        Enemy e = byId(Integer.parseInt(p[1]));
        e.takeDamage(Integer.parseInt(p[2]));
        System.out.println(e);
    }

    private static void move(String[] p) {
        Enemy e = byId(Integer.parseInt(p[1]));
        e.move(Integer.parseInt(p[2]), Integer.parseInt(p[3]));
        System.out.println(e);
    }

    private static void register(String[] p) {
        registry.register(p[1], byId(Integer.parseInt(p[2])));
        System.out.println("registered '" + p[1] + "' (registry size " + registry.size() + ")");
    }

    private static void spawn(String[] p) {
        Enemy e = registry.spawn(p[1]);
        enemies.add(e);
        System.out.println("spawned " + e);
    }

    private static void save() throws InterruptedException {
        sm.save(SAVE_PATH, new GameState(enemies, registry));
        System.out.println("saved " + enemies.size() + " enemies to " + SAVE_PATH);
    }

    private static void load() throws InterruptedException {
        GameState gs = sm.load(SAVE_PATH);
        enemies = new ArrayList<>(gs.activeEnemies);
        registry = gs.registry;
        System.out.println("loaded " + enemies.size() + " enemies from " + SAVE_PATH);
    }

    private static Enemy byId(int id) {
        for (Enemy e : enemies) if (e.getId() == id) return e;
        throw new IllegalArgumentException("no enemy with id " + id);
    }
}
