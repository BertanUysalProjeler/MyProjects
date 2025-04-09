import java.util.ArrayList;
import java.util.List;


public class Machine {
    private static Slot[] slots = new Slot[24];

    public static void main(String[] args) {
        // Initialize slots array
        for (int i = 0; i < slots.length; i++) {
            slots[i] = new Slot();
        }

        // Now you can proceed with other operations
        slotFiller(args[0], getSlots(),args[2]);
        writeProductsToFile(args[2]);
        processPurchases(args[1],args[2]);
        writeProductsToFile(args[2]);
    }
    /**
     * Reads the file at the given path and returns contents of it in a string array.Reads it line by line, finds the available slot and places the product.
     *
     * @param filePath              Path to the file that is going to be read.
     * @param slots                 Newly initialized slots array.
     * @param outputFilePath        Path to the file that is the contents going to be written.
     */
    public static void slotFiller(String filePath,Slot[] slots,String outputFilePath) {
        String[] lines = FileInput.readFile(filePath, true, true);
        StringBuilder fillerOutput = new StringBuilder(); // Initializing new StringBuilder for the content that is going to be written.

        int slotIndex;
        boolean isMachineFull = true;
        int lastCounter = 0;  // Variable for last checking purpose for if the slots are full or not.

        for (String line : lines) {      // Reading the lines of the product text file content line by line
            String[] parts = line.split("\t");
            int counter = 0;

            if (parts.length == 3) {       // Seperating the properties of product and filling the variables
                String name = parts[0];
                int price = Integer.parseInt(parts[1]);
                String[] secondParts = parts[2].split(" ");
                double protein = Double.parseDouble(secondParts[0]);
                double carbohydrate = Double.parseDouble(secondParts[1]);
                double fat = Double.parseDouble(secondParts[2]);

                Product product = new Product(name, price, protein, carbohydrate, fat);   // Constructing the product

                // Find the next available slot to load the product
                int nextSlotResult = findNextSlot(0, product, slots);
                if (nextSlotResult == -1) {
                    fillerOutput.append("There is no available place to put ").append(product.getName()).append("\n");
                    for (Slot slot : slots) {
                        if (slot.getProductCount() == 10) {
                            counter++;
                        }
                    }
                    if (counter == 24) {         // Checking for if the machine is full or not
                        isMachineFull = true;
                        break;
                    }else {
                        isMachineFull = false;
                    }
                } else {
                    slotIndex = nextSlotResult ;
                    slots[slotIndex].setProduct(product); // Storing the product in the slot
                }
            }
        }
        for (Slot slot : slots) {
            if (slot.getProductCount() == 10) {
                lastCounter++;
            }
        }
        if (lastCounter != 24) {
            isMachineFull = false;
        }

        if (isMachineFull){
            fillerOutput.append("INFO: The machine is full");
        }
        FileOutput.writeToFile(outputFilePath, fillerOutput.toString(), true, true);  // Writing the output content to the output file
    }

    /**
     * Finds the next available slot for the specific product to be placed.
     *
     * @param startingIndex              Variable for starting index.
     * @param slots                      Slots array.
     * @param product                    Product to be placed.
     */
    public static int findNextSlot(int startingIndex, Product product, Slot[] slots) {
        boolean foundSuitableSlot = false;       // Initializing a boolean variable for checking if the process is successful or not
        int suitableSlotIndex = -1;              // Placeholder for the suitable slot index

        while (startingIndex < slots.length && !foundSuitableSlot) {
            Slot currentSlot = slots[startingIndex];

            // Checking if the slot is empty or has the same product and is not full
            if ((currentSlot.getProduct() == null || currentSlot.getProduct().getName().equals(product.getName())) && !currentSlot.getIsFull()) {
                foundSuitableSlot = true;
                suitableSlotIndex = startingIndex;
                break;
            } else {
                startingIndex++; // Moving to the next slot
            }
        }

        if (foundSuitableSlot) {
            // If found, updating the product count or set the product if the slot was empty
            Slot slot = slots[suitableSlotIndex];
            if (slot.getProduct() == null) {
                slot.setProduct(product);
                slot.setProductCount(1); // Assuming each call to this method represents an attempt to add one product
            } else {
                slot.setProductCount(slot.getProductCount() + 1);
            }

            if (slot.getProductCount() == 10) {
                slot.setFull(true);
            }
        }

        return suitableSlotIndex; // Returning -1 if all slots are full or no suitable slot was found
    }

    /**
     * Writes the product to the machine interface
     *
     * @param filePath              Stands for the output text file.
     */
    public static void writeProductsToFile(String filePath) {
        StringBuilder productsContent = new StringBuilder();    // New StringBuilder for the content to be written
        productsContent.append("-----Gym Meal Machine-----\n");

        int elementCount = 0; // Keep track of the number of elements added to the current line
        for (Slot slot : slots) {
            if (slot.getProductCount() != 0) {
                productsContent.append(slot.getProduct().getName())
                        .append("(")
                        .append((int) slot.getProduct().getCalorie())
                        .append(", ")
                        .append(slot.getProductCount())
                        .append(")___");
            } else {
                productsContent.append("___(0, 0)___");
            }

            elementCount++;
            if (elementCount == 4) { // After every fourth element, inserting a newline
                productsContent.append("\n");
                elementCount = 0; // Resetting the element count
            }
        }
        productsContent.append("----------");

        // Ensuring the file is overwritten and not appended, hence set append = false
        FileOutput.writeToFile(filePath, productsContent.toString(), true, true);
    }

    public static Slot[] getSlots() {
        return slots;
    }

    /**
     * Gets the information about purchasing from the purchase text file, processes every purchase and writes the output to the output text file
     *
     * @param purchaseFilePath            Stands for the purchase text file.
     * @param outputFilePath              Stands for the output text file.
     */

    public static void processPurchases(String purchaseFilePath, String outputFilePath) {
        // Assuming readPurchaseDetails reads and formats each purchase attempt
        List<String> purchaseDetails = readPurchaseDetails(purchaseFilePath);      // Reading purchase details from the text file
        StringBuilder allPurchasesOutput = new StringBuilder();                    // New StringBuilder to store the content that is going to be written

        for (String detail : purchaseDetails) {
            // Splitting detail to extract input, money, choiceType, value
            String[] parts = detail.split("\t");// Assuming tab-separated values
            String input = parts[0]; // Simplified; in practice, constructing input from parts
            String [] moneys = parts[1].split(" ");// Parsing actual money values from parts
            int totalMoney = 0;   // Evaluating the total money
            for (int i = 0; i <= moneys.length - 1; i++){
                int currentMoney = Integer.parseInt(moneys[i]);
                totalMoney = totalMoney + currentMoney;
            }
            String choiceType = parts[parts.length - 2];
            int value = Integer.parseInt(parts[parts.length - 1]);

            // Processing each purchase and collect output
            String purchaseOutput = processPurchase(input, totalMoney, moneys,choiceType, value, getSlots());
            allPurchasesOutput.append(purchaseOutput);
        }

        // Writing all processed purchase details to the output file
        FileOutput.writeToFile(outputFilePath, allPurchasesOutput.toString(), true, false);
    }
    /**
     * Gets the information about purchasing from the purchase text file, processes every purchase and writes the output to the output text file
     *
     * @param filePath              Stands for the purchase text file.
     */
    public static List<String> readPurchaseDetails(String filePath) {
        List<String> purchaseDetails = new ArrayList<>();

        String[] lines = FileInput.readFile(filePath, true, true);
        if (lines != null) {
            for (String line : lines) {
                String[] parts = line.split("\t");
                if (parts.length == 4) {
                    purchaseDetails.add(line); // Adding valid purchase detail line to the list
                } else {
                    System.out.println("Invalid purchase detail: " + line); // Notifying of invalid format
                }
            }
        }

        return purchaseDetails;
    }
    /**
     * Gets the information about purchasing from the purchase text file, processes every purchase and writes the output to the output text file
     */
    public static String processPurchase(String input, int totalMoney, String[] moneys, String choiceType, int value, Slot[] slots) {
        StringBuilder output = new StringBuilder();

        // Always starting with the INPUT line
        output.append(String.format("INPUT: %s\t%s\t%s\t%d\n", input, String.join(" ", moneys), choiceType, value));

        // Directing slot number choice
        if ("NUMBER".equals(choiceType.toUpperCase())) {
            if (value < 0 || value >= slots.length) {
                // Slot number does not exist
                output.append("INFO: Number cannot be accepted. Please try again with another number.\n");
            } else {
                Slot slot = slots[value];
                if (slot.getProduct() == null || slot.getProductCount() <= 0) {
                    // Slot exists but is empty
                    output.append("INFO: This slot is empty, your money will be returned.\n");
                } else if (totalMoney < slot.getProduct().getPrice()) {
                    // Insufficient money
                    output.append("INFO: Insufficient money, try again with more money.\n");
                } else {
                    // Successful purchase
                    output.append(String.format("PURCHASE: You have bought one %s\n", slot.getProduct().getName()));
                    slot.setProductCount(slot.getProductCount() - 1);
                    output.append(String.format("RETURN: Returning your change: %d TL\n", totalMoney - slot.getProduct().getPrice()));
                    return output.toString(); // Early return to finalize successful transaction
                }
            }
        } else {
            // Nutritional value choice
            boolean found = false;
            boolean insufficientMoney = false;
            for (Slot slot : slots) {
                if (slot.getProduct() != null && slot.getProductCount() > 0 && matchesCriteria(slot.getProduct(), choiceType, value)) {
                    found = true; // Product that meets criteria is found
                    if (totalMoney >= slot.getProduct().getPrice()) {
                        // Successful purchase
                        output.append(String.format("PURCHASE: You have bought one %s\n", slot.getProduct().getName()));
                        slot.setProductCount(slot.getProductCount() - 1);
                        output.append(String.format("RETURN: Returning your change: %d TL\n", totalMoney - slot.getProduct().getPrice()));
                        return output.toString(); // Successful transaction completed
                    } else {
                        // Found but insufficient money
                        insufficientMoney = true;
                        break;
                    }
                }
            }
            if (!found) {
                // No product matches criteria
                output.append("INFO: Product not found, your money will be returned.\n");
            } else if (insufficientMoney) {
                // Insufficient money for a found product
                output.append("INFO: Insufficient money, try again with more money.\n");
            }
        }

        // Append RETURN line for unsuccessful transaction attempts without a specific RETURN line
        if (!output.toString().contains("RETURN:")) {
            output.append(String.format("RETURN: Returning your money: %d TL\n", totalMoney));
        }

        return output.toString();
    }

    private static boolean matchesCriteria(Product product, String choiceType, double targetValue) {
        double tolerance = 5.0; // Tolerance range
        switch (choiceType.toUpperCase()) {
            case "PROTEIN":
                return Math.abs(product.getProtein() - targetValue) <= tolerance;
            case "CARB":
                return Math.abs(product.getCarbohydrate() - targetValue) <= tolerance;
            case "FAT":
                return Math.abs(product.getFat() - targetValue) <= tolerance;
            case "CALORIE":
                return Math.abs(product.getCalorie() - targetValue) <= tolerance;
            default:
                return false; // Unknown criteria
        }
    }
}