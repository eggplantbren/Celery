from pylab import *
from numba import jit

# This is the script that was used to generate example_data.txt

seed(0)
t = sort(1000*rand(500))

[t1, t2] = meshgrid(t, t)
dt = t1 - t2

# Amplitude, period, quality
A, P, Q = 1.0, 10.0, 10.0
w0 = 2*pi/P
tau = abs(dt)
eta = sqrt(1.0 - 1.0/(4.0*Q**2))
C = A**2*exp(-w0*tau/(2*Q))*\
        (cos(eta*w0*tau) + sin(eta*w0*tau)/(2.0*eta*Q))\
        + 1.5**2*exp(-abs(dt)/10)

n = matrix(randn(len(t))).T
L = cholesky(C)

y = (L*n).T
y += 0.9*randn(len(t))

data = empty((len(t), 3))
data[:,0], data[:,1], data[:,2] = t, y, 0.3
savetxt('example_data.txt', data)

figure(figsize=(12,6))
errorbar(data[:,0], data[:,1], yerr=data[:,2], fmt=".", markersize=5)
xlabel("Time")
ylabel("Measurement")
show()

@jit
def periodogram(fs):
    """
    Compute the periodogram.
    """
    pgram = empty(len(fs))
    for i in range(len(fs)):
        sine   = sin(2*pi*fs[i]*t)
        cosine = cos(2*pi*fs[i]*t)
        pgram[i] = sum(sine*y/data[:,2])**2 + sum(cosine*y/data[:,2])**2
    return pgram/len(fs)

fs = linspace(0.01, 0.2, 1001)
pgram = periodogram(fs)
plot(fs, pgram)
xlabel("Frequency")
ylabel("Power")
title("Periodogram")
show()


# Log likelihood
y = matrix(data[:,1]).T
for i in range(0, len(t)):
  C[i, i] += 0.9**2
L = cholesky(C)
logl = -0.5*len(t)*log(2*pi) - 0.5*2*sum(log(diag(L))) - 0.5*y.T*inv(C)*y
print(logl)

