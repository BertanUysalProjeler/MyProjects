import java.util.Arrays;
import java.util.List;

public class AnObject {
    private int length;
    private Point firstPoint;
    private Point secondPoint;
    private List<Point> ends;
    private int id;

    /**
     * Constructor to create a new edge (AnObject) with specified length, points, and ID.
     *
     * @param length     the length of the edge.
     * @param firstPoint the first endpoint of the edge.
     * @param secondPoint the second endpoint of the edge.
     * @param id         the unique identifier for the edge.
     */
    public AnObject(int length, Point firstPoint, Point secondPoint, int id) {
        this.firstPoint = firstPoint;
        this.secondPoint = secondPoint;
        this.length = length;
        this.ends = Arrays.asList(firstPoint, secondPoint);
        this.id = id;
    }

    public int getLength() {
        return length;
    }

    public int getId() {
        return id;
    }

    public List<Point> getEnds() {
        return ends;
    }

    public Point getFirstPoint() {
        return firstPoint;
    }

    public Point getSecondPoint() {
        return secondPoint;
    }

    /**
     * Checks if the edge contains a given point.
     *
     * @param point the point to check.
     * @return true if the edge contains the point, false otherwise.
     */
    public boolean contains(Point point) {
        return ends.contains(point);
    }

    /**
     * Returns the other endpoint of the edge given one endpoint.
     *
     * @param point one endpoint of the edge.
     * @return the other endpoint of the edge.
     */
    public Point getOtherEnd(Point point) {
        if (point.equals(firstPoint)) {
            return secondPoint;
        } else {
            return firstPoint;
        }
    }
}