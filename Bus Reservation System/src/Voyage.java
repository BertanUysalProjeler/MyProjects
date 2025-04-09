import java.util.ArrayList;

public abstract class Voyage {
    private double refundCut;
    private String type;
    private int id;
    private String from;
    private String to;
    private int rows;
    private double price;
    private double revenue;

    public Voyage(String type, int id, String from, String to, int rows, double price) {
        this.type = type;
        this.id = id;
        this.from = from;
        this.to = to;
        this.rows = rows;
        this.price = price;
        revenue = 0;
    }
    public abstract void SeatPlanSymbolizer(int[] seats, ArrayList<Integer> takenSeats);
    public abstract int[] getSeats();
    public abstract ArrayList<Integer> getTakenSeats();
    public int getRows() {
        return rows;
    }
    public double getRevenue() {
        return revenue;
    }
    public void setRevenue(double revenue) {
        this.revenue = revenue;
    }
    public String getFrom() {
        return from;
    }
    public String getTo() {
        return to;
    }
    public double getPrice() {
        return price;
    }
    public abstract int getPremiumFee();
    public double getRefundCut() {
        return this.refundCut;
    }
    public String getType() {
        return type;
    }

}
