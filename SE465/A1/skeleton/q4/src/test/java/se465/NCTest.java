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

public class NCTest {
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

}

