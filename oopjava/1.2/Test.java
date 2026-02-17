public class Test {
    public static void main(String[] args) {
        Officer o = new Officer("John", "Doe", 23, 9);

        System.out.println("-- Initial officer");
        o.println();

        System.out.println("\n-- Setting x to 10 and giving a donut");
        o.setX(10);
        o.giveDonut();
        o.println();

        System.out.println("\n-- Eating a donut");
        o.eatDonut();
        o.println();

        System.out.println("\n-- Eating a donut");
        o.eatDonut();
        o.println();

        System.out.println("\n-- Taking 150 damage");
        o.takeDamage(150);
        o.println();
    }
}
