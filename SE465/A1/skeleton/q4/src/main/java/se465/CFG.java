package se465;

import org.objectweb.asm.tree.ClassNode;
import org.objectweb.asm.tree.MethodNode;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;


public class CFG {
  Set<Node> nodes = new HashSet<Node>();
  Map<Node, Set<Node>> edges = new HashMap<Node, Set<Node>>();

  static class Node {
    int position;
    MethodNode method;
    ClassNode clazz;

    Node(int p, MethodNode m, ClassNode c) {
      position = p;
      method = m;
      clazz = c;
    }

    public boolean equals(Object o) {
      if (!(o instanceof Node)) return false;
      Node n = (Node) o;
      return (position == n.position) &&
          method.equals(n.method) && clazz.equals(n.clazz);
    }

    public int hashCode() {
      return position + method.hashCode() + clazz.hashCode();
    }

    public String toString() {
      return clazz.name + "." +
          method.name + method.signature + ": " + position;
    }
  }

  public void addNode(int p, MethodNode m, ClassNode c) {
    Node n = new Node(p, m, c);

    if (nodes.contains(n)) {
      return;
    }

    nodes.add(n);

    Node n2 = new Node(p, m, c);
    edges.put(n2, new HashSet<Node>());
  }

  public void addEdge(int p1, MethodNode m1, ClassNode c1,
                      int p2, MethodNode m2, ClassNode c2) {
    Node n1 = new Node(p1, m1, c1);
    Node n2 = new Node(p2, m2, c2);

    if (!nodes.contains(n1)) {
      addNode(p1, m1, c1);
    }

    if (!nodes.contains(n2)) {
      addNode(p2, m2, c2);
    }

    edges.get(n1).add(n2);
  }

  public void deleteNode(int p, MethodNode m, ClassNode c) {
    Node t = new Node(p, m, c);
    
    if (!nodes.contains(t)) {
      return;
    }

    for (Node n : nodes) {
      Node t2 = new Node(p, m, c);
      edges.get(n).remove(t2);
    }

    nodes.remove(t);
    edges.remove(t);
  }

  public void deleteEdge(int p1, MethodNode m1, ClassNode c1,
                         int p2, MethodNode m2, ClassNode c2) {

    Node n1 = new Node(p1, m1, c1);
    Node n2 = new Node(p2, m2, c2);

    if (nodes.contains(n1)) {
      edges.get(n1).remove(n2);
    }

  }


  public boolean isReachable(int p1, MethodNode m1, ClassNode c1,
                             int p2, MethodNode m2, ClassNode c2) {

    Node n1 = new Node(p1, m1, c1);
    Node n2 = new Node(p2, m2, c2);

    if (!nodes.contains(n1) || !nodes.contains(n2)) {
      return false;
    }

    if (edges.get(n1).contains(n2)) {
      return true;
    }

    boolean is_reachable = false;
    for (Node n: edges.get(n1)) {
      is_reachable = isReachable(n.position, n.method, n.clazz, p2, m2, c2);

      if (is_reachable) {
        return true;
      }

    }

    return false;
  }
}
