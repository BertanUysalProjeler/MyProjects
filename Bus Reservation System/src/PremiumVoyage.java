import java.util.ArrayList;

public class PremiumVoyage extends Voyage {
    private int refundCut;
    private int premiumFee;
    private int[] seats;
    private ArrayList<Integer> takenSeats = new ArrayList<>();
    private double premiumPrice;
    public PremiumVoyage(String type, int id, String from, String to, int rows, double price, int refundCut, int premiumFee, double premiumPrice) {
        super(type, id, from, to, rows, price);
        setRefundCut(refundCut);
        setPremiumFee(premiumFee);
        int[] seats = new int[getRows() * 3];
        for (int i = 0; i < seats.length; i++) {
            seats[i] = i + 1;
        }
        setSeats(seats);
        setPremiumPrice(premiumPrice);
    }
    public void SeatPlanSymbolizer(int[] seats, ArrayList<Integer> takenSeats){
        StringBuilder output = new StringBuilder();
        for (int seatNumber : seats) {
            boolean isTaken = takenSeats.contains(seatNumber);
            if (isTaken) {
                if (seatNumber % 3 == 1){
                    output.append("X | ");
                }
                if (seatNumber % 3 == 2){
                    output.append("X ");
                }
                if (seatNumber % 3 == 0){
                    output.append("X" + "\n");
                }
            } else {
                if (seatNumber % 3 == 1){
                    output.append("* | ");
                }
                if (seatNumber % 3 == 2){
                    output.append("* ");
                }
                if (seatNumber % 3 == 0){
                    output.append("*" + "\n");
                }
            }
        }
        InputProcessor.getMainOutput().append(output);
    }
    public ArrayList<Integer> getTakenSeats() {
        return takenSeats;
    }
    public int[] getSeats () {
        return seats;
    }
    public void setSeats(int[] seats) {
        this.seats = seats;
    }
    public void setRefundCut (int refundCut){
        this.refundCut = refundCut;
    }
    public void setPremiumFee ( int premiumFee){
        this.premiumFee = premiumFee;
    }
    public void setPremiumPrice ( double premiumPrice){
        this.premiumPrice = premiumPrice;
    }
    public int getPremiumFee () {
        return premiumFee;
    }
    public double getRefundCut () {
        return refundCut;
    }
    public void setTakenSeats(ArrayList<Integer> takenSeats) {
        this.takenSeats = takenSeats;
    }
}
