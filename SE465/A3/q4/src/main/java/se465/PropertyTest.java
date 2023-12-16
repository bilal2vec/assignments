package se465;

import com.google.common.io.MoreFiles;
import com.google.common.io.RecursiveDeleteOption;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.util.concurrent.atomic.AtomicInteger;

public class PropertyTest {

  enum TestResult {
    INTERESTING,
    NOT_INTERESTING
  }

  private int counter;

  private final Path originalScriptPath;
  private final String inputFileName;

  protected PropertyTest(Path originalScriptPath, String inputFileName) throws IOException {
    this.originalScriptPath = originalScriptPath;
    this.inputFileName = inputFileName;
  }

  public int getCounter() {
    return counter;
  }

  public final TestResult test(Input input) {
    ++counter;
    Path workingDir = null;
    try {
      workingDir = Files.createTempDirectory("se465_delta_debugger");
      final Path inputFile = workingDir.resolve(inputFileName);
      final Path scriptFile = workingDir.resolve(originalScriptPath.getFileName());
      Files.copy(
          originalScriptPath,
          scriptFile,
          StandardCopyOption.REPLACE_EXISTING,
          StandardCopyOption.COPY_ATTRIBUTES);
      Files.write(inputFile, input.toString().getBytes());
      final String cmd = "./" + scriptFile.getFileName().toString();
      final Process process =
          new ProcessBuilder().command(cmd).directory(workingDir.toFile()).inheritIO().start();
      process.getOutputStream().close();
      final int exitCode = process.waitFor();
      return exitCode == 0 ? TestResult.INTERESTING : TestResult.NOT_INTERESTING;
    } catch (IOException | InterruptedException e) {
      throw new RuntimeException(e);
    } finally {
      if (workingDir != null) {
        try {
          MoreFiles.deleteRecursively(workingDir, RecursiveDeleteOption.ALLOW_INSECURE);
        } catch (IOException e) {
          e.printStackTrace();
        }
      }
    }
  }
}
