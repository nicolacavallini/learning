import numpy as np
import matplotlib.pyplot as plt
from scipy import special

import math

def get_normal_distribution(m,v):
    return lambda z : 1./(np.sqrt(2*np.pi*v))*np.exp(-(z-m)**2./(2.*v))

def get_gravity(m,v):
    g = get_normal_distribution(m,v)
    return lambda z: .5*special.erf((z-m)/(np.sqrt(2.)*np.sqrt(v)))




if __name__ == "__main__":

    #g = get_normal_distribution(0,1)

    sample = np.concatenate((np.random.normal(0, .1, 10),
                             np.random.normal(.3, .1, 10)))

    sample = np.concatenate((sample,[-.7]))

    stanadr_deviation = .05
    variance = stanadr_deviation**2



    g_list = [get_normal_distribution(s,variance) for s in sample]

    density = lambda z : sum( g(z) for g in g_list)

    f_list = [get_gravity(s,variance) for s in sample]

    force = lambda z : sum( f(z) for f in f_list)

    # mu = np.mean(sample)
    #
    # distance = lambda z : sum( for s in sample)
    #
    #
    s = np.linspace(-1,2,400)

    plt.plot(s,density(s))
    plt.plot(s,force(s),'g')
    plt.plot(sample,np.zeros(sample.shape),'or')
    plt.plot(np.sort(sample),np.zeros(sample.shape),'--r')
    plt.show()
