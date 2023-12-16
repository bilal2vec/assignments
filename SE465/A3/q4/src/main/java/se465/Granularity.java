package se465;

public enum Granularity {
  CHAR("char"),
  TOKEN("token"),
  LINE("line");

  public final String simpleName;

  Granularity(String simpleName) {
    this.simpleName = simpleName;
  }

  public static Granularity parse(String string) {
    switch (string.trim().toLowerCase()) {
      case "token":
        return TOKEN;
      case "line":
        return LINE;
      case "char":
        return CHAR;
    }
    throw new RuntimeException("unhandled type: " + string);
  }
}
