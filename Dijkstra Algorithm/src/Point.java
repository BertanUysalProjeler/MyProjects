
public class Point {
    private int currentShortestLength = Integer.MAX_VALUE;
    private String name;

    /**
     * Constructor to create a new point with a given name.
     *
     * @param name the name of the point.
     */
    public Point(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public int getCurrentShortestLength() {
        return currentShortestLength;
    }

    public void setCurrentShortestLength(int currentShortestLength) {
        this.currentShortestLength = currentShortestLength;
    }
}
