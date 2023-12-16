package se465;

class M {
  public void m(String arg, int i) {
    int q = 1;
    A o = null;

    if (i == 0)
      q = 4;
    q++;
    switch (arg.length()) {
      case 0:
        q /= 2;
        break;
      case 1:
        o = new A();
        q = 10;
        break;
      case 2:
        o = new B();
        q = 5;
        break;
      case 3:
        o = new A();
        new B();
        q = 25;
        break;
      default:
        o = new B();
        break;
    }
    if (arg.length() > 0) {
      o.m();
    } else {
      System.out.println("zero");
    }
  }
}
