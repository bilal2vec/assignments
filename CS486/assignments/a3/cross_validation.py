import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.model_selection import KFold
from neural_net import NeuralNetwork
from operations import Sigmoid, ReLU, Identity, MeanSquaredError, mean_absolute_error


def load_dataset(csv_path, target_feature):
    dataset = pd.read_csv(csv_path)
    t = np.expand_dims(dataset[target_feature].to_numpy().astype(float), axis=1)
    X = dataset.drop([target_feature], axis=1).to_numpy()
    return X, t


if __name__ == "__main__":
    X, y = load_dataset("data/wine_quality.csv", "quality")
    n_features = X.shape[1]

    folds = KFold(n_splits=5, shuffle=True, random_state=42)

    all_losses = []
    all_maes = []

    fold_idx = 1
    for train_index, val_index in folds.split(X):
        X_train, X_val = X[train_index], X[val_index]
        y_train, y_val = y[train_index], y[val_index]

        net = NeuralNetwork(
            n_features, [32, 32, 16, 1],
            [ReLU(), ReLU(), Sigmoid(), Identity()],
            MeanSquaredError(),
            learning_rate=0.001)

        _, epoch_losses = net.train(X_train, y_train, 500)

        val_mae = net.evaluate(X_val, y_val, mean_absolute_error)

        all_losses.append(epoch_losses)
        all_maes.append(val_mae)
        fold_idx += 1

    avg_losses = np.mean(all_losses, axis=0)
    avg_mae = np.mean(all_maes)
    std_mae = np.std(all_maes)

    print(np.mean(all_losses, axis=1))
    print(avg_mae)
    print(std_mae)

    plt.figure()
    plt.plot(range(500), avg_losses)
    plt.title(f'Avg training loss across folds')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.savefig("cross_validation_results.png")
    plt.show()
