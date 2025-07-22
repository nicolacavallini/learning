# Authors: The scikit-learn developers
# SPDX-License-Identifier: BSD-3-Clause

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LogNorm

from sklearn import mixture

n_samples = 5000

# generate random sample, two components
np.random.seed(0)

# generate spherical data centered on (20, 20)
shifted_gaussian = np.random.randn(n_samples) + np.array([5])

print(shifted_gaussian.shape)

# generate zero centered stretched Gaussian data
stretched_gaussian = np.random.randn(n_samples)*5

print(stretched_gaussian.shape)

# concatenate the two datasets into the final training set
X_train = np.vstack([shifted_gaussian, stretched_gaussian]).flatten()

print(X_train.shape)


# fit a Gaussian Mixture Model with two components
clf = mixture.GaussianMixture(n_components=2, covariance_type="full",tol=1e-7,verbose=1)
clf.fit(X_train.reshape(-1, 1))

print(clf.means_)

print(clf.covariances_)
print(clf.weights_)

x_ = np.linspace(-20, 20, 1000)

plt.hist(X_train, bins=50, density=True)


for m_, v_, w_ in zip(clf.means_.flatten(), clf.covariances_.flatten(), clf.weights_):
    gauss = w_*np.exp(-0.5*(x_-m_)**2/v_)/np.sqrt(2*np.pi*v_)
    plt.plot(x_, gauss)



plt.show()


# display predicted scores by the model as a contour plot
#x = np.linspace(-20.0, 30.0)
#y = np.linspace(-20.0, 40.0)
#X, Y = np.meshgrid(x, y)
#XX = np.array([X.ravel(), Y.ravel()]).T
#Z = -clf.score_samples(XX)
#Z = Z.reshape(X.shape)
#
#CS = plt.contour(
#    X, Y, Z, norm=LogNorm(vmin=1.0, vmax=1000.0), levels=np.logspace(0, 3, 10)
#)
#CB = plt.colorbar(CS, shrink=0.8, extend="both")
#plt.scatter(X_train[:, 0], X_train[:, 1], 0.8)
#
#plt.title("Negative log-likelihood predicted by a GMM")
#plt.axis("tight")
#plt.show()"""