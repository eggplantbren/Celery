from pylab import *
from numba import jit

# This is the script that was used to generate example_data.txt

seed(0)
t = sort(1500*rand(501))

[t1, t2] = meshgrid(t, t)
dt = t1 - t2
C = cos(2*pi*abs(dt)/20) * exp(-abs(dt)/1000.0)\
        + exp(-abs(dt)/20)

n = matrix(randn(len(t))).T
L = cholesky(C)

y = (L*n).T
y += 0.35*randn(len(t))

data = empty((len(t), 3))
data[:,0], data[:,1], data[:,2] = t, y, 0.3
savetxt('example_data.txt', data)

figure(figsize=(12,6))
errorbar(data[:,0], data[:,1], yerr=data[:,2], fmt=".", markersize=5)
xlabel("Time")
ylabel("Measurement")
show()

@jit
def periodogram(periods):
    """
    Compute the periodogram.
    """
    pgram = empty(ps.size)
    for i in range(len(periods)):
        sine   = sin(2*pi*t/periods[i])
        cosine = cos(2*pi*t/periods[i])
        pgram[i] = sum(sine*y/data[:,2])**2 + sum(cosine*y/data[:,2])**2
    return pgram

ps = linspace(0, 50, 1001)
pgram = periodogram(ps)
plot(ps, pgram)
xlabel("Period")
ylabel("Power")
title("Periodogram")
show()


# Log likelihood
y = matrix(data[:,1]).T
for i in range(0, len(t)):
  C[i, i] += 0.15**2
L = cholesky(C)
logl = -0.5*len(t)*log(2*pi) - 0.5*2*sum(log(diag(L))) - 0.5*y.T*inv(C)*y
print(logl)

