# To support both python 2 and python 3
from __future__ import division, print_function, unicode_literals
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
print('one = ', one) # tao ma tran 13x1, voi cac phan tu la 1
Xbar = np.concatenate((one, X), axis = 1)
print('Xbar = ', Xbar) #ghep hai ma tran one, X lam mot
#tao thanh ma tran Xbar voi kich thuoc 13x2
#cot 1 la cac so 1, cot 2 la cac phan tu cua X

# Calculating weights of the fitting line 
A = np.dot(Xbar.T, Xbar) #tinh A = X(T)*X
b = np.dot(Xbar.T, y)    #tinh b = X(T)*y
w = np.dot(np.linalg.pinv(A), b)#tinh w = A(-1)*b
print('w = ', w)
# Preparing the fitting line 
w_0 = w[0][0]
w_1 = w[1][0]
x0 = np.linspace(145, 185, 2)#thay so 2 bang 50 cung ko van de gi
y0 = w_0 + w_1*x0

# Drawing the fitting line 
plt.plot(X.T, y.T, 'ro')     # data 
plt.plot(x0, y0)               # the fitting line
plt.axis([140, 190, 45, 75])
plt.xlabel('Height (cm)')
plt.ylabel('Weight (kg)')
plt.show()

y1 = w_1*155 + w_0
y2 = w_1*160 + w_0

print( u'Predict weight of person with height 155 cm: %.2f (kg), real number: 52 (kg)'  %(y1) )
print( u'Predict weight of person with height 160 cm: %.2f (kg), real number: 56 (kg)'  %(y2) )
