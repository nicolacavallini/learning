import sympy as sym
from sympy.plotting import plot
import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    z, v, m = sym.symbols('z v m')

    g = 1./sym.sqrt((2.*sym.pi*v))*sym.exp(-(z-m)**2/(2*v))
    dz_g = sym.diff(g,z,1)
    ddz_g = sym.diff(g,z,2)

    sym.pprint(g)
    sym.pprint(dz_g)
    sym.pprint(ddz_g)
    sym.printing.print_ccode(g)
    sym.printing.print_ccode(dz_g)
    sym.printing.print_ccode(ddz_g)

    means = [3,6]
    variances = [.25,1]

    m0 = means[0]
    v0 = variances[0]
    s0 = np.sqrt(v0)
    g0 = g.subs([(m,m0),(v,v0)])
    dz_g0 = dz_g.subs([(m,m0),(v,v0)])
    ddz_g0 = ddz_g.subs([(m,m0),(v,v0)])

    m1 = means[1]
    v1 = variances[1]
    s1 = np.sqrt(v1)
    g1 = g.subs([(m,m1),(v,v1)])
    dz_g1 = dz_g.subs([(m,m1),(v,v1)])
    ddz_g1 = ddz_g.subs([(m,m1),(v,v1)])

    gm = g1-g0
    dz_gm = dz_g1-dz_g0
    ddz_gm = ddz_g1-ddz_g0
    g1n = sym.lambdify(z,g1, modules=['numpy'])
    g0n = sym.lambdify(z,g0, modules=['numpy'])
    gmn = sym.lambdify(z,gm, modules=['numpy'])
    dz_gmn = sym.lambdify(z,dz_gm, modules=['numpy'])
    ddz_gmn = sym.lambdify(z,ddz_gm, modules=['numpy'])

    print "    gmn(5) = ", gmn(5)
    print " dz_gmn(5) = ", dz_gmn(5)
    print "ddz_gmn(5) = ", ddz_gmn(5)
    zn = np.linspace(m0-2*s0,m1+2*s1,100)
    z0 = (m0+m1)/2.#(m0+m1)/2.
    print z0
    plt.plot(zn,gmn(zn),'-b')
    plt.plot(zn,g0n(zn),'-g')
    plt.plot(zn,g1n(zn),'-y')
    #plt.plot(zn,dz_gmn(zn),'-r')
    plt.plot(z0,gmn(z0),'ro')
    plt.plot([m0,m0],[0,gmn(m0)],'-b')
    plt.plot([m1,m1],[0,gmn(m1)],'-b')


    plt.plot([m0-s0,m0-s0],[0,gmn(m0-s0)],'--b')
    plt.plot([m0+s0,m0+s0],[0,gmn(m0+s0)],'--b')

    plt.plot([m1-s1,m1-s1],[0,gmn(m1-s1)],':b')
    plt.plot([m1+s1,m1+s1],[0,gmn(m1+s1)],':b')

    plt.plot([m1,m1],[0,gmn(m1)],'-b')

    plt.grid(True)
    for i in range(10):
        z1 = z0 - gmn(z0)/dz_gmn(z0)
        z0 = z1
        plt.plot(z0,gmn(z0),'ro')
        #plt.plot(z0,dz_gmn(z0),'ro')
        print z0

    plt.plot(z0,g1n(z0),'ks')
    plt.plot(z0,g0n(z0),'ks')
    plt.show()
