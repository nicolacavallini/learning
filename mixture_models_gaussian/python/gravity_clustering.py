import numpy as np
import matplotlib.pyplot as plt
from scipy import special

import math

def get_normal_distribution(m,v):
    return lambda z : 1./(np.sqrt(2*np.pi*v))*np.exp(-(z-m)**2./(2.*v))

def get_gravity(m,v):
    g = get_normal_distribution(m,v)
    return lambda z: -.5*special.erf((z-m)/(np.sqrt(2.)*np.sqrt(v)))




if __name__ == "__main__":

    #g = get_normal_distribution(0,1)

    m0, s0 = .0, .1
    m1, s1 = 1, .1

    deta_t = 2**(-6)

    g0 = get_normal_distribution(m0,s0)
    g1 = get_normal_distribution(m1,s1)

    sample = np.concatenate((np.random.normal(m0, s0, 10),
                             np.random.normal(m1, s1, 10)))

    sample = np.concatenate((sample,[-.7]))

    center = np.mean(sample)
    velocity = np.float64(sample.shape[0])/2.

    rotation = lambda z : velocity *(z-center)

    stanadr_deviation = .05
    variance = stanadr_deviation**2

    cumulavive_sample = sample

    for t  in range(20):
        g_list = [get_normal_distribution(s,variance) for s in sample]

        density = lambda z : sum( g(z) for g in g_list)

        f_list = [get_gravity(s,variance) for s in sample]

        gravity = lambda z : sum( f(z) for f in f_list)

        force = lambda z : (gravity(z) + rotation(z))*density(z)

        s = np.linspace(-1,2,400)

        # plt.plot(s,density(s))
        # # plt.plot(s,g0(s),)
        # # plt.plot(s,g1(s))
        # #plt.plot([m0,m1],np.amax(density(s))*np.ones(2),'xk')
        # plt.plot(s,force(s),'g')
        # plt.plot(sample,np.zeros(sample.shape),'or')
        # plt.plot(np.sort(sample),np.zeros(sample.shape),'--r')
        # plt.show()

        sample = sample + force(sample) * deta_t**2.

        cumulavive_sample = np.vstack((cumulavive_sample,sample))


    plt.plot(cumulavive_sample)
    plt.show()
