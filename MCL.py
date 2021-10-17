import math
import markov_clustering as mc
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#adds self loops
def addSelfLoop(Matrix): 
    size = len(Matrix)
    for i in range(size):
        Matrix[i][i] = 1
    return Matrix

#creates markov chain transition matrix
def createTransition(Matrix):
    size = len(Matrix)
    Transition = [[0 for i in range(size)] for j in range(size)]
    for j in range(size):
        sum = 0
        for i in range(size):
            sum += Matrix[i][j]
        for i in range(size):
            Transition[i][j] = round(Matrix[i][j]/sum, 2)
    return Transition

#takes the e-th power of the matrix
def expand(Transition):
    size = len(Transition)
    Expansion = [[0 for i in range(size)] for j in range(size)]
    for i in range(size):
        for j in range(size):
            sum = 0
            for k in range(size):
                sum += Transition[i][k] * Transition[k][j]
            Expansion[i][j] = round(sum,2)
    return Expansion

#Inflate by taking inflation of the resulting matrix with parameter r
def inflate(Expansion, power):
    size = len(Expansion)
    Inflation = [[0 for i in range(size)] for j in range(size)]
    for i in range(size):
        for j in range(size):
            Inflation[i][j] = math.pow(Expansion[i][j],power)

    for j in range(size):
        sum = 0
        for i in range(size):
            sum += Inflation[i][j]
        for i in range(size):
            Inflation[i][j] = round(Inflation[i][j]/sum, 2)
    return Inflation

def change(Matrix1, Matrix2):
    size = len(Matrix1)
    change = 0
    for i in range(size):
        for j in range(size):
            if(math.fabs(Matrix1[i][j]-Matrix2[i][j]) > change):
                change = math.fabs(Matrix1[i][j]-Matrix2[i][j])
    return change

def MCL(Matrix):
    
    Matrix = addSelfLoop(Matrix)
    print (pd.DataFrame(Matrix))
    Gamma = 2
    Transition = createTransition(Matrix)
    M1 = Transition
    print ("Transition")
    print (pd.DataFrame(M1))
    
    counter =1
    epsilon = 0.001
    change_ = float("inf")
    while (change_ > epsilon):
        print("Iterate :: ", counter,":::::::::::::::::::::::::::::")
        counter += 1
        # M_2 =  M_1 * M_1  #  expansion  
        M2 = expand(M1)
        print ("expanded\n",pd.DataFrame(M2))
        # M_1 =  Γ(M_2)     #  inflation
        M1 = inflate(M2, 2)
        print ("inflated\n",pd.DataFrame(M1))
        # change   =  difference(M_1, M_2)
        change_ = change(M1,M2)
    return M1

#test
#          0  1  2  3  4  5  6  7  
Matrix = [[0, 1, 0, 1, 0, 0, 0, 0], #0
          [1, 0, 1, 1, 0, 1, 0, 0], #1
          [0, 1, 0, 1, 1, 0, 0, 0], #2
          [1, 1, 1, 0, 0, 0, 0, 0], #3
          [0, 0, 1, 0, 0, 1, 1, 1], #4
          [0, 1, 0, 0, 1, 0, 1, 0], #5
          [0, 0, 0, 0, 1, 1, 0, 1], #6
          [0, 0, 0, 0, 1, 0, 1, 0]] #7


my_graph = nx.Graph() 
my_graph.add_edges_from([
                        (0,1),
                        (0,3),
                        (1,2),
                        (1,3),
                        (1,5),
                        (2,3),
                        (2,4),
                        (4,5),
                        (4,6),
                        (4,7),
                        (5,6),
                        (6,7)])
nx.draw(my_graph, with_labels=True, font_weight='bold')
result = mc.run_mcl(np.array(Matrix))  

