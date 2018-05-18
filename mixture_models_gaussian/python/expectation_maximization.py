import numpy as np

import matplotlib.pyplot as plt


def get_gaussian(m,s):
    return lambda x : 1./(np.sqrt(2.*np.pi*s*s))*np.exp(-1.*(x-m)**2./(2*s**2))

def test_gaussian_00():
    g1 = get_gaussian(0,1)
    x = np.linspace(-3,3)
    plt.plot(x,g1(x))
    plt.show()

def test_gaussian_01():
    mu, sigma = 5, 0.1 # mean and standard deviation
    s = np.random.normal(mu, sigma, 1000)
    g = get_gaussian(mu,sigma)

    count, bins, ignored = plt.hist(s, 30, normed=True)

    plt.plot(bins, g(bins),
                   linewidth=2, color='r')
    plt.show()

def gaussian_mixture(gaussian_list,pi_list,data):
    N = np.float64(data.shape[0])
    print "N = ", N

    for k in range(0,200):
        print "--- iter = ", k, " ---"

        prob_sum = np.zeros(data.shape)

        for p, g in zip(pi_list,gaussian_list):
            prob_sum += p*g(data)

        gamma_list = []
        for p, g in zip(pi_list,gaussian_list):
            gamma_list.append(p*g(data)/prob_sum)

        nk_list = []
        for gamma in gamma_list:
            nk_list.append(np.sum(gamma))

        #print nk_list

        mk_list = []
        for gamma, nk in zip(gamma_list,nk_list):
            mk_list.append(np.sum(gamma*data)/nk)

        # print mk_list

        sk_list = []
        for gamma, nk, mk in zip(gamma_list,nk_list,mk_list):
            sk_list.append(np.sqrt(np.sum(gamma*(data - mk)**2)/nk))
        # print sk_list

        pi_list = []
        for nk in nk_list:
            pi_list.append(nk/N)

        # print pi_list

        gaussian_list = []
        for mk, sk in zip(mk_list,sk_list):
            gaussian_list.append(get_gaussian(mk,sk))
        print "means    =", mk_list
        print "std dev  =", sk_list

def test_gaussian_mixture_00():

    data = np.array([-0.39,0.12,0.94,0.06,0.48,1.01,1.67,1.76,2.44,3.72,4.28,4.92,5.53,1.68,1.80,3.25,4.12,4.60,5.28,6.22])

    m1 = 2.
    s1 = 3.
    m2 = 1.
    s2 = .1
    p = .1

    gaussian_list = []
    gaussian_list.append(get_gaussian(m1,s1))
    gaussian_list.append(get_gaussian(m2,s1))

    pi_list = []
    pi_list.append(p)
    pi_list.append(1.-p)

    gaussian_mixture(gaussian_list,pi_list,data)

def test_gaussian_mixture_01():
    m1 = 2.
    s1 = 3.

    m2 = 2.
    s2 = .2

    gaussian_list = []
    gaussian_list.append(get_gaussian(m1,s1))
    gaussian_list.append(get_gaussian(m2,s1))

    n1 = 200
    n2 = 100

    nk_list = [n1,n2]

    d1 = np.random.normal(m1, s1, n1)
    d2 = np.random.normal(m2, s2, n2)

    data = np.hstack((d1,d2))

    n, bins, patches = plt.hist(data,50)
    for g, nk in zip(gaussian_list,nk_list):
        plt.plot(bins,g(bins))
    plt.show()



if __name__ == "__main__":
    test_gaussian_mixture_00()
