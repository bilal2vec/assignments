import math
import heapq
import matplotlib.pyplot as plt


def entropy(doc_ids, labels):
    n = len(doc_ids)
    if n == 0:
        return 1
    n_1 = sum(1 for i in doc_ids if labels[i] == 1)
    n_2 = sum(1 for i in doc_ids if labels[i] == 2)
    p1 = n_1 / n
    p2 = n_2 / n
    entropy = 0
    if p1 > 0:
        entropy += -p1 * math.log(p1, 2)
    if p2 > 0:
        entropy += -p2 * math.log(p2, 2)
    return entropy


def maj(doc_ids, labels):
    n_1 = sum(1 for i in doc_ids if labels[i] == 1)
    n_2 = sum(1 for i in doc_ids if labels[i] == 2)
    return 1 if n_1 >= n_2 else 2


class Node:

    def __init__(self, doc_ids, train_labels):
        self.doc_ids = doc_ids
        self.pred = maj(doc_ids, train_labels) if doc_ids else None
        self.is_leaf = True
        self.left = None
        self.right = None
        self.feature = None
        self.i_g = None
        self.split_order = None


def split(node, x, y, method):
    max_i_g = -float('inf')
    best_feature = None
    best_E_1 = None
    best_E_2 = None
    if not node.doc_ids:
        return None
    features = set()
    for doc in node.doc_ids:
        features |= x[doc]
    if not features:
        return None

    I_E = entropy(node.doc_ids, y)
    for feature in features:
        E_1 = set(doc for doc in node.doc_ids if feature in x[doc])
        if len(E_1) == 0 or len(E_1) == len(node.doc_ids):
            continue
        E_2 = node.doc_ids - E_1
        IE_1 = entropy(E_1, y)
        IE_2 = entropy(E_2, y)
        if method == 1:
            i_g = I_E - 0.5 * (IE_1 + IE_2)
        else:
            N_1 = len(E_1)
            N_2 = len(E_2)
            N = len(node.doc_ids)
            i_g = I_E - ((N_1 / N) * IE_1 + (N_2 / N) * IE_2)
        if i_g > max_i_g:
            max_i_g = i_g
            best_feature = feature
            best_E_1 = E_1
            best_E_2 = E_2
    if best_feature is None:
        return None
    return best_feature, max_i_g, best_E_1, best_E_2


def build_tree(x_train,
               y_train,
               x_test,
               y_test,
               method,
               max_n_internal_nodes=100):
    root = Node(set(range(len(x_train))), y_train)
    pq = []

    i = 0
    candidate = split(root, x_train, y_train, method)
    if candidate is not None:
        best_feature, max_i_g, E_1, E_2 = candidate
        heapq.heappush(pq, (-max_i_g, i, root, best_feature, E_1, E_2))
        i += 1

    train_accs = []
    test_accs = []
    n_internal_nodes = 0
    while pq and n_internal_nodes < max_n_internal_nodes:
        i_g, _, node, best_feature, E_1, E_2 = heapq.heappop(pq)
        i_g = -i_g

        node.is_leaf = False
        node.left = Node(E_1, y_train)
        node.right = Node(E_2, y_train)
        node.feature = best_feature
        node.i_g = i_g
        node.split_order = n_internal_nodes + 1

        n_internal_nodes += 1
        for child in [node.left, node.right]:
            candidate_child = split(child, x_train, y_train, method)
            if candidate_child is not None:
                feature, i_g, E_1, E_2 = candidate_child
                heapq.heappush(pq, (-i_g, i, child, feature, E_1, E_2))
                i += 1

        local_train_acc = score(root, x_train, y_train)
        local_test_acc = score(root, x_test, y_test)
        train_accs.append(local_train_acc)
        test_accs.append(local_test_acc)
    return root, train_accs, test_accs


def predict(root, x):
    node = root
    while not node.is_leaf:
        if node.feature in x:
            node = node.left
        else:
            node = node.right
    return node.pred


def score(root, x, y):
    correct = 0
    for i, doc in enumerate(x):
        if predict(root, doc) == y[i]:
            correct += 1
    return (correct / len(x)) * 100


def load_data():
    y_train = []
    with open("trainLabel.txt", "r") as f:
        for line in f:
            line = line.strip()
            if line:
                y_train.append(int(line))

    x_train = [set() for _ in range(len(y_train))]
    with open("trainData.txt", "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 2:
                continue
            doc_id, word_id = int(parts[0]) - 1, int(parts[1])
            x_train[doc_id].add(word_id)

    y_test = []
    with open("testLabel.txt", "r") as f:
        for line in f:
            line = line.strip()
            if line:
                y_test.append(int(line))
    x_test = [set() for _ in range(len(y_test))]

    with open("testData.txt", "r") as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) != 2:
                continue
            doc_id, word_id = int(parts[0]) - 1, int(parts[1])
            x_test[doc_id].add(word_id)

    words = []
    with open("words.txt", "r") as f:
        for line in f:
            words.append(line.strip())

    return x_train, y_train, x_test, y_test, words


def print_tree(node, words, depth=0):
    indent = "  " * depth
    if node.is_leaf:
        print(f"{indent}[leaf] pred = {node.pred} n_doc: {len(node.doc_ids)}")
    else:
        feature = words[node.feature -
                        1] if (node.feature -
                               1) < len(words) else str(node.feature)
        print(
            f"{indent}node split_order: {node.split_order}: split on '{feature}' feature: {node.feature} i_g = {node.i_g} n_doc: {len(node.doc_ids)}"
        )
        print_tree(node.left, words, depth + 1)
        print_tree(node.right, words, depth + 1)


def plot_accuracies(train_accs, test_accs, method_name):
    plt.figure()
    num_nodes = range(1, len(train_accs) + 1)
    plt.plot(num_nodes, train_accs, label="Train Accurayc")
    plt.plot(num_nodes, test_accs, label="Test Accurayc")
    plt.xlabel("n_internal_nodes")
    plt.ylabel("Accuracy")
    plt.title(f"Accuracy vs n_internal_nodes for {method_name}")
    plt.legend()
    plt.savefig(f"{method_name}.png")


def main():
    x_train, y_train, x_test, y_test, words = load_data()

    tree_1, train_accuray_1, test_accuray_1 = build_tree(x_train,
                                                         y_train,
                                                         x_test,
                                                         y_test,
                                                         method=1)
    plot_accuracies(train_accuray_1, test_accuray_1, "Method 1")

    tree_2, train_accuray_2, test_accuray_2 = build_tree(x_train,
                                                         y_train,
                                                         x_test,
                                                         y_test,
                                                         method=2)
    plot_accuracies(train_accuray_2, test_accuray_2, "Method 2")

    tree, _, _ = build_tree(x_train,
                            y_train,
                            x_test,
                            y_test,
                            method=1,
                            max_n_internal_nodes=10)
    print_tree(tree, words)

    tree_2, _, _ = build_tree(x_train,
                              y_train,
                              x_test,
                              y_test,
                              method=2,
                              max_n_internal_nodes=10)
    print_tree(tree_2, words)


if __name__ == "__main__":
    main()
