import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
import matplotlib.pyplot as plt

# CSV 파일 읽어오기
df = pd.read_csv('자료 조회_20241227.csv')

# 시간 데이터를 분 단위로 변환
def convert_to_minutes(time_str):
    try:
        hours, minutes = map(int, time_str.split(':'))
        return hours * 60 + minutes
    except ValueError:
        print(f"Invalid time format: {time_str}")  # 오류 메시지 출력
        return None

df['time_minutes'] = df['time'].apply(convert_to_minutes)

# pH 값의 결측값을 평균값으로 채우기
df['pH'].fillna(df['pH'].mean(), inplace=True)

# 'time_minutes' 열의 결측값 제거
df.dropna(subset=['time_minutes'], inplace=True)

# 선형 회귀 모델 학습
X = df[['time_minutes', 'temp']]
y = df['pH']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
model = LinearRegression()
model.fit(X_train, y_train)

# 예측 및 평가
y_pred = model.predict(X_test)
mse = mean_squared_error(y_test, y_pred)
r2 = r2_score(y_test, y_pred)

# 결과 출력
print('Mean Squared Error (MSE):', mse)
print('R-squared (R2):', r2)

# 예측 결과 시각화
plt.figure(figsize=(10, 6))
plt.scatter(X_test['time_minutes'], y_test, color='blue', label='Actual')
plt.plot(X_test['time_minutes'], y_pred, color='red', linewidth=2, label='Predicted')
plt.xlabel('Time (minutes)')
plt.ylabel('pH')
plt.title('Linear Regression: Time vs pH')
plt.legend()
plt.show()