# To support both python 2 and python 3
from __future__ import division, print_function, unicode_literals
import numpy as np 

def normalize_and_add_one(X):
    X = np.array(X)
    #tim X_max trong tung hang mot
    #bang cach duyet cac phan tu cua 
    #cot trong tung hang.
    X_max = np.array([[np.amax(X[:, column_id])
                        for column_id in range(X.shape[1])]
                    for _ in range(X.shape[0])
                    ])

    #Tuong tu nhu tim X_max, X_min cung nhu vay
    X_min = np.array([[np.amin(X[:, column_id])
                        for column_id in range(X.shape[1])]
                    for _ in range(X.shape[0])
                    ])

    #goi ham np.amin la tim so be nhat trong danh sach mang

    X_normalized = (X - X_min)/(X_max - X_min)

    #tao ra ma tran gom toan cac so 1,
    #so luong hang bang dung so luong hang cua ma tran X
    ones = np.array([[1] for _ in range (X.shape[0]) ])

    #column_stack se ghep hai ma tran vao theo kieu
    #cot cuoi cung cua ma tran nay se dung truoc
    #cot dau tien cua ma tran kia
    return np.column_stack((ones, X_normalized))
    
