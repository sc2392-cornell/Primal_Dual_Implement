import numpy as np
import csv
import matplotlib.pyplot as plt


def main():
    Num=[]
    V=[]
    H=[]
    Pen=[]
    with open('inputgraph.csv','r') as csvfile:
        reader=csv.reader(csvfile,delimiter='\t')
        for row in reader:
            Num.append(int(row[0]))
            V.append(int(row[1]))
            H.append(int(row[2]))
            Pen.append(int(row[3]))
    print Pen
    #===============

    PenTerms=[46, 75, 83, 53, 35, 43, 36, 6 ,45 ,62, 42, 24, 13, 67, 48]




    EdgeMap={}
    with open('edges.csv','r') as csvfile:
        reader=csv.reader(csvfile,delimiter='\t')
        for row in reader:
            x1=int(row[1])
            x2=int(row[2])
            EdgeMap[str(x1)+','+str(x2)]=int(row[3])
    print EdgeMap
    #===============


    OPT9st=[]
    OPT9le=[]
    with open('OPT9.csv','r') as csvfile:
        reader=csv.reader(csvfile,delimiter='\t')
        counter=0
        for row in reader:
            if(counter==0):
                for item in row:
                    OPT9st.append(int(item))
            else:
                OPT9le.append((int(row[0]),int(row[1])))
            counter+=1
    print OPT9le
    print OPT9st
    ONLINE9edge1=[70,30,21,21,23,23,53,76,45,45,75,87,46,27,27,49,35,43,46,66,36,6,33,33]
    ONLINE9edge2=[83,70,30,74,74,98,98,83,76,75,99,99,87,46,93,93,49,46,73,73,66,21,53,62]


    OPT11st=[]
    OPT11le=[]
    with open('OPT11.csv','r') as csvfile:
        reader=csv.reader(csvfile,delimiter='\t')
        counter=0
        for row in reader:
            if(counter==0):
                for item in row:
                    OPT11st.append(int(item))
            else:
                OPT11le.append((int(row[0]),int(row[1])))
            counter+=1
    print OPT11le
    print OPT11st
    ONLINE11edge1=[70,30,21,21,23,23,53,76,45,45,75,87,46,27,27,49,35,43,46,66,36,6,33,33,66,2,2,66,24]
    ONLINE11edge2=[83,70,30,74,74,98,98,83,76,75,99,99,87,46,93,93,49,46,73,73,66,21,53,62,77,77,42,86,86]

    OPT13st=[]
    OPT13le=[]
    with open('OPT13.csv','r') as csvfile:
        reader=csv.reader(csvfile,delimiter='\t')
        counter=0
        for row in reader:
            if(counter==0):
                for item in row:
                    OPT13st.append(int(item))
            else:
                OPT13le.append((int(row[0]),int(row[1])))
            counter+=1
    print OPT13le
    print OPT13st

    ONLINE13edge1=[70,30,21,21,23,23,53,76,45,45,75,87,46,27,27,49,35,43,46,66,36,6,33,33,66,2,2,66,24,13,62]
    ONLINE13edge2=[83,70,30,74,74,98,98,83,76,75,99,99,87,46,93,93,49,46,73,73,66,21,53,62,77,77,42,86,86,73,67]


    #PlotOnline(V,H,EdgeMap,PenTerms,10,ONLINE9edge1, ONLINE9edge2)
    #PlotOnline(V,H,EdgeMap,PenTerms,12,ONLINE11edge1, ONLINE11edge2)
    #PlotOnline(V,H,EdgeMap,PenTerms,14,ONLINE13edge1, ONLINE13edge2)



    #PlotOptOffline(V,H,EdgeMap,OPT9le,OPT9st,PenTerms,10)
    #PlotOptOffline(V,H,EdgeMap,OPT11le,OPT11st,PenTerms,12)
    PlotOptOffline(V,H,EdgeMap,OPT13le,OPT13st,PenTerms,14)
    ##plots all three offline figures


def PlotOnline(V,H,EdgeMap,PenTerms,PenArrNum,OnlineEdge1, OnlineEdge2):
    fig,ax = plt.subplots()
    ax.plot(H,V,'o')
    #plot all given nodes

    for ind in range(PenArrNum):
        ax.plot(H[PenTerms[ind]-1],V[PenTerms[ind]-1],'ro')
    # plots arrived terminals

    for ind in range(len(OnlineEdge1)):
        node1=OnlineEdge1[ind]
        node2=OnlineEdge2[ind]
        x=np.linspace(H[node1-1],H[node2-1],50)
        y=np.linspace(V[node1-1],V[node2-1],50)
        ax.plot(x,y,'b-')
    plt.show()
    #######################

def PlotOptOffline(V,H,EdgeMap, OPTle, OPTst,PenTerms,PenArrNum):
    fig,ax = plt.subplots()
    ax.plot(H,V,'o')
    #plot all given nodes

    for ind in range(PenArrNum):
        ax.plot(H[PenTerms[ind]-1],V[PenTerms[ind]-1],'ro')
    # plots arrived terminals

    for edge in OPTle:
        node1=0
        node2=0
        (node1,node2)=edge
        x=np.linspace(H[node1-1],H[node2-1],50)
        y=np.linspace(V[node1-1],V[node2-1],50)
        ax.plot(x,y,'b-')
    #plot all bought edges




    plt.show()




if __name__ == "__main__":
    main()
