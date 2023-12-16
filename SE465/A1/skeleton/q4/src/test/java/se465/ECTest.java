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

public class ECTest {
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

  /* test edge where second node is in graph */
  @Test
  public void addEdge() {
    cfg.addNode(1001, m_m, m);
    cfg.addEdge(1000, m_m, m, 1001, m_m, m);

    CFG.Node nn = new CFG.Node(1001, m_m, m);
    assertTrue(cfg.nodes.contains(nn));
  }

    // test case edges where more than one node links to the node being removed.
    @Test
    public void deleteNode() {
        CFG.Node n1 = new CFG.Node(1000, m_m, m);
        CFG.Node n2 = new CFG.Node(1001, m_m, m);
        CFG.Node n3 = new CFG.Node(1002, m_m, m);

        cfg.addEdge(n1.position, n1.method, n1.clazz, n3.position, n3.method, n3.clazz);
        cfg.addEdge(n2.position, n2.method, n2.clazz, n3.position, n3.method, n3.clazz);

        // proceed if the node is added successfully

        Assume.assumeTrue(cfg.edges.get(n1).contains(n3));
        Assume.assumeTrue(cfg.edges.get(n2).contains(n3));

        int sizeExpected = cfg.nodes.size();

        cfg.deleteNode(n3.position, n3.method, n3.clazz);

        assertFalse(cfg.nodes.contains(n3));
        assertEquals(sizeExpected - 1, cfg.nodes.size());

        assertFalse(cfg.edges.get(n1).contains(n3));
        assertFalse(cfg.edges.get(n2).contains(n3));
    }

}

