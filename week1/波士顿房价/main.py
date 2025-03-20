import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# 定义列名（根据数据集特征描述）
column_names = [
    'MEDV','CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM',
    'AGE', 'DIS', 'RAD', 'TAX', 'PTRATIO', 'B', 'LSTAT'
]

# 读取数据
df = pd.read_csv(
    'price.txt',
    sep='  ',         #匹配双空格作为分隔符
    header=None,       #无默认列名
    names=column_names #指定自定义列名
)

# === 数据分析与可视化 ===
plt.figure(figsize=(12, 6))


for n in column_names:
    if n == 'MEDV':break
    plt.scatter(df[n], df['MEDV'], alpha=0.6, edgecolor='w')
    plt.title(n)
    plt.ylabel('price')
    plt.tight_layout()
    plt.show()

#读取数据
X = df.drop('MEDV', axis=1).values
y = df['MEDV'].values.reshape(-1,1)


#预处理，标准化，要不梯度会爆炸
X_scaled = (X-np.mean(X, axis=0))/np.std(X, axis=0)
X = np.hstack((X_scaled,np.ones((len(df['MEDV']),1))))



class LinearRegression:
    def __init__(self):
        self.w = None   #训练参数

    def fit_normal(self, X, y):
        # (Xt*X)^-1*Xt*y
        self.w = np.linalg.inv(X.T.dot(X)).dot(X.T).dot(y)

    def fit_gradient(self, X, y,lr=0.01, n_iters=1000):
        m = X.shape[0]
        self.w = np.zeros((X.shape[1], 1))  #参数初始化为0吧

        for _ in range(n_iters):
            gradients = 2 / m * X.T.dot(X.dot(self.w) - y)  #计算梯度
            self.w -= lr * gradients    #更新权重

    def predict(self, X):
        return X.dot(self.w)    #计算预估值





split_ratio = 1.0
split = int(split_ratio * len(df['MEDV']))

#最小二乘法
lr_normal = LinearRegression()
lr_normal.fit_normal(X, y)

#梯度下降法
lr_gradient = LinearRegression()
lr_gradient.fit_gradient(X, y,lr=0.01, n_iters=100000)

#预测
y_pred_normal = lr_normal.predict(X)
y_pred_gradient = lr_gradient.predict(X)







def evaluate(y_true, y_pred):
    mse = np.mean((y_true - y_pred)**2)
    r2 = 1 - np.sum((y_true - y_pred)**2) / np.sum((y_true - np.mean(y_true))**2)
    return mse, r2

mse_normal, r2_normal = evaluate(y, y_pred_normal)
mse_gradient, r2_gradient = evaluate(y, y_pred_gradient)

print(f"最小二乘法结果：方差={mse_normal}, R²={r2_normal}")
print(f"梯度下降法结果：方差={mse_gradient}, R²={r2_gradient}")




plt.figure(figsize=(10,4))

plt.subplot(1,2,1)
plt.scatter(y, y_pred_normal)
plt.plot([0,50], [0,50], 'r--')
plt.title('LS')
plt.xlabel('actual')
plt.ylabel('estimated')

plt.subplot(1,2,2)
plt.scatter(y, y_pred_gradient)
plt.plot([0,50], [0,50], 'r--')
plt.title('GD')
plt.xlabel('actual')
plt.ylabel('estimated')

plt.tight_layout()
plt.show()