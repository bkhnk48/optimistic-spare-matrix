import numpy as np 

class RidgeRegression:
    def __init__(self):
        return 

    def fit(self, X_train, Y_train, LAMBDA):
        assert len(X_train.shap) == 2 and \
                X_train.shape[0] == Y_train.shape[0]
        W = np.linalg.inv(
            X_train.transpose().dot(X_train) + 
            LAMBDA * np.identity(X_train.shape[1])
        ).dot(X_train.transpose()).dot(Y_train)
        return W

    def predict(self, W, X_new):    
        X_new = np.array(X_new)
        Y_new = X_new.dot(W)
        return Y_new

    def compute_RSS(self, Y_new, Y_predicted):
        loss = (1. / Y_new.shape[0]) * np.sum((Y_new - Y_predicted) ** 2)
        return loss

    def get_the_best_LAMBDA(self, X_train, Y_train):
        def cross_validation(num_folds, LAMBDA):
            row_ids = np.array(range(X_train.shape[0]))
            #np.split( ) requires equal divisions
            valid_ids = np.split(row_ids[:len(row_ids) - len(row_ids) % num_folds], 
                                    num_folds)
            valid_ids[-1] = np.append(valid_ids[-1], 
                            row_ids[len(row_ids) - len(row_ids)%num_folds:])

            train_ids = [[k for k in row_ids if k not in valid_ids[i] ]
                                for i in range(num_folds)]
            
            aver_RSS = 0
            for i in range(num_folds):
                valid_part = {'X': X_train[valid_ids[i]], 'Y': Y_train[valid_ids[i]]}
                train_part = {'X': X_train[train_ids[i]], 'Y': Y_train[train_ids[i]]}
                W = self.fit(train_part['X'], train_part['Y'], LAMBDA)
                Y_predicted = self.predict(W, valid_part['X'])
                aver_RSS += self.compute_RSS(valid_part['Y'], Y_predicted)
            return aver_RSS/num_folds

        def range_scan(best_LAMBDA, minimum_RSS, LAMBDA_values):
            for current_LAMBDA in LAMBDA_values:
                aver_RSS = cross_validation(num_folds = 5, LAMBDA = current_LAMBDA)
                if aver_RSS < minimum_RSS:
                    best_LAMBDA = current_LAMBDA
                    minimum_RSS = aver_RSS
            return best_LAMBDA, minimum_RSS


        best_LAMBDA, minimum_RSS = range_scan(best_LAMBDA = 0,
                minimum_RSS = 10000 ** 2,
                LAMBDA_values = range(50)
                    )    
        
        LAMBDA_values = [k * 1. / 1000 for k in range(
            max(0, (best_LAMBDA - 1) * 1000), (best_LAMBDA + 1)*1000, 1)
                        ] #step size = 0.001

        best_LAMBDA, minimum_RSS = range_scan(best_LAMBDA = best_LAMBDA,
                minimum_RSS = minimum_RSS,
                LAMBDA_values = LAMBDA_values
                    )  

        return best_LAMBDA
                                
