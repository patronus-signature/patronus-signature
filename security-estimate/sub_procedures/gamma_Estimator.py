import numpy as np
import scipy.fft as dft
from math import sqrt
import random as rd

def bi_cent(n):
    res = 0
    for i in range(int(n/2)):
        res += rd.randint(0,1) - rd.randint(0,1)
    return res

def sample_c(n, tau):
    tmp = [i for i in range(n)]
    ind = rd.sample(tmp, tau)
    c = [0 for i in range(n)] 
    for elt in ind:
        c[elt] = (-1)**rd.randint(0,1)
    return c

    
def anti_circulant_matrix(v):
    n = len(v)
    A = np.zeros((n, n))
    for i in range(n):

        A[:, i] = np.roll(v, i)
    for i in range(n):
        for j in range(n):
            if j>i:
                A[i,j]*= -1
    return A

def norm_sc(n, dim, eta, tau):
    s1 = [[ bi_cent(eta) for _ in range(n)] for _ in range(dim)]
    c = np.array(sample_c(n, tau))
    c_a = []
    for elt in c:
        c_a.append([elt])
    compt_res = []
    res = 0
    for i in range(len(s1)):
        res += np.linalg.norm(np.matmul(anti_circulant_matrix(s1[i]),c_a))
    return res

def gamma_estimate_2(N,n, dim, eta, tau ):
    res = 0
    for i in range(N):
        print(i)
        res += norm_sc(n, dim, eta, tau)
    return res/N
#print(gamma_estimate(100, 256, 4, 4, 39))

    
def gamma_estimate(N, n, dim, eta, tau, rate, d, k):

    res = []
    i_max = n//tau
    leftover = n % tau


    for loop in range(N):
        #Generate a secret
        #s1 = [np.concatenate((np.random.randint(-eta,eta+1,size=n), np.array([0 for i in range(n)]))) for i in range(dim)]
        #s1 = [np.concatenate((np.random.normal(0, eta/sqrt(sqrt(n*k)),size=n), np.array([0 for i in range(n)]))) for i in range(dim)]
        s1 = [[bi_cent(eta) for i in range(n)] for _ in range(dim)]
        #Generate a public key (assumed uniform here)
        #Compute the canonical embeddings and their norm
        y = [dft.fft(s1[i])[1::2] for i in range(len(s1))]
        norm_y = [np.linalg.norm([abs(y[i][j]) for i in range(len(y))]) for j in range(len(y[0]))]
        largest = max(norm_y)
        #We now compute the bound
        sorted_y = sorted(norm_y, reverse = True)
        res.append(sqrt(tau**2*sum([x**2 for x in sorted_y[:i_max]])+(leftover*tau)*sorted_y[i_max]**2)/sqrt(n*tau))
    return([np.nanquantile(res, r/100) for r in rate])
#print("Best 25%, 50% and max values for the two bounds: "+str(S_estimate(10000, 256, 6, 1, 58, [10,25,50], 1, 2)))
