public class Ticket {
    private int voyageId;
    private int seatNumbers[];

    public Ticket(int voyageId, int[] seatNumbers) {
        this.voyageId = voyageId;
        this.seatNumbers = seatNumbers;
    }
    public int getVoyageId() {
        return voyageId;
    }
    public int[] getSeatNumbers() {
        return seatNumbers;
    }
}
