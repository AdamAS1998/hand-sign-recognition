import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import accuracy_score
import joblib

# load dataset
data = pd.read_csv("dataset.csv", header=None)

# label first column
y = data.iloc[:,0]

# features remaining columns
X = data.iloc[:,1:]

# split dataset
X_train, X_val, y_train, y_val = train_test_split(
    X, y, test_size=0.2, random_state=42
)

best_k = 1
best_acc = 0

# test different K values
for k in range(1, 16):

    knn = KNeighborsClassifier(n_neighbors=k)

    knn.fit(X_train, y_train)

    pred = knn.predict(X_val)

    acc = accuracy_score(y_val, pred)

    print("K =", k, "Accuracy =", acc)

    if acc > best_acc:
        best_acc = acc
        best_k = k

print("\nBest K =", best_k)

# train final model
knn = KNeighborsClassifier(n_neighbors=best_k)
knn.fit(X, y)

# save model
joblib.dump(knn, "knn_model.pkl")

print("Model saved!")

input("Press Enter to exit")