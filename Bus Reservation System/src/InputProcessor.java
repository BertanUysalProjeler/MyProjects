import java.util.*;

public class InputProcessor {
    private static HashMap<Integer, Voyage> voyages = new HashMap<>();
    private static ArrayList<Ticket> tickets = new ArrayList<>();
    private static StringBuilder MainOutput = new StringBuilder();

    /**
     * The main method of the InputProcessor class.
     * It reads the input file, processes each line, and writes the output to the output file.
     * If the last non-empty line does not contain the "Z_REPORT" command, it performs a Z report.
     * If all lines are empty, it also performs a Z report.
     *
     * @param args Command line arguments. Not used in this method.
     */
    public static void main(String[] args) {

        // Read the input file
        String[] lines = FileInput.readFile(args[0], true, true);

        // Process the initial lines
        InitialProcessor(lines);

        // Find the last non-empty line
        String lastNonEmptyLine = "";
        for (int i = lines.length - 1; i >= 0; i--) {
            if (!lines[i].trim().isEmpty()) {
                lastNonEmptyLine = lines[i];
                break;
            }
        }

        // If the last non-empty line does not contain the "Z_REPORT" command, perform a Z report
        // Also perform a Z report if all lines are empty (i.e., lastNonEmptyLine is still empty)
        if (lastNonEmptyLine.isEmpty() || !lastNonEmptyLine.contains("Z_REPORT")) {
            ZReporter(voyages, new String[]{"Z_REPORT"});
        }

        // Trim the StringBuilder before writing to the file to remove trailing newlines
        // Write the output to the output file
        FileOutput.writeToFile(args[1], MainOutput.toString().trim(), false, false);
    }
    /**
     * Processes the initial lines of the input file.
     * It reads each line, splits it into parts, and performs the appropriate action based on the command in the line.
     * The actions include initializing a voyage, selling a ticket, refunding a ticket, cancelling a voyage, printing a voyage, and generating a Z report.
     *
     * @param lines The lines of the input file to be processed.
     */
    public static void InitialProcessor(String[] lines){
        // Loop through each line in the input file
        for (String line : lines){
            // Skip empty lines
            if (line.trim().isEmpty()) {
                continue;
            }

            // Split the line into parts
            String[] parts = line.split("\t");

            // Check if the command in the line is valid
            boolean erroneousCommand = !line.split("\t")[0].equals("INIT_VOYAGE") && !line.split("\t")[0].equals("Z_REPORT") && !line.split("\t")[0].equals("PRINT_VOYAGE") && !line.split("\t")[0].equals("SELL_TICKET") && !line.split("\t")[0].equals("REFUND_TICKET") && !line.split("\t")[0].equals("CANCEL_VOYAGE");
            if (erroneousCommand) {
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                getMainOutput().append("ERROR: There is no command namely ").append(parts[0]).append("!").append("\n");
            }

            // Perform the appropriate action based on the command in the line
            if (parts[0].equals("INIT_VOYAGE")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                VoyageInitializer(parts);
            }
            if (parts[0].equals("Z_REPORT")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                ZReporter(voyages, parts);
            }
            if (parts[0].equals("PRINT_VOYAGE")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                VoyagePrinter(voyages, parts, false);
            }
            if (parts[0].equals("SELL_TICKET")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                TicketSeller(parts, getTickets(), getVoyages());
            }
            if (parts[0].equals("REFUND_TICKET")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                TicketRefunder(parts, getTickets(), getVoyages());
            }
            if (parts[0].equals("CANCEL_VOYAGE")){
                getMainOutput().append("COMMAND: ").append(line).append("\n");
                VoyageCanceller(parts);
            }
        }
    }
    /**
     * Initializes a new voyage based on the provided parts.
     * The parts should contain the voyage type, ID, departure and arrival locations, number of rows, ticket price, and other optional parameters depending on the voyage type.
     * The method validates the input, creates a new voyage of the appropriate type, and adds it to the voyages map.
     *
     * @param parts The parts of the command line that contain the voyage details.
     */
    public static void VoyageInitializer(String[] parts){
        // Parse the voyage ID, number of rows, and ticket price from the parts
        int id = Integer.parseInt(parts[2]);
        int rows = Integer.parseInt(parts[5]);
        double price = Double.parseDouble(parts[6]);
        // Check if the voyage type is valid
        boolean erroneousUsage = parts[1].equals("Premium") || parts[1].equals("Standard") || parts[1].equals("Minibus");
        if (!erroneousUsage) {
            getMainOutput().append("ERROR: Erroneous usage of \"INIT_VOYAGE\" command!" + "\n");
            return;
        }
        // Check if a voyage with the same ID already exists
        if (voyages.containsKey(id)) {
            getMainOutput().append("ERROR: There is already a voyage with ID of ").append(id).append("!").append("\n");
            return;
        }
        // Validate the voyage ID, number of rows, and ticket price
        if (id < 0){
            getMainOutput().append("ERROR: ").append(id).append(" is not a positive integer, ID of a voyage must be a positive integer!").append("\n");
            return;
        }
        if (rows < 0){
            getMainOutput().append("ERROR: ").append(rows).append(" is not a positive integer, number of seat rows of a voyage must be a positive integer!").append("\n");
            return;
        }
        if (price < 0){
            getMainOutput().append("ERROR: ").append((int) price).append(" is not a positive number, price must be a positive number!").append("\n");
            return;
        }
        // Create a new voyage of the appropriate type and add it to the voyages map
        if (parts[1].equals("Standard")){
            String type = parts[1];
            String from = parts[3];
            String to = parts[4];
            // Parse the refund cut from the parts
            int refundCut = Integer.parseInt(parts[7]);
            // Validate the refund cut
            if (refundCut < 0 || refundCut > 100){
                getMainOutput().append("ERROR: ").append(refundCut).append(" is not an integer that is in range of [0, 100], refund cut must be an integer that is in range of [0, 100]!").append("\n");
                return;
            }
            getMainOutput().append("Voyage ").append(parts[2]).append(" was initialized as a standard (2+2) voyage from ").append(parts[3])
                    .append(" to ").append(parts[4]).append(" with ").append(String.format("%.2f", Double.parseDouble(parts[6])))
                    .append(" TL priced ").append(rows*4).append(" regular seats.").append(" Note that refunds will be ")
                    .append(parts[7]).append("% less than the paid amount.\n");
            // Create a new StandardVoyage and add it to the voyages map
            Voyage newVoyage = new StandardVoyage(type, id, from, to, rows, price, refundCut);
            voyages.put(id, newVoyage);
            // Similar code for Premium and Minibus voyage types...
        }
        if (parts[1].equals("Premium")){
            String type = parts[1];
            String from = parts[3];
            String to = parts[4];
            int refundCut = Integer.parseInt(parts[7]);
            int premiumFee = Integer.parseInt(parts[8]);
            double premiumTicketPrice = (1 + (double)premiumFee/100) * price;

            if (refundCut < 0 || refundCut > 100){
                getMainOutput().append("ERROR: ").append(refundCut).append(" is not an integer that is in range of [0, 100], refund cut must be an integer that is in range of [0, 100]!").append("\n");
                return;
            }
            if (premiumFee < 0){
                getMainOutput().append("ERROR: ").append(premiumFee).append(" is not a non-negative integer, premium fee must be a non-negative integer!").append("\n");
                return;
            }
            getMainOutput().append("Voyage ").append(parts[2]).append(" was initialized as a premium (1+2) voyage from ").append(parts[3])
                    .append(" to ").append(parts[4]).append(" with ").append(String.format("%.2f", Double.parseDouble(parts[6])))
                    .append(" TL priced ").append(rows*2).append(" regular seats and ").append(String.format("%.2f", premiumTicketPrice)).append(" TL priced ").append(rows).append(" premium seats.")
                    .append(" Note that refunds will be ").append(parts[7]).append("% less than the paid amount.\n");

            Voyage newVoyage = new PremiumVoyage(type, id, from, to, rows, price, refundCut, premiumFee, premiumTicketPrice);
            voyages.put(id, newVoyage);
        }
        if (parts[1].equals("Minibus")){
            String type = parts[1];
            String from = parts[3];
            String to = parts[4];

            getMainOutput().append("Voyage ").append(parts[2]).append(" was initialized as a minibus (2) voyage from ").append(parts[3])
                    .append(" to ").append(parts[4]).append(" with ").append(String.format("%.2f", Double.parseDouble(parts[6])))
                    .append(" TL priced ").append(rows*2).append(" regular seats.").append(" Note that minibus tickets are not refundable.").append("\n");

            Voyage newVoyage = new MinibusVoyage(type, id, from, to, rows, price);
            voyages.put(id, newVoyage);
        }
    }
    /**
     * Sells tickets for a specific voyage.
     * The parts should contain the voyage ID and the seat numbers to be sold.
     * The method validates the input, creates a new ticket, and adds it to the tickets list.
     * It also updates the voyage's revenue and taken seats list.
     *
     * @param parts The parts of the command line that contain the ticket details.
     * @param tickets The list of all tickets.
     * @param voyages The map of all voyages.
     */
    public static void TicketSeller(String[] parts, ArrayList <Ticket> tickets, HashMap<Integer, Voyage> voyages){
        // Parse the voyage ID from the parts
        int voyageId = Integer.parseInt(parts[1]);

        // Validate the command usage
        if (parts.length != 3) {
            getMainOutput().append("ERROR: Erroneous usage of \"SELL_TICKET\" command!" + "\n");
            return;
        }

        // Check if the voyage exists
        if (!voyages.containsKey(voyageId)) {
            getMainOutput().append("ERROR: There is no voyage with ID of ").append(voyageId).append("!").append("\n");
            return;
        }

        // Parse the seat numbers from the parts
        String[] seatNumbersString = parts[2].split("_");
        int[] seatNumbers = Arrays.stream(seatNumbersString).mapToInt(Integer::parseInt).toArray();

        // Calculate the total sell amount
        double sellAmount = 0;
        int[] seats = voyages.get(voyageId).getSeats();
        List<Integer> takenSeats = voyages.get(voyageId).getTakenSeats();

        // If the voyage is of type "Premium", calculate the sell amount considering the premium fee
        if (voyages.get(voyageId).getType().equals("Premium")){
            int premiumFee = voyages.get(voyageId).getPremiumFee();
            double premiumTicketPrice = (1 + (double)premiumFee/100) * voyages.get(voyageId).getPrice();

            // Calculate the sell amount for each seat
            for (int seatNumber : seatNumbers) {
                // Validate the seat number
                if (seatNumber <= 0) {
                    getMainOutput().append("ERROR: ").append(seatNumber).append(" is not a positive integer, seat number must be a positive integer!").append("\n");
                    return;
                }
                if (Arrays.stream(seats).noneMatch(seat -> seat == seatNumber)) {
                    getMainOutput().append("ERROR: There is no such a seat!").append("\n");
                    return;
                }
                // Check if the seat exists and is not taken
                if (!Arrays.stream(seats).anyMatch(seat -> seat == seatNumber) || takenSeats.contains(seatNumber)) {
                    getMainOutput().append("ERROR: One or more seats already sold!" + "\n");
                    return;
                }

                // Add the seat price to the sell amount
                if (seatNumber % 3 == 1) {
                    sellAmount += premiumTicketPrice;
                } else {
                    sellAmount += voyages.get(voyageId).getPrice();
                }
            }
        } else {
            // If the voyage is not of type "Premium", calculate the sell amount normally
            for (int seatNumber : seatNumbers) {
                // Validate the seat number
                if (seatNumber <= 0) {
                    getMainOutput().append("ERROR: ").append(seatNumber).append(" is not a positive integer, seat number must be a positive integer!").append("\n");
                    return;
                }
                if (Arrays.stream(seats).noneMatch(seat -> seat == seatNumber)) {
                    getMainOutput().append("ERROR: There is no such a seat!").append("\n");
                    return;
                }
                // Check if the seat exists and is not taken
                if (!Arrays.stream(seats).anyMatch(seat -> seat == seatNumber) || takenSeats.contains(seatNumber)) {
                    getMainOutput().append("ERROR: One or more seats already sold!" + "\n");
                    return;
                }

                // Add the seat price to the sell amount
                sellAmount += voyages.get(voyageId).getPrice();
            }
        }

        // Update the voyage's taken seats list and revenue
        for (int seatNumber : seatNumbers) {
            voyages.get(voyageId).getTakenSeats().add(seatNumber);
        }
        voyages.get(voyageId).setRevenue(voyages.get(voyageId).getRevenue() + sellAmount);

        // Create a new ticket and add it to the tickets list
        Ticket newTicket = new Ticket(voyageId, seatNumbers);
        tickets.add(newTicket);

        // Print a success message
        String seatNumbersFormatted = String.join("-", parts[2].split("_"));
        getMainOutput().append("Seat ").append(seatNumbersFormatted).append(" of the Voyage ").append(voyageId).append(" from ").append(voyages.get(voyageId).getFrom()).append(" to ").append(voyages.get(voyageId).getTo()).append(" was successfully sold for ").append(String.format("%.2f", sellAmount)).append(" TL.").append("\n");
    }
    /**
     * Refunds tickets for a specific voyage.
     * The parts should contain the voyage ID and the seat numbers to be refunded.
     * The method validates the input, removes the ticket from the tickets list, and updates the voyage's revenue and taken seats list.
     * It also calculates the refund amount considering the voyage type and refund cut.
     *
     * @param parts The parts of the command line that contain the ticket details.
     * @param tickets The list of all tickets.
     * @param voyages The map of all voyages.
     */
    public static void TicketRefunder(String[] parts, ArrayList <Ticket> tickets, HashMap<Integer, Voyage> voyages){
        // Validate the command usage
        if (parts.length != 3) {
            getMainOutput().append("ERROR: Erroneous usage of \"REFUND_TICKET\" command!" + "\n");
            return;
        }

        // Parse the voyage ID from the parts
        int voyageId = Integer.parseInt(parts[1]);

        // Check if the voyage exists
        if (!voyages.containsKey(voyageId)) {
            getMainOutput().append("ERROR: There is no voyage with ID of ").append(voyageId).append("!").append("\n");
            return;
        }

        // Parse the seat numbers from the parts
        String[] seatNumbersString = parts[2].split("_");
        int[] seatNumbers = Arrays.stream(seatNumbersString).mapToInt(Integer::parseInt).toArray();

        // Calculate the total refund amount
        double refundAmount = 0;
        List<Integer> takenSeats = voyages.get(voyageId).getTakenSeats();
        int[] seats = voyages.get(voyageId).getSeats();
        // If the voyage is of type "Premium", calculate the refund amount considering the premium fee and refund cut
        if (voyages.get(voyageId).getType().equals("Premium")){
            int premiumFee = voyages.get(voyageId).getPremiumFee();
            double premiumTicketPrice = (1 + (double)premiumFee/100) * voyages.get(voyageId).getPrice();

            // Calculate the refund amount for each seat
            for (int seatNumber : seatNumbers) {
                // Validate the seat number
                if (seatNumber <= 0) {
                    getMainOutput().append("ERROR: ").append(seatNumber).append(" is not a positive integer, seat number must be a positive integer!").append("\n");
                    return;
                }
                if (Arrays.stream(seats).noneMatch(seat -> seat == seatNumber)) {
                    getMainOutput().append("ERROR: There is no such a seat!").append("\n");
                    return;
                }
                // Check if the seat is taken
                if (!takenSeats.contains(seatNumber)) {
                    getMainOutput().append("ERROR: One or more seats are already empty!" + "\n");
                    return;
                }

                // Add the seat price to the refund amount
                if (seatNumber % 3 == 1) {
                    refundAmount += premiumTicketPrice * (1 - voyages.get(voyageId).getRefundCut()/100.0);
                } else {
                    refundAmount += voyages.get(voyageId).getPrice() * (1 - voyages.get(voyageId).getRefundCut()/100.0);
                }
            }
        } else if (voyages.get(voyageId).getType().equals("Standard")) {
            // If the voyage is not of type "Premium", calculate the refund amount normally
            for (int seatNumber : seatNumbers) {
                // Validate the seat number
                if (seatNumber <= 0) {
                    getMainOutput().append("ERROR: ").append(seatNumber).append(" is not a positive integer, seat number must be a positive integer!").append("\n");
                    return;
                }

                // Check if the seat is taken
                if (!takenSeats.contains(seatNumber)) {
                    getMainOutput().append("ERROR: One or more seats are already empty!" + "\n");
                    return;
                }

                // Add the seat price to the refund amount
                refundAmount += voyages.get(voyageId).getPrice() * (1 - voyages.get(voyageId).getRefundCut()/100.0);
            }
        } else {
            // If the voyage is of type "Minibus", tickets are not refundable
            getMainOutput().append("ERROR: Minibus tickets are not refundable!" + "\n");
            return;
        }

        // Update the voyage's taken seats list and revenue
        for (int seatNumber : seatNumbers) {
            voyages.get(voyageId).getTakenSeats().remove((Integer) seatNumber);
        }
        voyages.get(voyageId).setRevenue(voyages.get(voyageId).getRevenue() - refundAmount);

        // Remove the refunded tickets from the tickets list
        tickets.removeIf(ticket -> ticket.getVoyageId() == voyageId && Arrays.equals(ticket.getSeatNumbers(), seatNumbers));

        // Print a success message
        String seatNumbersFormatted = String.join("-", parts[2].split("_"));
        getMainOutput().append("Seat ").append(seatNumbersFormatted).append(" of the Voyage ").append(voyageId).append(" from ").append(voyages.get(voyageId).getFrom()).append(" to ").append(voyages.get(voyageId).getTo()).append(" was successfully refunded for ").append(String.format("%.2f", refundAmount)).append(" TL.").append("\n");
    }
    /**
     * Cancels a specific voyage.
     * The parts should contain the voyage ID to be cancelled.
     * The method validates the input, removes the voyage from the voyages map, and refunds all tickets for the voyage.
     * It also updates the voyage's revenue and taken seats list.
     *
     * @param parts The parts of the command line that contain the voyage details.
     */
    public static void VoyageCanceller(String[] parts){
        // Validate the command usage
        if (parts.length != 2) {
            getMainOutput().append("ERROR: Erroneous usage of \"CANCEL_VOYAGE\" command!" + "\n");
            return;
        }
        // Parse the voyage ID from the parts
        int voyageId = Integer.parseInt(parts[1]);
        // Validate the voyage ID
        if (voyageId < 0) {
            getMainOutput().append("ERROR: " + voyageId + " is not a positive integer, ID of a voyage must be a positive integer!").append("\n");
            return;
        }
        // Check if the voyage exists
        if (!voyages.containsKey(voyageId)) {
            getMainOutput().append("ERROR: There is no voyage with ID of " + voyageId + "!").append("\n");
            return;
        }
        int[] seatsCopy = voyages.get(voyageId).getSeats().clone();
        ArrayList<Integer> takenSeatsCopy = new ArrayList<>(voyages.get(voyageId).getTakenSeats());
        double refundedAmount = 0;
        boolean refundable = false;
        if (voyages.get(voyageId).getType().equals("Standard") || voyages.get(voyageId).getType().equals("Premium")){
            refundable = true;
        }
        // Check if there are any taken seats
        if (!voyages.get(voyageId).getTakenSeats().isEmpty()) {
            // If the voyage is of type "Minibus", tickets are not refundable
            if (voyages.get(voyageId).getType().equals("Minibus")) {
                getMainOutput().append("ERROR: Minibus tickets are not refundable!" + "\n");
            }
            // If the voyage is of type "Premium", calculate the refund amount considering the premium fee and refund cut
            if (voyages.get(voyageId).getType().equals("Premium")) {
                int premiumFee = voyages.get(voyageId).getPremiumFee();
                double premiumTicketPrice = (1 + (double) premiumFee / 100) * voyages.get(voyageId).getPrice();
                // Calculate the refund amount for each seat
                for (int seatNumber : voyages.get(voyageId).getTakenSeats()) {
                    if (seatNumber % 3 == 1) {
                        refundedAmount += premiumTicketPrice;
                    } else {
                        refundedAmount += voyages.get(voyageId).getPrice();
                        }
                }
            }
            // If the voyage is of type "Standard", calculate the refund amount normally
            if (voyages.get(voyageId).getType().equals("Standard")){
                // Calculate the refund amount for each seat
                for (int seatNumber : voyages.get(voyageId).getTakenSeats()) {
                    refundedAmount += voyages.get(voyageId).getPrice();
                }
            }
            // Remove the refunded tickets from the tickets list
            tickets.removeIf(ticket -> ticket.getVoyageId() == voyageId && voyages.get(voyageId).getTakenSeats().isEmpty());
        }
        if (refundable){
            getMainOutput().append("Voyage ").append(voyageId).append(" was successfully cancelled!\n").append("Voyage details can be found below:").append("\n");
            getMainOutput().append("Voyage ").append(voyageId).append("\n").append(voyages.get(voyageId).getFrom()).append("-").append(voyages.get(voyageId).getTo()).append("\n");
            // Update the voyage's revenue
            voyages.get(voyageId).setRevenue(voyages.get(voyageId).getRevenue() - refundedAmount);
            voyages.get(voyageId).SeatPlanSymbolizer(seatsCopy, takenSeatsCopy);
            getMainOutput().append("Revenue: ").append(String.format("%.2f", voyages.get(voyageId).getRevenue())).append("\n");

            // Remove the voyage from the voyages map
            voyages.remove(voyageId);
        }
    }
    public static void ZReporter(HashMap<Integer, Voyage> voyages, String[] parts){
        if (parts.length != 1) {
            getMainOutput().append("ERROR: Erroneous usage of \"Z_REPORT\" command!" + "\n");
        } else {
            getMainOutput().append("Z Report:" + "\n");
            if (voyages.isEmpty()) {
                getMainOutput().append("----------------" + "\n" + "No Voyages Available!" + "\n" + "----------------" + "\n");
            } else {
                // Create a list from elements of HashMap
                List<Map.Entry<Integer, Voyage>> list = new ArrayList<>(voyages.entrySet());
                // Sort the list
                list.sort(Map.Entry.comparingByKey());
                VoyagePrinter(voyages, parts, true);
                }
            }
    }
    public static void VoyagePrinter(HashMap<Integer, Voyage> voyages, String[] parts , boolean fromZReporter){
        if (fromZReporter){
            getMainOutput().append("----------------").append("\n");
            for (Map.Entry<Integer, Voyage> entry : voyages.entrySet()) {
                int voyageId = entry.getKey();
                getMainOutput().append("Voyage ").append(entry.getKey()).append("\n");
                getMainOutput().append(voyages.get(voyageId).getFrom()).append("-").append(voyages.get(voyageId).getTo()).append("\n");
                voyages.get(voyageId).SeatPlanSymbolizer(voyages.get(voyageId).getSeats(), voyages.get(voyageId).getTakenSeats());
                getMainOutput().append("Revenue: ").append(String.format("%.2f", voyages.get(voyageId).getRevenue())).append("\n");
                getMainOutput().append("----------------").append("\n");
            }
        }
        if (!fromZReporter){
            if (parts.length != 2) {
                getMainOutput().append("ERROR: Erroneous usage of \"PRINT_VOYAGE\" command!" + "\n");
                return;
            }
            int voyageId = Integer.parseInt(parts[1]);
            if (voyageId < 0) {
                getMainOutput().append("ERROR: ").append(voyageId).append(" is not a positive integer, ID of a voyage must be a positive integer!").append("\n");
                return;
            }
            if (!voyages.containsKey(voyageId)) {
                getMainOutput().append("ERROR: There is no voyage with ID of ").append(voyageId).append("!").append("\n");
                return;
            }
            getMainOutput().append("Voyage ").append(voyageId).append("\n").append(voyages.get(voyageId).getFrom()).append("-").append(voyages.get(voyageId).getTo()).append("\n");
            voyages.get(voyageId).SeatPlanSymbolizer(voyages.get(voyageId).getSeats(), voyages.get(voyageId).getTakenSeats());
            getMainOutput().append("Revenue: ").append(String.format("%.2f", voyages.get(voyageId).getRevenue())).append("\n");
        }
    }
    public static HashMap<Integer, Voyage> getVoyages() {
        return voyages;
    }
    public static ArrayList<Ticket> getTickets() {
        return tickets;
    }
    public static StringBuilder getMainOutput() {
        return MainOutput;
    }
}
