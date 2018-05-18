import numpy as np
import matplotlib.pyplot as plt

def get_normal_distribution(m,v):
    return lambda z : 1./(np.sqrt(2*np.pi*v))*np.exp(-(z-m)**2./(2.*v))

# def density_function(data,h):
#     basis_functions = [get_normal_distribution(d,h) for d in data]
#
#
#
# class KernelDensity:
#     def __init__(self,h,data):
#         basis_functions = [get_normal_distribution(d,h) for d in data]

if __name__ == "__main__":
    print("stocazzo")
    m0, s0, n0 = .0, .001, 30
    m1, s1, n1 = .03, .001, 30

    deta_t = 2**(-6)

    g0 = get_normal_distribution(m0,s0)
    g1 = get_normal_distribution(m1,s1)



    sample = np.concatenate((np.random.normal(m0, s0, n0),
                             np.random.normal(m1, s1, n1)))

    density = lambda z : sum( get_normal_distribution(s,.0001)(z) for s in sample)

    x = np.linspace(-.05,.05,200)

    plt.plot(sample,np.zeros(sample.shape),'ob')
    plt.plot(x,density(x))
    plt.show()
