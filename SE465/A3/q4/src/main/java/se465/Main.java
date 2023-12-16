package se465;

import com.google.common.collect.ImmutableList;
import com.google.common.io.MoreFiles;
import org.antlr.v4.runtime.Token;
import org.perses.grammar.c.PnfCParserFacade;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

import static com.google.common.base.Preconditions.checkArgument;
import static com.google.common.base.Preconditions.checkState;

public class Main {

  private final Granularity granularity;
  private final Path scriptFile;
  private final Path inputFile;
  private final AbstractDeltaDebugger debugger = new DeltaDebugger();
  private final PropertyTest tester;
  private final Input input;

  public Main(Granularity granularity, Path scriptFile, Path inputFile) throws IOException {
    checkArgument(Files.isRegularFile(scriptFile), "The script file %s is not a file.", scriptFile);
    checkArgument(
        Files.isExecutable(scriptFile), "The script file %s is not executable.", scriptFile);
    checkArgument(Files.isRegularFile(inputFile), "The input file %s is not a file.", inputFile);

    this.granularity = granularity;
    this.scriptFile = scriptFile;
    this.inputFile = inputFile;
    tester = new PropertyTest(scriptFile, inputFile.getFileName().toString());
    input = createInput(this.granularity, this.inputFile);

    if (tester.test(input) != PropertyTest.TestResult.INTERESTING) {
      throw new RuntimeException("The original input does not pass the reduction script");
    }
  }

  public int numOfPropertyTests() {
    return tester.getCounter();
  }

  public Input run() {
    final Input result = debugger.ddmin(input, tester);
    checkState(tester.test(result) == PropertyTest.TestResult.INTERESTING);
    return result;
  }

  public void runAndSave(Path outputFile) throws IOException {
    final String result = run().toString();
    MoreFiles.asCharSink(outputFile, StandardCharsets.UTF_8)
        .write("#property tests: " + tester.getCounter() + ". " + result);
    System.out.println("The result is saved to " + outputFile);
  }

  public static void main(String[] args) throws Exception {
    if (args.length != 4) {
      throw new RuntimeException(
          "Usage: java -jar q4.jar "
              + Main.class.toString()
              + "<token|char|line> <script file> <input file> <output file>");
    }
    final Granularity type = Granularity.parse(args[0]);
    final Path scriptFile = Paths.get(args[1]);
    final Path inputFile = Paths.get(args[2]);
    final Path outputFile = Paths.get(args[3]);

    new Main(type, scriptFile, inputFile).runAndSave(outputFile);
  }

  public static Input createInput(Granularity type, String inputString) throws IOException {
    Path temp = Files.createTempFile("prefix", "suffix");
    Files.write(temp, inputString.getBytes(StandardCharsets.UTF_8));
    final Input result = createInput(type, temp);
    Files.delete(temp);
    return result;
  }

  public static Input createInput(Granularity type, Path inputFile) throws IOException {
    switch (type) {
      case TOKEN:
        PnfCParserFacade facade = new PnfCParserFacade();
        ImmutableList<Token> tokens = facade.parseIntoTokens(inputFile.toFile());
        return new Input(
            tokens.stream().map(Token::getText).collect(ImmutableList.toImmutableList()), " ");
      case CHAR:
        return new Input(MoreFiles.asCharSource(inputFile, StandardCharsets.UTF_8).read());
      case LINE:
        return new Input(
            MoreFiles.asCharSource(inputFile, StandardCharsets.UTF_8).readLines(), "\n");
      default:
        throw new RuntimeException("unhandled type: " + type);
    }
  }
}
