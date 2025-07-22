import numpy as np
import matplotlib.pyplot as plt
import pyvista as pv

import tensorly as tl
from tensorly.decomposition import parafac

def plot_tensor_data(tensor):
    grid = pv.StructuredGrid(X, Y, T)
    grid.point_data["values"] = tensor.flatten(order="F")
    

    p = pv.Plotter()
    p.add_mesh_clip_plane(grid)
    p.add_axes(xlabel='x',ylabel='y',zlabel='t')
    p.show()


if __name__ == "__main__":

    x = np.arange(-5,5.01,.1)
    y = np.arange(-6,6.01,.1)
    t = np.arange(0,10.01*np.pi,.1)
    
    X,Y,T = np.meshgrid(x,y,t)
    
    f1 = np.exp(-(X**2+.5*Y**2))*np.cos(2*T)
    f2 = (1/np.cosh(X)*np.tanh(X)*np.exp(-.2*Y**2))*np.sin(T)

    F = f1+f2

    T = tl.tensor(F)

    factors = parafac(T, rank=2)
    
    plot_tensor_data(F)


    print(factors[1][0].shape)
    print(factors[1][1].shape)
    print(factors[1][2].shape)

    fig, ax = plt.subplots(3,1)

    ax[0].plot(factors[1][0])
    ax[1].plot(factors[1][1])
    ax[2].plot(factors[1][2])
    plt.show()