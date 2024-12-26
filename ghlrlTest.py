import sklearn
import numpy as np
import pandas as pd
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.datasets import  load_diabetes

diabetes = load_diabetes()
diabetes_df = pd.DataFrame(data=diabetes.data, columns=diabetes.feature_names)
diabetes_df['target'] = diabetes.target
diabetes_corr = diabetes_df.corr()

x_data=diabetes_df.loc[:, ['bmi', 's5']]
y_data=diabetes_df.loc[:, 'target']
X_train, X_test, y_train, y_test=train_test_split(x_data, y_data, test_size=0.2, random_state=1)
lr = LinearRegression()
lr.fit(X_train, y_train)
print(np.round(lr.coef_, 2))
print(np.round(lr.intercept_, 2))
pred = lr.predict(X_test)
plt.figure(figsize=(10, 6))
plt.scatter(X_test['bmi'], y_test, label='test')
plt.scatter(X_test['bmi'], pred, c='r', label='predict')
plt.legend()
plt.show()