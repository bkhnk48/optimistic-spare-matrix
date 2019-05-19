from __future__ import division, print_function, unicode_literals
import numpy as np 
from normalized_data import normalize_and_add_one
from RidgeRegression import RidgeRegression
from pathlib import Path
import os



def get_data(relative_path):
    
    mypath = Path().absolute()
    path = str(mypath) + relative_path
    print(path)
    
    file = open(path, "r")
    profile_list = file.readlines()
    print(len(profile_list))
    print(profile_list[72])
    file.close()
    return 'X', 'Y'
if __name__ == '__main__':
    print("Main model")
    #X, Y = get_data(path = '/datasets/death_rates_data.txt')
    X, Y = get_data(relative_path = '/datasets/death_rates_data.txt')