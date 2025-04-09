import java.util.ArrayList;

public class StandardVoyage extends Voyage {
    private int refundCut = 0;
    private int[] seats = new int[0];
    private ArrayList<Integer> takenSeats = new ArrayList<>();
    public StandardVoyage(String type, int id, String from, String to, int rows, double price, int refundCut) {
        super(type, id, from, to, rows, price);
        setRefundCut(refundCut);
        int[] seats = new int[getRows() * 4];
        for (int i = 0; i < seats.length; i++) {
            seats[i] = i + 1;
        }
        setSeats(seats);
    }

    public void SeatPlanSymbolizer(int[] seats, ArrayList<Integer> takenSeats){
        StringBuilder output = new StringBuilder();
        for (int seatNumber : seats) {
            boolean isTaken = takenSeats.contains(seatNumber);
            if (isTaken) {
                if (seatNumber % 4 == 1){
                    output.append("X ");
                }
                if (seatNumber % 4 == 2){
                    output.append("X | ");
                }
                if (seatNumber % 4 == 3){
                    output.append("X ");
                }
                if (seatNumber % 4 == 0){
                    output.append("X"+"\n");
                }
            } else {
                if (seatNumber % 4 == 1){
                    output.append("* ");
                }
                if (seatNumber % 4 == 2){
                    output.append("* | ");
                }
                if (seatNumber % 4 == 3){
                    output.append("* ");
                }
                if (seatNumber % 4 == 0){
                    output.append("*"+"\n");
                }
            }
        }
        InputProcessor.getMainOutput().append(output);
    }

    public void setSeats(int[] seats) {
        this.seats = seats;
    }
    public void setRefundCut(int refundCut) {
        this.refundCut = refundCut;
    }
    public int[] getSeats() {
        return seats;
    }
    public double getRefundCut() {
        return refundCut;
    }
    public ArrayList<Integer> getTakenSeats() {
        return takenSeats;
    }
    public int getPremiumFee() {
        return 0;
    }
    public void setTakenSeats(ArrayList<Integer> takenSeats) {
        this.takenSeats = takenSeats;
    }
}
