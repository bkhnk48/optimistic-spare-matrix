from __future__ import division, print_function, unicode_literals
import numpy as np 
from normalized_data import normalize_and_add_one
from RidgeRegression import RidgeRegression
from pathlib import Path
import os



def get_data(relative_path):
    
    mypath = Path().absolute()
    path = str(mypath) + relative_path
    
    file = open(path, "r")
    list = file.readlines()

    list = list[72:]
    #X = [[] for i in range(len(list))]
    X = [] #np.arange(len(list)*15)
    #Y = [[] for i in range(len(list))]
    Y = []
  
    num_of_rows = len(list)
    for i in range(num_of_rows):
        temp = list[i].split()
        
        temp1 = temp[1: len(temp) - 1]
        
        temp2 = temp[len(temp) - 1]
        #X[i] = X.append(np.array(temp1))
        #X[i].append(temp1)
        #X.extend(temp1)
        tempF1 = np.array(temp1)
        X.extend(tempF1.astype(np.float))
        #Y = Y.append(temp2)
        #Y[i].append(temp2)
        #tempF2 = np.array(temp2)
        Y.append(float(temp2))
    file.close()

    X = np.reshape(X, (num_of_rows, 15))
    Y = np.reshape(Y, (num_of_rows, 1))
    return X, Y
    
if __name__ == '__main__':
    print("Main model")
    
    X, Y = get_data(relative_path = '/datasets/death_rates_data.txt')
    X = normalize_and_add_one(X)
    #print(Y)