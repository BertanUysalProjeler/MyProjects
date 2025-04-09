public class Product {
    private String name;
    private int price;
    private double protein;
    private double carbohydrate;
    private double fat;
    private double calorie;

    // Constructor method
    public Product(String name, int price, double protein, double carbohydrate, double fat) {
        this.name = name;
        this.price = price;
        this.protein = protein;
        this.carbohydrate = carbohydrate;
        this.fat = fat;
        this.calorie = 4 * protein + 4 * carbohydrate + 9 * fat; // Initializing calorie
    }

    // Getter methods
    public String getName() {
        return name;
    }

    public int getPrice() {
        return price;
    }

    public double getProtein() {
        return protein;
    }

    public double getCarbohydrate() {
        return carbohydrate;
    }

    public double getFat() {
        return fat;
    }

    public double getCalorie() {
        return calorie;
    }

}
