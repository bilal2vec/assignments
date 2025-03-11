import numpy as np
from collections import defaultdict


def load_data(x_path, y_path):
    x = defaultdict(list)
    y = []

    with open(x_path, 'r') as f:
        for line in f:
            doc_id, word_id = map(int, line.strip().split())
            x[doc_id].append(word_id)

    with open(y_path, 'r') as f:
        for line in f:
            y.append(int(line.strip()))

    return x, y


def train(x, y, n_words):
    n = len(y)

    counts = {1: 0, 2: 0}
    for label in y:
        counts[label] += 1

    prior_probs = {c: count / n for c, count in counts.items()}

    word_counts = {1: np.zeros(n_words + 1), 2: np.zeros(n_words + 1)}

    for doc_id, word_ids in x.items():
        label = y[doc_id - 1]
        for word_id in word_ids:
            word_counts[label][word_id] += 1

    cond_probs = {}
    cond_probs[1] = (word_counts[1] + 1) / (counts[1] + 2)
    cond_probs[2] = (word_counts[2] + 1) / (counts[2] + 2)

    return prior_probs, cond_probs


def pred(x, prior, cond_probs, n_words):
    preds = []

    for doc_id, word_ids in x.items():
        log_posteriors = {}
        for c in [1, 2]:
            log_posterior = np.log(prior[c])

            for word_id in word_ids:
                if word_id <= n_words:
                    log_posterior += np.log(cond_probs[c][word_id])

            for word_id in range(1, n_words + 1):
                if word_id not in word_ids:
                    log_posterior += np.log(1 - cond_probs[c][word_id])

            log_posteriors[c] = log_posterior

        y_hat = max(log_posteriors, key=log_posteriors.get)
        preds.append(y_hat)

    return preds


def accuracy(y_hats, ys):
    n_correct = 0

    for y_hat, y in zip(y_hats, ys):
        if y_hat == y:
            n_correct += 1

    return (n_correct / len(ys)) * 100


if __name__ == "__main__":
    words = []
    n_words = 0
    with open('words.txt', 'r') as f:
        for line in f:
            words.append(line.strip())
            n_words += 1

    x_train, y_train = load_data('trainData.txt', 'trainLabel.txt')

    prior_probs, cond_probs = train(x_train, y_train, n_words)
    y_hats = pred(x_train, prior_probs, cond_probs, n_words)

    train_accuracy = accuracy(y_hats, y_train)
    print(f"trian accuracy {train_accuracy}")

    x_test, y_test = load_data('testData.txt', 'testLabel.txt')

    y_hats = pred(x_test, prior_probs, cond_probs, n_words)

    test_accuracy = accuracy(y_hats, y_test)
    print(f"test accuracy {test_accuracy}")

    log_diffs = []
    for word_id in range(1, n_words + 1):
        log_prob_1 = np.log(cond_probs[1][word_id])
        log_prob_2 = np.log(cond_probs[2][word_id])
        diff = abs(log_prob_1 - log_prob_2)
        log_diffs.append((word_id, diff))

    log_diffs.sort(key=lambda x: x[1], reverse=True)
    top_n_diffs = log_diffs[:10]

    for word_id, diff in top_n_diffs:
        print(f"{word_id} {words[word_id-1]} {diff}")
