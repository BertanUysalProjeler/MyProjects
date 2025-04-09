import java.util.ArrayList;

public class MinibusVoyage extends Voyage {
    private int[] seats;
    private ArrayList<Integer> takenSeats = new ArrayList<>();
    public MinibusVoyage(String type, int id, String from, String to, int rows, double price){
        super(type, id, from, to, rows, price);
        int[] seats = new int[getRows() * 2];
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
                if (seatNumber % 2 == 1){
                    output.append("X ");
                }
                if (seatNumber % 2 == 0){
                    output.append("X"+"\n");
                }
            } else {
                if (seatNumber % 2 == 1){
                    output.append("* ");
                }
                if (seatNumber % 2 == 0){
                    output.append("*"+"\n");
                }
            }
        }
        InputProcessor.getMainOutput().append(output);
    }
    public void setSeats(int[] seats) {
        this.seats = seats;
    }
    public int[] getSeats() {
        return seats;
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


