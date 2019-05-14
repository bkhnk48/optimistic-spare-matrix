# To support both python 2 and python 3
from __future__ import division, print_function, unicode_literals
from sklearn import datasets, linear_model

import numpy as np 
import matplotlib.pyplot as plt

# height (cm)
X = np.array([[147, 150, 153, 158, 163, 165, 168, 170, 173, 175, 178, 180, 183]]).T
# weight (kg)
y = np.array([[ 49, 50, 51,  54, 58, 59, 60, 62, 63, 64, 66, 67, 68]]).T
# Visualize data 
#plt.plot(X, y, 'ro')
#plt.axis([140, 190, 45, 75])
#plt.xlabel('Height (cm)')
#plt.ylabel('Weight (kg)')
#plt.show()


# Building Xbar 
#print('X.shape[0]', X.shape[0])
one = np.ones((X.shape[0], 1)) #X.shape[0] = 13
#print('one = ', one) # tao ma tran 13x1, voi cac phan tu la 1
Xbar = np.concatenate((one, X), axis = 1)
#ghep hai ma tran one, X lam mot
#tao thanh ma tran Xbar voi kich thuoc 13x2
#cot 1 la cac so 1, cot 2 la cac phan tu cua X

# Calculating weights of the fitting line 
A = np.dot(Xbar.T, Xbar)
b = np.dot(Xbar.T, y)
w = np.dot(np.linalg.pinv(A), b)

# fit the model by Linear Regression
regr = linear_model.LinearRegression(fit_intercept=False) # fit_intercept = False for calculating the bias
regr.fit(Xbar, y)

# Compare two results
print( 'Solution found by scikit-learn  : ', regr.coef_ )
print( 'Solution found by (5): ', w.T)
