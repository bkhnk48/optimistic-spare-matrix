from __future__ import division, print_function, unicode_literals
import numpy as np 
from normalized_data import normalize_and_add_one
from RidgeRegression import RidgeRegression
from pathlib import Path
import os



def get_data(relative_path):
    
    mypath = Path().absolute()
    path = str(mypath) + relative_path
    #print(path)
    #X = np.array([[]]) #np.array([])
    
    Y = []
    file = open(path, "r")
    list = file.readlines()
    #print(len(profile_list))
    #print(profile_list[72])
    #row_ids = np.array(range(len(list)))
    list = list[72:]
    #X = [[] for i in range(len(list))]
    X = np.arange(len(list))
    Y = [[] for i in range(len(list))]
    #print(list[0])
    for i in range(len(list)):
        temp = list[i].split()
        temp1 = temp[1: len(temp) - 2]
        temp2 = temp[len(temp) - 1]
        #X[i] = X.append(np.array(temp1))
        X[i].append(temp1)
        #Y = Y.append(temp2)
        Y[i].append(temp2)
    file.close()
    return X, Y
if __name__ == '__main__':
    print("Main model")
    
    X, Y = get_data(relative_path = '/datasets/death_rates_data.txt')
    #X = normalize_and_add_one(X)
    print(X)