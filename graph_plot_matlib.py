import matplotlib.pyplot as plt 
import numpy as np 

plt.xlim([0, 1000])
plt.ylim([0,70])

# plt.xticks(np.arange(0, 1100, 10))
# plt.yticks(np.arange(0, 150, 5))
#n^2 size
# x=[100, 225, 625, 1600, 3600, 8100, 10000, 22500, 2500, 1000000]

#n size
x=[10,15,25,40,60,90,100,150,50,1000]

# naive timings 
y1 = [0.039000,
0.060000,
0.186000,
1.950000,
13.379000,
52.161000,
46.594000,
9.452000,
151.798000,
214.982000] 
# plotting the line 1 points  
plt.plot(x, y1, label = "naive") 
  
# zhu points
y2 = [0.112000,
0.179000,
0.777000,
0.148000,
0.227000,
0.438000,
0.533000,
1.090000,
10.295000,
38.468000] 
# plotting the line 2 points  
plt.plot(x, y2, label = "zhu-takaoka") 

# baeza points
y3 = [0.115000,
0.068000,
0.133000,
0.536000,
1.556000,
4.488000,
1.939000,
0.295000,
1.416000,
7.628000] 
# plotting the line 2 points  
plt.plot(x, y3, label = "baeza-yates") 


# baker points
y4 = [0.125000,
0.093000,
0.216000,
0.739000,
2.067000,
9.501000,
5.450000,
3.054000,
20.441000,
35.881000] 
# plotting the line 2 points  
plt.plot(x, y4, label = "baker-bird") 
  
# naming the x axis 
plt.xlabel('Text Size (n)') 
# naming the y axis 
plt.ylabel('time') 
# giving a title to my graph 
plt.title('Comparison of 2D String Matching Algorithms') 
  
# show a legend on the plot 
plt.legend() 
  
# function to show the plot 
plt.show() 
