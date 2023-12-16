package se465;

import com.google.common.io.MoreFiles;
import com.google.common.io.RecursiveDeleteOption;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import static com.google.common.truth.Truth.assertThat;

@RunWith(JUnit4.class)
public class SystemTest {

  private Path tempDir;
  private final String script = "src/test/resources/hello_world_bug/r.sh";
  private final String program = "src/test/resources/hello_world_bug/t.c";

  @Before
  public void setup() throws IOException {
    tempDir = Files.createTempDirectory("se465_delta_debugging_test");
  }

  @After
  public void teardown() throws IOException {
    MoreFiles.deleteRecursively(tempDir, RecursiveDeleteOption.ALLOW_INSECURE);
  }

  @Test
  public void test_hello_world_bug_with_char_based_ddmin() throws Exception {
    test("hello_world_bug", "t.c", "r.sh", "char", "golden_char_t.c");
  }

  @Test
  public void test_hello_world_bug_with_token_based_ddmin() throws Exception {
    test("hello_world_bug", "t.c", "r.sh", "token", "golden_token_t.c");
  }

  @Test
  public void test_select_bug_with_char_based_ddmin() throws Exception {
    test("select_bug", "t.html", "r.sh", "char", "golden_char_t.html");
  }

  @Test
  public void test_hello_world_bug_with_line_based_ddmin() throws Exception {
    test("hello_world_bug", "t.c", "r.sh", "line", "golden_line_t.c");
  }

  @Test
  public void test_rust_bug_1_with_line_based_ddmin() throws Exception {
    test("rust_bug1", "t.rs", "r.sh", "line", "golden_line_t.rs");
  }

  @Test
  public void test_rust_bug_2_with_line_based_ddmin() throws Exception{
    test("rust_bug2", "t.rs", "r.sh", "line", "golden_line_t.rs");
  }

  private void test(
      String testDirectoryName,
      String programFileName,
      String scriptFileName,
      String granularity,
      String goldenFileName)
      throws Exception {
    final Path resourceDir = Paths.get("src/test/resources");

    final Path testDirectory = resourceDir.resolve(testDirectoryName);
    final Path programFile = testDirectory.resolve(programFileName);
    final Path scriptFile = testDirectory.resolve(scriptFileName);
    final Path goldenFile = testDirectory.resolve(goldenFileName);

    assertThat(Files.isRegularFile(goldenFile)).isTrue();

    final Path resultFile =
        tempDir.resolve(granularity + "_" + testDirectoryName + "_" + programFileName + ".result");
    Main.main(
        new String[] {
          granularity, scriptFile.toString(), programFile.toString(), resultFile.toString()
        });
    String testOutput = MoreFiles.asCharSource(resultFile, StandardCharsets.UTF_8).read().trim();
    final String golden = MoreFiles.asCharSource(goldenFile, StandardCharsets.UTF_8).read().trim();
    assertThat(testOutput).isEqualTo(golden);
  }
}
