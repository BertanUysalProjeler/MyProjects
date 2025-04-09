public class Slot {
    private int productCount = 0;
    private boolean isFull = false;
    private Product product;

    // Setter and Getter methods
    public void setProduct(Product product) {
        this.product = product;
    }

    public void setFull(boolean full) {
        isFull = full;
    }

    public void setProductCount(int productCount) {
        this.productCount = productCount;
    }

    public int getProductCount() {
        return productCount;
    }

    public Product getProduct() {
        return product;
    }

    public boolean getIsFull() {
        return isFull;
    }
}
