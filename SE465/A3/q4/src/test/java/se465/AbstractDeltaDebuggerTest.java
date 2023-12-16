package se465;

import com.google.common.collect.ImmutableList;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;
import se465.AbstractDeltaDebugger.Partitions;

import java.util.List;

import static com.google.common.truth.Truth.assertThat;
import static org.junit.Assert.assertEquals;
import static se465.AbstractDeltaDebugger.partitionSize;

@RunWith(JUnit4.class)
public class AbstractDeltaDebuggerTest {

  @Test
  public void test_partitionSize() {
    assertEquals(1, partitionSize(2, 2));
    assertEquals(2, partitionSize(4, 2));
    assertEquals(1, partitionSize(4, 3));
  }

  @Test
  public void test_partition_2_partitions() {
    Partitions result = partition("ab", 2);
    assertThat(toStrings(result.partitions)).containsExactly("a", "b").inOrder();

    result = partition("abc", 2);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "c").inOrder();

    result = partition("abcd", 2);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "cd").inOrder();

    result = partition("abcdefgh", 2);
    assertThat(toStrings(result.partitions)).containsExactly("abcd", "efgh").inOrder();
  }

  private Partitions partition(String s, int numOfPartitions) {
    return AbstractDeltaDebugger.partition(new Input(s), numOfPartitions);
  }

  @Test
  public void test_partition_3_partitions() {
    Partitions result = partition("abc", 3);
    assertThat(toStrings(result.partitions)).containsExactly("a", "b", "c").inOrder();

    result = partition("abcd", 3);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "c", "d").inOrder();

    result = partition("abcdefgh", 3);
    assertThat(toStrings(result.partitions)).containsExactly("abc", "def", "gh").inOrder();
  }

  @Test
  public void test_partition_4_partitions() {
    Partitions result = partition("abcd", 4);
    assertThat(toStrings(result.partitions)).containsExactly("a", "b", "c", "d").inOrder();

    result = partition("abcde", 4);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "c", "d", "e").inOrder();

    result = partition("abcdef", 4);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "cd", "e", "f").inOrder();

    result = partition("abcdefg", 4);
    assertThat(toStrings(result.partitions)).containsExactly("ab", "cd", "ef", "g").inOrder();
  }

  private ImmutableList<String> toStrings(List<Input> partitions) {
    return partitions.stream().map(Input::toString).collect(ImmutableList.toImmutableList());
  }
}
