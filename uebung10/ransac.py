import numpy as np
import pandas as pd
import random
import math



df=pd.read_csv("Roof.txt", header=None)
df=df.iloc[:,0:3]


def random_pt():
    return df.iloc[random.randint(0, df.shape[0]-1),:]


def RANSAC(k, t, tol):    
    models=[]
    votes=[]
    for i in range(1,k+1):
        vote_cnt=0
        pt1=random_pt()
        pt2=random_pt()
        pt3=random_pt()
        pt12 = pt1-pt2
        pt13 = pt1-pt3
        n = np.cross(pt12, pt13)
        n/= math.sqrt(n[0]**2+n[1]**2+n[2]**2)
        d_Modell = np.dot(pt1, n)
        for j in range(0, df.shape[0]):
            d=df.iloc[j,:]
            d-=d_Modell
            d=np.dot(n, d)
            if d < tol:
                vote_cnt+=1
        if vote_cnt >= t:
            votes.append(vote_cnt)
            models.append(n)
    print(max(votes))
    return models[np.argmax(votes)]
        


k=50
t=10
tol=5

print(RANSAC(k, t, tol))