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

        System.out.println("\n-- Taking 10 damage with 1.5 multiplier");
        o.takeDamage(10, 1.5);
        o.println();

        System.out.println("\n-- Taking 150 damage");
        o.takeDamage(150);
        o.println();

        System.out.println("\n-- Changing name to Bobby Jones");
        o.setFirstName("Bobby");
        o.setLastName("Jones");
        o.println();

        System.out.println("\n-- Creating 5 more officers");
        Officer[] officers = new Officer[5];
        for (int i = 0; i < 5; i++) {
            officers[i] = new Officer();
            System.out.println("-- New officer " + (i + 1));
            officers[i].println();
        }
        System.out.println("\n-- Officer count:");
        System.out.println(Officer.getOfficerCount());
    }
}
