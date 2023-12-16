package se465;

import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.JUnit4;

import org.junit.Before;
import org.junit.After;

import java.io.*;

@RunWith(JUnit4.class)
public class TestM {

  private ByteArrayOutputStream baos;
  private PrintStream old;

  @Before
  public void setUp() {
    baos = new ByteArrayOutputStream();
    PrintStream ps = new PrintStream(baos);
    old = System.out;
    System.setOut(ps);
  }

  @After
  public void tearDown() {
    System.out.flush();
    System.setOut(old);
  }

  /* The following tests cover Node Coverage */

  /* This test covers Nodes a, b, c, d, h, j, k */
  @Test
  public void testNodesABCDHJK() {

    M obj = new M();
    obj.m("", 0);

    Assert.assertEquals(baos.toString(), "zero\n");
  }

  /* This test covers Nodes a, c, e, h, i, k */
  @Test
  public void testNodesACEHIK() {

    M obj = new M();
    obj.m("a", 1);

    Assert.assertEquals(baos.toString(), "a\n");
  }

  /* This test covers Nodes a, c, f, g, h, i, k */
  @Test
  public void testNodesACFGHIK() {

    M obj = new M();
    obj.m("aa", 1);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* The following tests cover Edge Coverage */


  /* This test covers Edges [a, b], [b, c], [c, d], [d, h], [h, j], [j, k] */
  @Test
  public void testEdges1() {

    M obj = new M();
    obj.m("", 0);

    Assert.assertEquals(baos.toString(), "zero\n");
  }


  /* This test covers Edges [a, c], [c, e], [e, h], [h, i], [i, k]  */
  @Test
  public void testEdges2() {

    M obj = new M();
    obj.m("a", 1);

    Assert.assertEquals(baos.toString(), "a\n");
  }


  /* This test covers Edges [a, c], [c, f], [f, g], [g, h], [h, i], [i, k] */
  @Test
  public void testEdges3() {

    M obj = new M();
    obj.m("aa", 1);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* This test covers Edges [a, c], [c, g], [g, h], [h, i], [i, k] */
  @Test
  public void testEdges4() {

    M obj = new M();
    obj.m("aaa", 1);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* The following tests cover Prime Path Coverage */

  /* This test covers Prime Path [a, c, d, h, j, k] */  
  @Test
  public void testPrimePath1() {

    M obj = new M();
    obj.m("", 1);

    Assert.assertEquals(baos.toString(), "zero\n");
  }


  /* This test covers Prime Path [a, c, e, h, i, k] */  
  @Test
  public void testPrimePath2() {

    M obj = new M();
    obj.m("a", 1);

    Assert.assertEquals(baos.toString(), "a\n");
  }

  /* This test covers Prime Path [a, c, f, g, h, i, k] */  
  @Test
  public void testPrimePath3() {

    M obj = new M();
    obj.m("aa", 1);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* This test covers Prime Path [a, c, g, h, i, k] */  
  @Test
  public void testPrimePath4() {

    M obj = new M();
    obj.m("aaa", 1);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* This test covers Prime Path [a, b, c, d, h, j, k] */  
  @Test
  public void testPrimePath5() {

    M obj = new M();
    obj.m("", 0);

    Assert.assertEquals(baos.toString(), "zero\n");
  }


  /* This test covers Prime Path [a, b, c, e, h, i, k] */  
  @Test
  public void testPrimePath6() {

    M obj = new M();
    obj.m("a", 0);

    Assert.assertEquals(baos.toString(), "a\n");
  }

  /* This test covers Prime Path [a, b, c, f, g, h, i, k] */  
  @Test
  public void testPrimePath7() {

    M obj = new M();
    obj.m("aa", 0);

    Assert.assertEquals(baos.toString(), "b\n");
  }

  /* This test covers Prime Path [a, b, c, g, h, i, k] */  
  @Test
  public void testPrimePath8() {

    M obj = new M();
    obj.m("aaa", 0);

    Assert.assertEquals(baos.toString(), "b\n");
  }

}
