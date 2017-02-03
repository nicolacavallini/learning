import numpy as np

if __name__ == "__main__":

    f = open("outfile.dat   ", "rb")
    data = np.fromfile(f, dtype=np.float64)
    f.close()

    print data
