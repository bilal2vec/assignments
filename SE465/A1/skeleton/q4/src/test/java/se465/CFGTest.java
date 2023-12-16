package se465;

import org.junit.Assume;
import org.junit.Before;
import org.junit.Test;
import org.objectweb.asm.ClassReader;
import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;
import org.objectweb.asm.tree.analysis.Analyzer;
import org.objectweb.asm.tree.analysis.AnalyzerException;
import org.objectweb.asm.tree.analysis.BasicInterpreter;

import java.util.*;

import static org.junit.Assert.*;

public class CFGTest {
  private CFG cfg;
  private ClassNode m;
  private MethodNode m_m;

  @Before
  public void createCFG() {
    cfg = new CFG();

    ClassReader cr;
    try {
      cr = new ClassReader("se465.M");
    } catch (Throwable e) {
      throw new RuntimeException("Can't open class se465.M", e);
    }
    final ClassNode cn = new ClassNode();
    m = cn;
    cr.accept(cn, ClassReader.SKIP_DEBUG);

    for (final MethodNode m : (List<MethodNode>) cn.methods) {
      if (m.name.equals("m")) m_m = m;

      for (int i = 0; i < m.instructions.size(); i++)
        cfg.addNode(i, m, cn);

      Analyzer a = new Analyzer(new BasicInterpreter()) {
        protected void newControlFlowEdge(int insn, int succ) {
          cfg.addEdge(insn, m, cn,
              succ, m, cn);
        }
      };
      try {
        a.analyze(cn.name, m);
      } catch (AnalyzerException e) {
      }
    }
  }

  @Test
  public void addNode() {
    cfg.addNode(1000, m_m, m);
    CFG.Node n = new CFG.Node(1000, m_m, m);
    assertTrue(cfg.nodes.contains(n));
  }

  @Test
  public void addEdge() {
    cfg.addEdge(1000, m_m, m,
        1001, m_m, m);
    CFG.Node n = new CFG.Node(1000, m_m, m);
    CFG.Node nn = new CFG.Node(1001, m_m, m);

    assertTrue(cfg.edges.get(n).contains(nn));
  }

  @Test
  public void addEdge_oneNewNode() {
    cfg.addNode(1000, m_m, m);
    cfg.addEdge(1000, m_m, m, 1001, m_m, m);

    CFG.Node nn = new CFG.Node(1001, m_m, m);
    assertTrue(cfg.nodes.contains(nn));
  }

  @Test
  public void addNode_duplicate() {
    cfg.addEdge(1000, m_m, m,
        1001, m_m, m);
    cfg.addNode(1000, m_m, m);
    CFG.Node n = new CFG.Node(1000, m_m, m);
    assertTrue(cfg.edges.get(n).size() == 1);
  }

  @Test
  public void deleteNode() {
    CFG.Node n = new CFG.Node(1000, m_m, m);
    CFG.Node nn = new CFG.Node(1001, m_m, m);
    cfg.addEdge(n.position, n.method, n.clazz, nn.position, nn.method, nn.clazz);
    // proceed if the node is added successfully
    Assume.assumeTrue(cfg.edges.get(n).contains(nn));
    int sizeExpected = cfg.nodes.size();

    cfg.deleteNode(nn.position, nn.method, nn.clazz);

    assertFalse(cfg.nodes.contains(nn));
    assertEquals(sizeExpected - 1, cfg.nodes.size());
    assertFalse(cfg.edges.get(n).contains(nn));
  }

  @Test
  public void deleteNode_missing() {
    CFG.Node n = new CFG.Node(1000, m_m, m);

    Set<CFG.Node> expected = new HashSet<CFG.Node>();

    for (CFG.Node m : cfg.nodes) {
      CFG.Node mCopy = new CFG.Node(m.position, m.method, m.clazz);
      expected.add(mCopy);
    }

    cfg.deleteNode(n.position, n.method, n.clazz);

    // there must be no change in nodes.
    assertEquals(expected, cfg.nodes);
  }

  @Test
  public void deleteEdge() {
    CFG.Node n = new CFG.Node(1000, m_m, m);
    CFG.Node nn = new CFG.Node(1001, m_m, m);

    // add an edge
    cfg.addEdge(n.position, n.method, n.clazz, nn.position, nn.method, nn.clazz);

    int sizeExpected = cfg.edges.get(n).size();

    // proceed if the edge is added successfully
    Assume.assumeTrue(cfg.edges.get(n).contains(nn));

    // remove the added edge
    cfg.deleteEdge(n.position, n.method, n.clazz, nn.position, nn.method, nn.clazz);
    assertFalse(cfg.edges.get(n).contains(nn));
    assertEquals(sizeExpected - 1, cfg.edges.get(n).size());
  }

  @Test
  public void deleteEdge_missing() {
    CFG.Node n = new CFG.Node(1000, m_m, m);
    CFG.Node nn = new CFG.Node(1001, m_m, m);

    Map<CFG.Node, Set<CFG.Node>> expected = new HashMap<CFG.Node, Set<CFG.Node>>();

    for (CFG.Node m : cfg.edges.keySet()) {
      CFG.Node mCopy = new CFG.Node(m.position, m.method, m.clazz);
      Set<CFG.Node> mNeighbours = cfg.edges.get(m);
      Set<CFG.Node> mNeighboursCopy = new HashSet<CFG.Node>();

      for (CFG.Node mm : mNeighbours) {
        CFG.Node mmCopy = new CFG.Node(mm.position, mm.method, mm.clazz);
        mNeighboursCopy.add(mmCopy);
      }

      expected.put(mCopy, mNeighboursCopy);
    }

    // try to remove a non-existing edge
    cfg.deleteEdge(n.position, n.method, n.clazz, nn.position, nn.method, nn.clazz);
    //there must be no change in edges.
    assertEquals(expected, cfg.edges);
  }

  @Test
  public void deleteEdge_missingSrcNode() {
    CFG.Node n = new CFG.Node(1000, m_m, m);
    CFG.Node nn = new CFG.Node(1001, m_m, m);

    //make sure the source node is missing
    Assume.assumeTrue(!cfg.nodes.contains(n));

    cfg.addNode(nn.position, nn.method, nn.clazz);

    Map<CFG.Node, Set<CFG.Node>> expected = new HashMap<CFG.Node, Set<CFG.Node>>();

    for (CFG.Node m : cfg.edges.keySet()) {
      CFG.Node mCopy = new CFG.Node(m.position, m.method, m.clazz);
      Set<CFG.Node> mNeighbours = cfg.edges.get(m);
      Set<CFG.Node> mNeighboursCopy = new HashSet<CFG.Node>();

      for (CFG.Node mm : mNeighbours) {
        CFG.Node mmCopy = new CFG.Node(mm.position, mm.method, mm.clazz);
        mNeighboursCopy.add(mmCopy);
      }

      expected.put(mCopy, mNeighboursCopy);
    }


    // try to remove a non-existing edge
    cfg.deleteEdge(n.position, n.method, n.clazz, nn.position, nn.method,
        nn.clazz);

    // there must be no change in edges.
    assertEquals(expected, cfg.edges);

  }

  @Test
  public void reachable_true() {
    assertTrue(cfg.isReachable(0, m_m, m,
        3, m_m, m));
  }

  @Test
  public void reachable_unreachable() {
    assertFalse(cfg.isReachable(59, m_m, m,
        0, m_m, m));
  }

  @Test
  public void reachable_missingSrc() {
    assertFalse(cfg.isReachable(-100, m_m, m,
        0, m_m, m));
  }

  @Test
  public void reachable_missingTarget() {
    assertFalse(cfg.isReachable(0, m_m, m, 1000, m_m, m));
  }

}

