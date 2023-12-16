package se465;

import com.google.common.base.Preconditions;

import java.util.ArrayList;

public abstract class AbstractDeltaDebugger {

  public abstract Input ddmin(Input input, PropertyTest property);

  protected static Partitions partition(Input s, int numOfPartitions) {
    final int length = s.length();
    Preconditions.checkArgument(length >= numOfPartitions);
    final int partitionSize = partitionSize(length, numOfPartitions);
    int remainder = length % numOfPartitions;
    final Partitions result = new Partitions();
    int start = 0;
    while (start < length) {
      int end = start + partitionSize;
      if (remainder > 0) {
        end++;
        remainder--;
      }

      if (end > length) {
        end = length;
      }
      result.partitions.add(s.substring(start, end));
      result.complements.add(s.substring(0, start).combine(s.substring(end)));
      start = end;
    }
    return result;
  }

  protected static int partitionSize(int stringLength, int numberOfPartitions) {
    Preconditions.checkArgument(stringLength > 0);
    Preconditions.checkArgument(numberOfPartitions > 0);
    final int partitionSize = (stringLength) / numberOfPartitions;
    return partitionSize;
  }

  protected static class Partitions {
    final ArrayList<Input> partitions = new ArrayList<>();
    final ArrayList<Input> complements = new ArrayList<>();
  }
}
