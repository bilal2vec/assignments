package se465;

import com.google.common.base.Joiner;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.Iterables;

public final class Input {

  private final ImmutableList<String> elements;
  private final String delimiter;

  public Input(String input) {
    this(
        input
            .chars()
            .mapToObj(c -> String.valueOf((char) c))
            .collect(ImmutableList.toImmutableList()),
        "");
  }

  public Input(ImmutableList<String> elements, String delimiter) {
    this.elements = elements;
    this.delimiter = delimiter;
  }

  public int length() {
    return elements.size();
  }

  public Input substring(int start, int end) {
    return new Input(elements.subList(start, end), delimiter);
  }

  public Input combine(Input other) {
    return new Input(ImmutableList.copyOf(Iterables.concat(elements, other.elements)), delimiter);
  }

  public Input substring(int start) {
    return new Input(elements.subList(start, elements.size()), delimiter);
  }

  @Override
  public String toString() {
    return Joiner.on(delimiter).join(elements);
  }
}
