import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

file_path = './Iris.csv'
df = pd.read_csv(file_path)

#提取特征（假设前四列为特征）
X = df.iloc[:, :4].values  #取前四列，转换为 numpy 数组

#手动标准化数据（Z-score）
def standardize(data):
    mean = np.mean(data, axis=0)
    std = np.std(data, axis=0)
    return (data - mean) / std

X_scaled = standardize(X)


class KMeans:
    def __init__(self, n_clusters=3, max_iter=100, random_state=None):
        self.n_clusters = n_clusters
        self.max_iter = max_iter
        self.random_state = random_state
        self.centroids = None

    def _initialize_centroids(self, X):
        np.random.seed(self.random_state)
        indices = np.random.choice(X.shape[0], self.n_clusters, replace=False)
        return X[indices]

    def _compute_distances(self, X, centroids):
        #计算欧氏距离的平方（优化性能）
        distances = np.sum((X[:, np.newaxis] - centroids) ** 2, axis=2)
        return np.sqrt(distances)

    def fit(self, X):
        self.centroids = self._initialize_centroids(X)
        for _ in range(self.max_iter):
            #分配样本到最近的簇
            distances = self._compute_distances(X, self.centroids)
            labels = np.argmin(distances, axis=1)

            #更新中心点并处理空簇
            new_centroids = np.zeros_like(self.centroids)
            for i in range(self.n_clusters):
                cluster_points = X[labels == i]
                if len(cluster_points) == 0:  #处理空簇：随机选择一个样本
                    new_centroids[i] = X[np.random.choice(X.shape[0])]
                else:
                    new_centroids[i] = cluster_points.mean(axis=0)

            #检查是否收敛
            if np.allclose(self.centroids, new_centroids):
                break
            self.centroids = new_centroids

        self.labels_ = labels
        return self

    def predict(self, X):
        distances = self._compute_distances(X, self.centroids)
        return np.argmin(distances, axis=1)

#训练模型
kmeans = KMeans(n_clusters=3, max_iter=100, random_state=42)
kmeans.fit(X_scaled)
labels = kmeans.labels_


#输出聚类中心
print("\n聚类中心坐标:")
for i, centroid in enumerate(kmeans.centroids):
    print(f"簇{i}: {np.round(centroid, 4)}")

#使用前两个特征（花萼长度和宽度）绘制聚类结果
plt.figure(figsize=(10, 6))
plt.scatter(X_scaled[:, 0], X_scaled[:, 1], c=labels, cmap='viridis', s=50, alpha=0.8)
plt.scatter(kmeans.centroids[:, 0], kmeans.centroids[:, 1], c='red', s=200, marker='X', label='centre')
plt.xlabel('length')
plt.ylabel('width')
plt.title('K-means Iris')
plt.legend()
plt.show()