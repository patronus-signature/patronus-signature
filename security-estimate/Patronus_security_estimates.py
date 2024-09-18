from sub_procedures.MSIS_security import MSIS_summarize_attacks, MSISParameterSet
from sub_procedures.MLWE_security import MLWE_summarize_attacks, MLWEParameterSet
from math import sqrt, log, pi, floor, ceil
from scipy.special import betaincinv, gammaln
from sub_procedures.entropy_coordinate_hyperball import compute_entropy_coordinate, compute_f_table
from sub_procedures.tau_computer import tau_computer
from sub_procedures.gamma_Estimator import gamma_estimate
import sys
import random

# This script is a modification of the one for Dilithium, available at
# https://github.com/pq-crystals/security-estimates
# and translated from Haetae to Patronus: https://kpqc.cryptolab.co.kr/

weak_uf = True
key_recovery = True
fast_LWE = True
size = True



###############
# Preliminaries
###############

class PatronusParameterSet(object):
    def __init__(self, q, n=256, k=1, l=1, eta=5, r=1, tau=0, beta=120, gamma=512, omega=70, d=0):
        self.q     = q
        self.n     = n
        self.k     = k
        self.l     = l
        self.eta   = eta
        self.r     = r
        self.tau   = tau
        self.beta  = beta
        self.gamma = gamma
        self.omega = omega
        self.d     = d
        self.zeta = max(2.*self.gamma + 1. + (2**(self.d-1))*self.tau, self.r)


####################
# Schemes parameters
####################

n     =    256

all_params = []
all_params += [("Patronus II", PatronusParameterSet(  523777, n, k=5, l=4, eta=2, r=180350, tau=39, beta=194, gamma=130944, omega=70, d=13))]
all_params += [("Patronus III", PatronusParameterSet(  523777, n, k=7, l=5, eta=2, r=210425, tau=49, beta=238, gamma=130944, omega=80, d=14))]
all_params += [("Patronus V", PatronusParameterSet(  1047041, n, k=10, l=7, eta=2, r=467345, tau=60, beta=287, gamma=523520, omega=50, d=14))]



#########################
# Conversion to MSIS/MLWE
#########################

def Patronus_to_MSIS(dps, strong_uf = False):
    return MSISParameterSet(dps.n, dps.k + dps.l + 1, dps.k, dps.zeta, dps.q, norm="linf")


def Patronus_to_MLWE(dps):
    return MLWEParameterSet(dps.n, dps.l, dps.k, dps.eta, dps.q, distr="binomial")

text_SIS = ["BKZ block-size $b$ to break SIS","Best Known Classical bit-cost","Best Known Quantum bit-cost","Best Plausible bit-cost"]
text_LWE = ["BKZ block-size $b$ to break LWE","Best Known Classical bit-cost","Best Known Quantum bit-cost","Best Plausible bit-cost"]



###################
# Security estimate
###################
def Patronus_Entropy(dps):
    """
    Approximation of the signature length.
    """
    size_c = dps.n 
    size_c_h = size_c + 8*(dps.omega + dps.k)
    return size_c_h + (compute_entropy_coordinate(dps.n*dps.l,floor(1.5*dps.r)))*(dps.n*dps.l)

def Patronus_PK_Size(dps):
    """
    Computes the expected size of a verification key.
    """
    if dps.d <0:
        return (256 + dps.k*dps.n*(int(log(dps.q,2)+1))) #The public vector is computed mod q
    return (256 + dps.k*dps.n*(int(log(dps.q,2)+1)-dps.d)) #The public vector is computed mod q



table_SIS = [len(all_params)*[0] for i in range(4)]
table_LWE = [len(all_params)*[0] for i in range(4)]
table_entropy = [len(all_params)*[0] for i in range(4)]

#For each selected scheme, build the estimate cost of selected attacks
j = 0
for (scheme, param) in all_params:
    print("\n"+scheme)
    print(param.__dict__)
    print("")
    if weak_uf:
        print("=== WEAK UF")
        v = MSIS_summarize_attacks(Patronus_to_MSIS(param))
        for i in range(4):
            table_SIS[i][j] = v[i]
    if key_recovery:
        print("=== SECRET KEY RECOVERY")
        v = MLWE_summarize_attacks(Patronus_to_MLWE(param),fast_LWE)
        for i in range(4):
            table_LWE[i][j] = v[i]
    if size:
        print("=== PK  SIZE")
        table_entropy[j] = Patronus_PK_Size(param)
        print(table_entropy[j]/8)        
        print("=== SIGNATURE SIZE")
        table_entropy[j] = Patronus_Entropy(param)
        print(table_entropy[j]/8)
    j+=1
