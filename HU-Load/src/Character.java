public class Character {
    private int GridX; // The x-coordinate of the character
    private int GridY; // The y-coordinate of the character
    private double x;
    private double y;
    private double cellWidth;
    private double cellHeight;
    private double fuel;
    private double haul;
    private double moneyBank;


    // Constructor that initializes the character with the given coordinates
    public Character(int GridX, int GridY, double x, double y, double cellWidth, double cellHeight) {
        this.GridX = GridX;
        this.GridY = GridY;
        this.x = x;
        this.y = y;
        this.cellWidth = cellWidth;
        this.cellHeight = cellHeight;
        this.fuel = 10000;
        this.haul = 0;
        this.moneyBank = 0;
    }

    // Getter for the x-coordinate
    public double getX() {
        return x;
    }

    // Getter for the y-coordinate
    public double getY() {
        return y;
    }

    public int getGridX() {
        return GridX;
    }

    public int getGridY() {
        return GridY;
    }

    // Method to move the character by the given amounts in the x and y directions
    // Method to move the character by the given amounts in the x and y directions
    public void move(double dx, double dy) {
        x += dx * cellWidth; // Adjust the x-coordinate
        y += dy * cellHeight; // Adjust the y-coordinate
    }

    public void moveGrid(int dx, int dy) {
        GridX += dx; // Adjust the x-coordinate
        GridY += dy; // Adjust the y-coordinate
    }
    // New methods to update fuel, haul, and moneyBank
    public void decreaseFuel(double amount) {
        this.fuel -= amount;
    }

    public void increaseHaul(double amount) {
        this.haul += amount;
    }

    public void increaseMoneyBank(double amount) {
        this.moneyBank += amount;
    }

    // Getters for fuel, haul, and moneyBank
    public double getFuel() {
        return this.fuel;
    }

    public double getHaul() {
        return this.haul;
    }

    public double getMoneyBank() {
        return this.moneyBank;
    }
}