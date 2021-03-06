import sympy as sp
import numpy as np
import matplotlib.pyplot as plt

def get_normal_distribution(m,s):
    return lambda z : 1./(np.sqrt(2*np.pi*s*s))*np.exp(-(z-m)**2./(2.*s*s))

def get_normal_distribution_prime(m,s):
    return lambda z : -np.sqrt(2)*(-2*m + 2*z)*np.exp(-(-m + z)**2/(2*s**2))/(4*s**2*np.sqrt(np.pi*s**2))

def get_normal_distribution_prime_prime(m,s):
    return lambda z : np.sqrt(2)*(-1 + (m - z)**2/s**2)*np.exp(-(m - z)**2/(2*s**2))/(2*s**2*np.sqrt(np.pi*s**2))

def newtons_method(residuo,tangente,ic):
    x0 = ic
    converged = True
    for i in range(40):
        #samples.append(x0)

        x = x0 -residuo(x0)/tangente(x0)
        x0 = x
        if np.sqrt(residuo(x)*residuo(x)) < 1e-12:
            break
        if i == 39:
            return np.nan, False
    return x,converged


class NormalDistributionDensity:
    def __init__(self,sample,sigma):
        self.function = lambda x : sum( get_normal_distribution(s,sigma)(x) for s in sample)
        self.prime = lambda x : sum( get_normal_distribution_prime(s,sigma)(x) for s in sample)
        self.prime_prime = lambda x : sum( get_normal_distribution_prime_prime(s,sigma)(x) for s in sample)

        self.x_min = np.min(sample)
        self.x_max = np.max(sample)

        self.length = (np.amax(sample) - np.amin(sample))
        self.initial_values_array = np.sort(sample)

    def find_local_maxima(self):

        values = self.initial_values_array

        maxima = np.array([np.nan])

        for initial_guess in values:

            (z_local_max,converged) = newtons_method(self.prime,self.prime_prime,initial_guess)

            concavity = self.prime_prime(z_local_max)

            first_maximum_fuond = np.isnan(maxima)[0] and concavity < 0
            distance = np.amin(np.abs(z_local_max - maxima))
            # print("distance")
            # print(distance)
            distinct_maxima = distance > 1e-4

            other_maxima_fuond = (distinct_maxima and concavity < 0) and np.isfinite(maxima)[0]
            if first_maximum_fuond and converged:
                maxima = np.array([z_local_max])
            if other_maxima_fuond and converged:
                maxima = np.concatenate((maxima,[z_local_max]))

        return maxima




def symbolic_definitions():
    s,z,p,m = sp.symbols('s z p m')

    g = 1/(sp.sqrt(2*p*s**2))*sp.exp(-(z-m)**2/(2*s**2))
    dg = sp.diff(g,z)
    ddg = sp.diff(g,z,z)
    print("====================")
    sp.pprint(g)
    sp.pprint(dg)
    sp.pprint(ddg)
    print("====================")
    print("====================")
    print(sp.python(g))
    print(sp.python(dg))
    print(sp.python(ddg))
    print("====================")

def numeric_test():

    g = get_normal_distribution(0,1)
    dg = get_normal_distribution_prime(0,1)
    ddg = get_normal_distribution_prime_prime(0,1)

    x = np.linspace(-3,3)

    plt.plot(x,g(x))
    plt.plot(x,dg(x))
    plt.plot(x,ddg(x))
    plt.show()

    # samples = np.array(samples)
    #
    # z = np.linspace(-.5,1,100)
    # plt.plot(z,residuo(z))
    # plt.plot(samples,residuo(samples),"ob")
    # plt.grid()
    # plt.show()


def test_find_distribution_maxima():

    # mean = np.array([.0,.5,1.,1.5])
    # std = np.array([.1,.1,.2,.3])
    mean = np.array([.0,.5])
    std = np.array([.1,.1])

    sample = np.array([])

    for m,s in zip(mean,std):
        sample = np.concatenate((sample,np.random.normal(m, s, 10)))

    ndd = NormalDistributionDensity(sample,.1)
    maxima = ndd.find_local_maxima()
    print(maxima)

    length = (np.amax(sample) - np.amin(sample))

    x = np.linspace(np.amin(sample)-.1*length,np.amax(sample)+.1*length,100)
    plt.plot(x,ndd.function(x))
    plt.plot(maxima,ndd.function(maxima),'ob')
    plt.plot(sample,np.zeros(sample.shape),'or')
    plt.grid()
    plt.show()


if __name__ == "__main__":
    #symbolic_definitions()
    #numeric_test()
    test_find_distribution_maxima()
