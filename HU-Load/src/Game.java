import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.image.Image;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import javafx.scene.paint.Color;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.util.Duration;
import javafx.scene.control.Label;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.text.Font;

import java.util.Random;

public class Game extends Application {
    private Character character;
    private Image characterRightImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\drill\\drill_55.png");
    private Image characterLeftImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\drill\\drill_53.png");
    private Image characterUpImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\drill\\drill_27.png");
    private Image characterDownImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\drill\\drill_43.png");
    private Image soilImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\soil_04.png");
    private Image topSoilImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\top_02.png");
    private Image boulderImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\obstacle_03.png");
    private Image lavaImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\lava_03.png");
    private Image amazoniteImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\valuable_amazonite.png");
    private Image diamondImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\valuable_diamond.png");
    private Image EinsteiniumImage = new Image("file:C:\\Users\\berta\\Downloads\\BBM104_S24_PA3_Assets\\assets\\underground\\valuable_einsteinium.png");
    private Image currentCharacterImage = characterLeftImage;
    private static final int NUM_CELLS_X = 16;
    private static final int NUM_CELLS_Y = 15;
    private int[][] originalGrid = new int[NUM_CELLS_X][NUM_CELLS_Y];
    private int[][] grid = new int[NUM_CELLS_X][NUM_CELLS_Y];
    // New attributes for the values of the valuables
    private static final double AMAZONITE_MASS = 120;
    private static final double AMAZONITE_VALUE = 500000;
    private static final double EINSTEINIUM_MASS = 40;
    private static final double EINSTEINIUM_VALUE = 2000;
    private static final double DIAMOND_MASS = 100;
    private static final double DIAMOND_VALUE = 100000;

    private void displayValues(GraphicsContext gc) {
        double xPosition = 10;
        double yPosition = 20;
        double width = 200; // Adjust this width to cover the maximum possible width of the text
        double height = 80; // Adjust this height based on the font size and number of lines

        // Set fill for semi-transparent rectangle
        gc.setFill(new Color(0, 0, 0, 0.5)); // Black with 50% opacity
        gc.fillRect(xPosition, yPosition, width, height);

        // Set fill color for text
        gc.setFill(Color.WHITE);
        gc.setFont(new Font("Arial", 20));

        // Display the new values
        gc.fillText("Fuel: " + character.getFuel(), xPosition, yPosition + 20);
        gc.fillText("Haul: " + character.getHaul(), xPosition, yPosition + 40);
        gc.fillText("Money: " + character.getMoneyBank(), xPosition, yPosition + 60);
    }

    @Override
    public void start(Stage primaryStage) {
        double cellWidth = soilImage.getWidth();
        double cellHeight = soilImage.getHeight();
        double sceneWidth = cellWidth * NUM_CELLS_X;
        double sceneHeight = cellHeight * NUM_CELLS_Y;

        character = new Character(7,1,7 * cellWidth, cellHeight, cellWidth, cellHeight);

        Canvas canvas = new Canvas(sceneWidth, sceneHeight);
        GraphicsContext gc = canvas.getGraphicsContext2D();

        // Create counters for each element
        int boulderCount = 0;
        int lavaCount = 0;
        int mineral1Count = 0;
        int mineral2Count = 0;
        int mineral3Count = 0;

        // Populate the array with random elements
        Random rand = new Random();
        for (int i = 2; i < NUM_CELLS_X - 2; i++) {
            for (int j = 4; j < NUM_CELLS_Y - 1; j++) {
                int num = rand.nextInt(250);
                if (num < 60 || (boulderCount >= 3 && lavaCount >= 10 && mineral1Count >= 3 && mineral2Count >= 3 && mineral3Count >= 3)) {
                    grid[i][j] = 0; // Soil
                } else if (num < 75 && boulderCount < 3) {
                    grid[i][j] = 1; // Boulder
                    boulderCount++;
                } else if (num < 85 && lavaCount < 10) {
                    grid[i][j] = 2; // Lava
                    lavaCount++;
                } else if (num < 92 && mineral1Count < 3) {
                    grid[i][j] = 3; // Mineral 1
                    mineral1Count++;
                } else if (num < 97 && mineral2Count < 3) {
                    grid[i][j] = 4; // Mineral 2
                    mineral2Count++;
                } else if (mineral3Count < 3) {
                    grid[i][j] = 5; // Mineral 3
                    mineral3Count++;
                }
            }
        }
        for (int i = 0; i < NUM_CELLS_X; i++) {
            System.arraycopy(grid[i], 0, originalGrid[i], 0, NUM_CELLS_Y);
        }

        // Draw the cells
        for (int i = 0; i < NUM_CELLS_X; i++) {
            for (int j = 0; j < NUM_CELLS_Y; j++) {
                if (j < 2) {
                    grid[i][j] = -1;
                    // Paint the first two rows blue
                    gc.setFill(Color.BLUE);
                    gc.fillRect(i * cellWidth, j * cellHeight, cellWidth , cellHeight);
                } else if (j == 2) {
                    grid[i][j] = 7;
                    // Draw the top soil image for the third row
                    gc.setFill(Color.BLUE);
                    gc.fillRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                    gc.drawImage(topSoilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                } else if ((i == 0 || i == NUM_CELLS_X - 1) && j >= 3) {
                    grid[i][j] = 1;
                    // Draw the boulder image for the boundary cells, excluding the first 3 rows
                    gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                } else if (j == NUM_CELLS_Y - 1) {
                    grid[i][j] = 1;
                    // Draw the boulder image for the last row
                    gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                } else {
                    // Draw the original soil image for the rest of the cells
                    switch (grid[i][j]) {
                        case 0:
                            gc.drawImage(soilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                        case 1:
                            gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                        case 2:
                            gc.drawImage(lavaImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                        case 3:
                            gc.drawImage(amazoniteImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                        case 4:
                            gc.drawImage(diamondImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                        case 5:
                            gc.drawImage(EinsteiniumImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                            break;
                    }
                }
            }
        }

        // Draw the character
        gc.drawImage(characterRightImage, character.getX(), character.getY(), cellWidth + 20, cellHeight + 20);

        // Create a new Timeline object for gravity effect
        Timeline gravity = new Timeline(new KeyFrame(Duration.seconds(1), e -> {
            displayValues(gc);
            // Decrease fuel
            character.decreaseFuel(1);

            if (character.getFuel() <= 0) {
                displayGameOverScreenFuel(primaryStage); // Game over due to fuel depletion
            }

            // Check if the cell below the character is drilled soil
            if (grid[character.getGridX()][character.getGridY() + 1] == 6 || grid[character.getGridX()][character.getGridY() + 1] == 2 || grid[character.getGridX()][character.getGridY() + 1] == -1) {
                // Move the character down by one cell
                character.move(0, 1);
                character.moveGrid(0, 1);
                if (grid[character.getGridX()][character.getGridY()] == 2) {
                    displayGameOverScreen(primaryStage);
                }
                // Clear the canvas and redraw everything
                gc.clearRect(0, 0, sceneWidth, sceneHeight);
                displayValues(gc);
                for (int i = 0; i < NUM_CELLS_X; i++) {
                    for (int j = 0; j < NUM_CELLS_Y; j++) {
                        if (j < 2) {
                            // Paint the first two rows blue
                            gc.setFill(Color.BLUE);
                            gc.fillRect(i * cellWidth, j * cellHeight, cellWidth , cellHeight);
                        } else if ((i == 0 || i == NUM_CELLS_X - 1) && j >= 3) {
                            // Draw the boulder image for the boundary cells, excluding the first 3 rows
                            gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                        } else if (j == NUM_CELLS_Y - 1) {
                            // Draw the boulder image for the last row
                            gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                        } else {
                            // Draw the original soil image for the rest of the cells
                            switch (grid[i][j]) {
                                case 0:
                                    gc.drawImage(soilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 1:
                                    gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 2:
                                    gc.drawImage(lavaImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 3:
                                    gc.drawImage(amazoniteImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 4:
                                    gc.drawImage(diamondImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 5:
                                    gc.drawImage(EinsteiniumImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 6:
                                    gc.setFill(Color.BROWN);
                                    gc.fillRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                                case 7:
                                    gc.setFill(Color.BLUE);
                                    gc.fillRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    gc.drawImage(topSoilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                    break;
                            }
                        }
                    }
                }
                // Draw the character with the current image
                if (currentCharacterImage == characterRightImage) {
                    gc.drawImage(characterRightImage, character.getX(), character.getY(), cellWidth + 20, cellHeight + 20);
                } else if (currentCharacterImage == characterLeftImage) {
                    gc.drawImage(characterLeftImage, character.getX() - 15 , character.getY(), cellWidth + 20, cellHeight + 20); // Adjust x coordinate
                } else if (currentCharacterImage == characterUpImage) {
                    gc.drawImage(characterUpImage, character.getX() - 15, character.getY() , cellWidth + 20, cellHeight + 20); // Adjust y coordinate
                } else {
                    gc.drawImage(characterDownImage, character.getX() - 15, character.getY() , cellWidth + 20, cellHeight + 20); // Adjust y coordinate
                }
                displayValues(gc);
            }
        }));

        // Set the cycle count to INDEFINITE, which means the animation will run until it is stopped
        gravity.setCycleCount(Timeline.INDEFINITE);

        // Start the animation
        gravity.play();


        Pane root = new Pane(canvas);
        Scene scene = new Scene(root, sceneWidth, sceneHeight); // Create a new scene with the canvas
        // Update the setOnKeyPressed event handler
        scene.setOnKeyPressed(e -> {

            // Decrease fuel when the character moves
            character.decreaseFuel(100);

            if (character.getFuel() <= 0) {
                displayGameOverScreenFuel(primaryStage); // Game over due to fuel depletion
            }

            switch (e.getCode()) {
                case UP:
                    if (grid[character.getGridX()][character.getGridY() - 1] != 1 && grid[character.getGridX()][character.getGridY() - 1] == 6 || grid[character.getGridX()][character.getGridY() - 1] != 1 && grid[character.getGridX()][character.getGridY() - 1] == -1) {
                        character.move(0, -1);
                        character.moveGrid(0, -1);
                        grid[character.getGridX()][character.getGridY()] = 6;
                    }
                    currentCharacterImage = characterUpImage;
                    break;
                case DOWN:
                    if (grid[character.getGridX()][character.getGridY() + 1] != 1 ) {
                        character.move(0, 1);
                        character.moveGrid(0, 1);
                        grid[character.getGridX()][character.getGridY()] = 6;
                    }
                    currentCharacterImage = characterDownImage;
                    break;
                case LEFT:
                    // Check if the left cell contains lava
                    if (grid[character.getGridX() - 1][character.getGridY()] == 2) {
                        displayGameOverScreen(primaryStage); // Game over due to drilling into lava
                    } else if (grid[character.getGridX() - 1][character.getGridY()] != 1) {
                        character.move(-1, 0);
                        character.moveGrid(-1, 0);
                        grid[character.getGridX()][character.getGridY()] = 6;
                    }
                    currentCharacterImage = characterLeftImage;
                    break;
                case RIGHT:
                    // Check if the right cell contains lava
                    if (grid[character.getGridX() + 1][character.getGridY()] == 2) {
                        displayGameOverScreen(primaryStage); // Game over due to drilling into lava
                    } else if (grid[character.getGridX() + 1][character.getGridY()] != 1) {
                        character.move(1, 0);
                        character.moveGrid(1, 0);
                        grid[character.getGridX()][character.getGridY()] = 6;
                    }
                    currentCharacterImage = characterRightImage;
                    break;


            }

            switch (originalGrid[character.getGridX()][character.getGridY()]) {
                case 3: // Amazonite
                    character.increaseHaul(AMAZONITE_MASS);
                    character.increaseMoneyBank(AMAZONITE_VALUE);
                    getOriginalGrid()[character.getGridX()][character.getGridY()] = 6;
                    break;
                case 4: // Diamond
                    character.increaseHaul(DIAMOND_MASS);
                    character.increaseMoneyBank(DIAMOND_VALUE);
                    getOriginalGrid()[character.getGridX()][character.getGridY()] = 6;
                    break;
                case 5: // Einsteinium
                    character.increaseHaul(EINSTEINIUM_MASS);
                    character.increaseMoneyBank(EINSTEINIUM_VALUE);
                    getOriginalGrid()[character.getGridX()][character.getGridY()] = 6;
                    break;
            }

            // Clear the canvas and redraw everything
            gc.clearRect(0, 0, sceneWidth, sceneHeight);
            for (int i = 0; i < NUM_CELLS_X; i++) {
                for (int j = 0; j < NUM_CELLS_Y; j++) {
                    if (j < 2) {
                        // Paint the first two rows blue
                        gc.setFill(Color.BLUE);
                        gc.fillRect(i * cellWidth, j * cellHeight, cellWidth , cellHeight);
                    } else if ((i == 0 || i == NUM_CELLS_X - 1) && j >= 3) {
                        // Draw the boulder image for the boundary cells, excluding the first 3 rows
                        gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                    } else if (j == NUM_CELLS_Y - 1) {
                        // Draw the boulder image for the last row
                        gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                    } else {
                        // Draw the original soil image for the rest of the cells
                        switch (grid[i][j]) {
                            case 0:
                                gc.drawImage(soilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 1:
                                gc.drawImage(boulderImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 2:
                                gc.drawImage(lavaImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 3:
                                gc.drawImage(amazoniteImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 4:
                                gc.drawImage(diamondImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 5:
                                gc.drawImage(EinsteiniumImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 6:
                                gc.setFill(Color.BROWN);
                                gc.fillRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                            case 7:
                                gc.setFill(Color.BLUE);
                                gc.fillRect(i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                gc.drawImage(topSoilImage, i * cellWidth, j * cellHeight, cellWidth, cellHeight);
                                break;
                        }
                    }
                }
            }

            // Draw the character with the current image
            if (currentCharacterImage == characterRightImage) {
                gc.drawImage(characterRightImage, character.getX(), character.getY(), cellWidth + 20, cellHeight + 20);
            } else if (currentCharacterImage == characterLeftImage) {
                gc.drawImage(characterLeftImage, character.getX() - 15 , character.getY(), cellWidth + 20, cellHeight + 20); // Adjust x coordinate
            } else if (currentCharacterImage == characterUpImage) {
                gc.drawImage(characterUpImage, character.getX() - 15, character.getY() , cellWidth + 20, cellHeight + 20); // Adjust y coordinate
            } else {
                gc.drawImage(characterDownImage, character.getX() - 15, character.getY() , cellWidth + 20, cellHeight + 20); // Adjust y coordinate
            }
            displayValues(gc);
        });

        primaryStage.setTitle("Game");
        primaryStage.setScene(scene);
        primaryStage.show();
        scene.getRoot().requestFocus();
    }

    public static void main(String[] args) {
        launch(args);
    }

    private void displayGameOverScreen(Stage primaryStage) {
        Pane gameOverPane = new Pane();
        gameOverPane.setBackground(new Background(new BackgroundFill(Color.RED, null, null)));

        Label gameOverLabel = new Label("Game Over");
        gameOverLabel.setFont(new Font("Arial", 50));
        gameOverLabel.setTextFill(Color.WHITE);
        gameOverLabel.layoutXProperty().bind(gameOverPane.widthProperty().subtract(gameOverLabel.widthProperty()).divide(2));
        gameOverLabel.layoutYProperty().bind(gameOverPane.heightProperty().subtract(gameOverLabel.heightProperty()).divide(2));

        gameOverPane.getChildren().add(gameOverLabel);

        Scene gameOverScene = new Scene(gameOverPane, primaryStage.getScene().getWidth(), primaryStage.getScene().getHeight());
        primaryStage.setScene(gameOverScene);
        primaryStage.show();
    }

    private void displayGameOverScreenFuel(Stage primaryStage) {
        Pane gameOverPane = new Pane();
        gameOverPane.setBackground(new Background(new BackgroundFill(Color.GREEN, null, null)));

        Label gameOverLabel = new Label("Game Over");
        gameOverLabel.setFont(new Font("Arial", 50));
        gameOverLabel.setTextFill(Color.WHITE);
        gameOverLabel.layoutXProperty().bind(gameOverPane.widthProperty().subtract(gameOverLabel.widthProperty()).divide(2));
        gameOverLabel.layoutYProperty().bind(gameOverPane.heightProperty().divide(2).subtract(30));

        Label moneyLabel = new Label("Money Collected: " + character.getMoneyBank());
        moneyLabel.setFont(new Font("Arial", 30));
        moneyLabel.setTextFill(Color.WHITE);
        moneyLabel.layoutXProperty().bind(gameOverPane.widthProperty().subtract(moneyLabel.widthProperty()).divide(2));
        moneyLabel.layoutYProperty().bind(gameOverPane.heightProperty().divide(2).add(30));

        gameOverPane.getChildren().addAll(gameOverLabel, moneyLabel);

        Scene gameOverScene = new Scene(gameOverPane, primaryStage.getScene().getWidth(), primaryStage.getScene().getHeight());
        primaryStage.setScene(gameOverScene);
        primaryStage.show();
    }

    public int[][] getOriginalGrid() {
        return originalGrid;
    }
}
