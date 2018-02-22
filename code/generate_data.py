from pylab import *

seed(0)
t = sort(100*rand(1001))

[t1, t2] = meshgrid(t, t)
dt = t1 - t2
C = cos(2*pi*abs(dt)/20) * exp(-abs(dt)/1000.0)

n = matrix(randn(len(t))).T
L = cholesky(C)

y = (L*n).T
y += 0.15*randn(len(t))

data = empty((len(t), 3))
data[:,0], data[:,1], data[:,2] = t, y, 0.1
savetxt('data.txt', data)

plot(data[:,0], data[:,1], 'o')
show()

# Log likelihood
y = matrix(data[:,1]).T
for i in range(0, len(t)):
  C[i, i] += 0.15**2
L = cholesky(C)
logl = -0.5*len(t)*log(2*pi) - 0.5*2*sum(log(diag(L))) - 0.5*y.T*inv(C)*y
print(logl)

