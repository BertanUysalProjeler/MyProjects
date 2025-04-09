import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.PrintStream;

public class FileOutput {
    /**
     * This method writes given content to file at given path.
     *
     * @param path    Path for the file content is going to be written.
     * @param content Content that is going to be written to file.
     * @param append  Append status, true if wanted to append to file if it exists, false if wanted to create file from zero.
     * @param newLine True if wanted to append a new line after content, false if vice versa.
     */
    public static void writeToFile(String path, String content, boolean append, boolean newLine) {
        PrintStream ps = null;
        try {
            ps = new PrintStream(new FileOutputStream(path, append));
            ps.print(content);
            if (newLine) {
                ps.println();
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
        }
    }
}