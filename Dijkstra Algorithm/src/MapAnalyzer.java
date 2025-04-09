import java.util.*;

public class MapAnalyzer {
    private static List<Point> visited = new ArrayList<>();
    private static List<Point> unvisited = new ArrayList<>();
    private static List<Point> pointList = new ArrayList<>();
    private static List<AnObject> anObjectList = new ArrayList<>();
    private static Map<Point, AnObject> path = new HashMap<>();
    private static Map<Point, List<AnObject>> paths = new HashMap<>();
    private static List<AnObject> mstEdges = new ArrayList<>();
    static StringBuilder mainOutput = new StringBuilder();

    /**
     * Main method to execute the Dijkstra and Prim algorithms.
     *
     * @param args the command line arguments, where args[0] is the input file path and args[1] is the output file path.
     */
    public static void main(String[] args) {
        Locale.setDefault(Locale.US);
        String[] lines = FileInput.readFile(args[0], true, true);
        PointListBuilder(lines, getPointList());
        AnObjectListBuilder(lines, getAnObjectList());
        getUnvisited().addAll(getPointList());
        InputProcessor(lines, args[1]);
    }

    /**
     * Processes the input data, executes Dijkstra and Prim algorithms, and writes the output to a file.
     *
     * @param lines      the lines of input data.
     * @param outputPath the path to the output file.
     */
    public static void InputProcessor(String[] lines, String outputPath) {
        // Parse starting and finishing points from the first line
        String[] splitLine = lines[0].split("\t");
        String startingPoint = splitLine[0];
        String finishingPoint = splitLine[1];
        Point startPoint = null;
        Point finishPoint = null;
        StringBuilder output1 = new StringBuilder();

        // Find start and finish points in the point list
        for (Point point : getPointList()) {
            if (point.getName().equals(startingPoint)) {
                startPoint = point;
            }
            if (point.getName().equals(finishingPoint)) {
                finishPoint = point;
            }
        }

        // Execute Dijkstra's algorithm on the original map
        ShortestPathBuilder(startPoint, finishPoint);
        int originalMapDistance = finishPoint.getCurrentShortestLength();
        output1.append("Fastest Route from ").append(startingPoint).append(" to ").append(finishingPoint)
                .append(" (").append(originalMapDistance).append(" KM):\n");

        // Reconstruct the path from the finish point to the start point
        Point current = finishPoint;
        Stack<AnObject> pathStack = new Stack<>();
        while (path.containsKey(current)) {
            AnObject edge = path.get(current);
            pathStack.push(edge);
            current = edge.getOtherEnd(current);
        }
        while (!pathStack.isEmpty()) {
            AnObject edge = pathStack.pop();
            output1.append(edge.getFirstPoint().getName())
                    .append("\t").append(edge.getSecondPoint().getName())
                    .append("\t").append(edge.getLength())
                    .append("\t").append(edge.getId())
                    .append("\n");
        }

        // Execute Prim's algorithm to create the MST
        mstEdges = PrimAlgorithm(startPoint);
        StringBuilder output3 = new StringBuilder();
        output3.append("Roads of Barely Connected Map is:\n");
        mstEdges.sort(Comparator.comparingInt(AnObject::getLength)
                .thenComparingInt(AnObject::getId));
        int mstTotalLength = 0;
        for (AnObject edge : mstEdges) {
            mstTotalLength += edge.getLength();
            output3.append(edge.getFirstPoint().getName())
                    .append("\t").append(edge.getSecondPoint().getName())
                    .append("\t").append(edge.getLength())
                    .append("\t").append(edge.getId())
                    .append("\n");
        }
        output3.append("Fastest Route from ").append(startingPoint).append(" to ").append(finishingPoint)
                .append(" on Barely Connected Map (");

        // Execute Dijkstra's algorithm on the MST
        path.clear();
        paths.clear();
        getVisited().clear();
        getUnvisited().clear();
        getUnvisited().addAll(getPointList());
        for (Point point : getPointList()) {
            point.setCurrentShortestLength(Integer.MAX_VALUE);
        }
        ShortestPathBuilderMST(startPoint, finishPoint);
        int mstMapDistance = finishPoint.getCurrentShortestLength();
        int totalRouteLength = 0;
        current = finishPoint;
        pathStack = new Stack<>();
        while (path.containsKey(current)) {
            AnObject edge = path.get(current);
            pathStack.push(edge);
            totalRouteLength += edge.getLength();
            current = edge.getOtherEnd(current);
        }
        output3.append(totalRouteLength).append(" KM):\n");
        while (!pathStack.isEmpty()) {
            AnObject edge = pathStack.pop();
            output3.append(edge.getFirstPoint().getName())
                    .append("\t").append(edge.getSecondPoint().getName())
                    .append("\t").append(edge.getLength())
                    .append("\t").append(edge.getId())
                    .append("\n");
        }

        StringBuilder output2 = new StringBuilder();
        // Analysis
        double constructionMaterialRatio = (double) mstTotalLength / getTotalLength();
        double fastestRouteRatio = (double) mstMapDistance / originalMapDistance;
        output2.append("Analysis:\n");
        output2.append(String.format("Ratio of Construction Material Usage Between Barely Connected and Original Map: %.2f\n", constructionMaterialRatio));
        output2.append(String.format("Ratio of Fastest Route Between Barely Connected and Original Map: %.2f\n", fastestRouteRatio));

        getMainOutput().append(output1);
        getMainOutput().append(output3);
        getMainOutput().append(output2);

        // Write to file without adding extra newline at the end
        String finalOutput = getMainOutput().toString().trim();
        FileOutput.writeToFile(outputPath, finalOutput, false, false);
    }

    /**
     * Implements Dijkstra's algorithm to find the shortest path between startPoint and finishPoint.
     *
     * @param startPoint   the starting point.
     * @param finishPoint  the finishing point.
     */
    public static void ShortestPathBuilder(Point startPoint, Point finishPoint) {
        startPoint.setCurrentShortestLength(0);
        PriorityQueue<Point> pq = new PriorityQueue<>(Comparator.comparingInt(Point::getCurrentShortestLength));
        pq.add(startPoint);

        while (!pq.isEmpty()) {
            Point currentPoint = pq.poll();
            if (currentPoint.equals(finishPoint)) {
                break;
            }
            for (AnObject anObject : getAnObjectList()) {
                if (anObject.contains(currentPoint)) {
                    Point neighbor = anObject.getOtherEnd(currentPoint);
                    int newDist = currentPoint.getCurrentShortestLength() + anObject.getLength();
                    if (newDist < neighbor.getCurrentShortestLength() ||
                            (newDist == neighbor.getCurrentShortestLength() && betterPath(currentPoint, neighbor, anObject))) {
                        neighbor.setCurrentShortestLength(newDist);
                        path.put(neighbor, anObject);
                        updatePath(currentPoint, neighbor, anObject);
                        pq.add(neighbor);
                    }
                }
            }
        }
    }

    /**
     * Implements Dijkstra's algorithm on the Minimum Spanning Tree (MST) to find the shortest path.
     *
     * @param startPoint   the starting point.
     * @param finishPoint  the finishing point.
     */
    public static void ShortestPathBuilderMST(Point startPoint, Point finishPoint) {
        startPoint.setCurrentShortestLength(0);
        PriorityQueue<Point> pq = new PriorityQueue<>(Comparator.comparingInt(Point::getCurrentShortestLength));
        pq.add(startPoint);

        while (!pq.isEmpty()) {
            Point currentPoint = pq.poll();
            if (currentPoint.equals(finishPoint)) {
                break;
            }
            for (AnObject anObject : mstEdges) {
                if (anObject.contains(currentPoint)) {
                    Point neighbor = anObject.getOtherEnd(currentPoint);
                    int newDist = currentPoint.getCurrentShortestLength() + anObject.getLength();
                    if (newDist < neighbor.getCurrentShortestLength() ||
                            (newDist == neighbor.getCurrentShortestLength() && betterPath(currentPoint, neighbor, anObject))) {
                        neighbor.setCurrentShortestLength(newDist);
                        path.put(neighbor, anObject);
                        updatePath(currentPoint, neighbor, anObject);
                        pq.add(neighbor);
                    }
                }
            }
        }
    }

    /**
     * Updates the paths map with the current point, neighbor, and the edge between them.
     *
     * @param currentPoint the current point.
     * @param neighbor     the neighboring point.
     * @param anObject     the edge between currentPoint and neighbor.
     */
    public static void updatePath(Point currentPoint, Point neighbor, AnObject anObject) {
        List<AnObject> newPath = new ArrayList<>(paths.getOrDefault(currentPoint, new ArrayList<>()));
        newPath.add(anObject);
        paths.put(neighbor, newPath);
    }

    /**
     * Determines if the new path is better than the existing path to the neighbor.
     *
     * @param currentPoint the current point.
     * @param neighbor     the neighboring point.
     * @param newEdge      the edge being considered.
     * @return true if the new path is better, false otherwise.
     */
    public static boolean betterPath(Point currentPoint, Point neighbor, AnObject newEdge) {
        List<AnObject> existingPath = paths.get(neighbor);
        List<AnObject> newPath = new ArrayList<>(paths.getOrDefault(currentPoint, new ArrayList<>()));
        newPath.add(newEdge);

        if (existingPath == null) {
            return true;
        }

        for (int i = 0; i < Math.min(existingPath.size(), newPath.size()); i++) {
            if (newPath.get(i).getId() < existingPath.get(i).getId()) {
                return true;
            } else if (newPath.get(i).getId() > existingPath.get(i).getId()) {
                return false;
            }
        }
        return newPath.size() < existingPath.size();
    }

    /**
     * Implements Prim's algorithm to find the Minimum Spanning Tree (MST).
     *
     * @param startPoint the starting point.
     * @return a list of edges that form the MST.
     */
    public static List<AnObject> PrimAlgorithm(Point startPoint) {
        List<Point> mstSet = new ArrayList<>();
        List<AnObject> mstEdges = new ArrayList<>();
        PriorityQueue<AnObject> edgeQueue = new PriorityQueue<>(Comparator.comparingInt(AnObject::getLength)
                .thenComparingInt(AnObject::getId));

        mstSet.add(startPoint);
        for (AnObject edge : getAnObjectList()) {
            if (edge.contains(startPoint)) {
                edgeQueue.add(edge);
            }
        }

        while (!edgeQueue.isEmpty()) {
            AnObject minEdge = edgeQueue.poll();
            Point point1 = minEdge.getFirstPoint();
            Point point2 = minEdge.getSecondPoint();

            if (mstSet.contains(point1) && mstSet.contains(point2)) {
                continue;
            }

            mstEdges.add(minEdge);

            Point newPoint = mstSet.contains(point1) ? point2 : point1;
            mstSet.add(newPoint);

            for (AnObject edge : getAnObjectList()) {
                if (edge.contains(newPoint) && !mstSet.contains(edge.getOtherEnd(newPoint))) {
                    edgeQueue.add(edge);
                }
            }
        }
        return mstEdges;
    }

    /**
     * Builds the list of points from the input data.
     *
     * @param lines     the input data lines.
     * @param pointList the list of points to be populated.
     */
    public static void PointListBuilder(String[] lines, List<Point> pointList) {
        for (int i = 1; i < lines.length; i++) {
            String[] splitLine = lines[i].split("\t");
            String firstEnd = splitLine[0];
            String secondEnd = splitLine[1];

            if (pointList.stream().noneMatch(point -> point.getName().equals(firstEnd))) {
                pointList.add(new Point(firstEnd));
            }

            if (pointList.stream().noneMatch(point -> point.getName().equals(secondEnd))) {
                pointList.add(new Point(secondEnd));
            }
        }
    }

    /**
     * Builds the list of edges (AnObject) from the input data.
     *
     * @param lines        the input data lines.
     * @param anObjectList the list of edges to be populated.
     */
    public static void AnObjectListBuilder(String[] lines, List<AnObject> anObjectList) {
        for (int i = 1; i < lines.length; i++) {
            String[] splitLine = lines[i].split("\t");
            String firstEnd = splitLine[0];
            String secondEnd = splitLine[1];
            int length = Integer.parseInt(splitLine[2]);
            int id = Integer.parseInt(splitLine[3]);
            Point firstEndPoint = null;
            Point secondEndPoint = null;
            for (Point point : getPointList()) {
                if (point.getName().equals(firstEnd)) {
                    firstEndPoint = point;
                }
                if (point.getName().equals(secondEnd)) {
                    secondEndPoint = point;
                }
            }
            AnObject anObject = new AnObject(length, firstEndPoint, secondEndPoint, id);
            anObjectList.add(anObject);
        }
    }

    /**
     * Returns the total length of all edges in the graph.
     *
     * @return the total length of all edges.
     */
    public static int getTotalLength() {
        return anObjectList.stream().mapToInt(AnObject::getLength).sum();
    }

    public static List<Point> getVisited() {
        return visited;
    }

    public static List<Point> getPointList() {
        return pointList;
    }

    public static List<AnObject> getAnObjectList() {
        return anObjectList;
    }

    public static List<Point> getUnvisited() {
        return unvisited;
    }

    public static StringBuilder getMainOutput() {
        return mainOutput;
    }
}
