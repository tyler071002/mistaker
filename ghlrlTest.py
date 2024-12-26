import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
import matplotlib.pyplot as plt

df = pd.read_excel('수오도.xlsx')
print("Columns:", df.columns)
X = df[['time','takdo','tds','temperature']]
y = df['ph + 7.7']

X_train, X_test, y_train, y_test = train_test_split( X, y, test_size=0.2, random_state=42)

model = LinearRegression()
model.fit(X_train, y_train)

y_pred = model.predict(X_test)

mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)
print("Mean Squared Error (MSE):", mse)
print("R-squared (R2):", r2)

df['predicted'] = model.predict(X)
plt.figure(figsize=(8, 5))
plt.plot(df['time'], df['ph + 7.7'], label='Actual (ph + 7.7)', marker='o')
plt.plot(df['time'], df['predicted'], label='Predicted (ph + 7.7)', marker='x')
plt.xlabel('time')
plt.ylabel('ph + 7.7')
plt.title('Actual vs Predicted Trend')
plt.legend()
plt.show()
