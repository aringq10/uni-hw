package npc;

public interface Movable {
    void move(int dx, int dy);
    void setPosition(int x, int y);
    int getX();
    int getY();
}
