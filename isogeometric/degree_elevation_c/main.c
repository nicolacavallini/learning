#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* convert c vector to c matrix. */
double **vec2mat(double *vec, int nrows, int ncols)
{
  int col;
  double **mat;

  mat = (double**) malloc (ncols*sizeof(double*));
  mat[0] = vec;
  for (col = 1; col < ncols; col++)
    mat[col] = mat[col-1] + nrows;
  return mat;
}

/* create a new c matrix */
double **matrix(int nrows, int ncols)
{
  int col;
  double **mat;

  mat = (double**) malloc (ncols*sizeof(double*));
  mat[0] = (double*) malloc (nrows*ncols*sizeof(double));
  for (col = 1; col < ncols; col++)
    mat[col] = mat[col-1] + nrows;
  return mat;
}

void freevec2mat(double **mat)
{
  free(mat);
}

void freematrix(double **mat)
{
  free(mat[0]);
  free(mat);
}


#define min(x,y) (x < y ? x : y)
#define max(x,y) (x > y ? x : y)

/* Compute logarithm of the gamma function */
/* Algorithm from 'Numerical Recipes in C, 2nd Edition' pg214. */
double gammaln(double xx)
{
  double x,y,tmp,ser;
  static double cof[6] = {76.18009172947146,-86.50532032291677,
                          24.01409824083091,-1.231739572450155,
                          0.12086650973866179e-2, -0.5395239384953e-5};
  int j;
  y = x = xx;
  tmp = x + 5.5;
  tmp -= (x+0.5) * log(tmp);
  ser = 1.000000000190015;
  for (j=0; j<=5; j++) ser += cof[j]/++y;
  return -tmp+log(2.5066282746310005*ser/x);
}

/* computes ln(n!) */
/* Numerical Recipes in C */
/* Algorithm from 'Numerical Recipes in C, 2nd Edition' pg215. */
double factln(int n)
{
  static int ntop = 0;
  static double a[101];

  if (n <= 1) return 0.0;
  while (n > ntop)
  {
    ++ntop;
    a[ntop] = gammaln(ntop+1.0);
  }
  return a[n];
}

/* Computes the binomial coefficient. */

/*     ( n )      n!                  */
/*     (   ) = --------               */
/*     ( k )   k!(n-k)!               */

/* Algorithm from 'Numerical Recipes in C, 2nd Edition' pg215. */
double bincoeff(int n, int k)
{
  return floor(0.5+exp(factln(n)-factln(k)-factln(n-k)));
}

void print_matrix(double **m, int rows, int cols){
    int i = 0;
    int j = 0;
    for (i =0; i < rows; i++){
        for (j =0; j < cols; j++){
            printf("%e, ",m[j][i]);
        }
        printf("\n");
    }
}

/* Degree elevate a B-Spline t times. */
/* i.e. from d to d+t */

/* INPUT: */

/*   n,p,U,Pw,t  */

/* OUTPUT:  */

/*   nh,Uh,Qw  */

/* Modified version of Algorithm A5.9 from 'The NURBS BOOK' pg206. */

int bspdegelev(int d, double *c, int mc, int nc, double *k, int nk,
               int t, int *nh, double *ic, double *ik)
{
  int row,col;

  int ierr = 0;
  int i, j, q, s, m, ph, ph2, mpi, mh, r, a, b, cind, oldr, mul;
  int n, lbz, rbz, save, tr, kj, first, kind, last, bet, ii;
  double inv, ua, ub, numer, den, alf, gam;
  double **bezalfs, **bpts, **ebpts, **Nextbpts, *alfs;

  double **ctrl  = vec2mat(c, mc, nc);
  double **ictrl = vec2mat(ic, mc, nc*(t+1));

  n = nc - 1;

  printf("nc = %d\n", nc);
  printf("mc = %d\n", mc);

  bezalfs = matrix(d+1,d+t+1);
  bpts = matrix(mc,d+1);
  ebpts = matrix(mc,d+t+1);
  Nextbpts = matrix(mc,d+1);
  alfs = (double *) malloc(d*sizeof(double));

  m = n + d + 1;
  ph = d + t;
  ph2 = ph / 2;

  printf("m = %d\n",m);
  printf("ph = %d\n",ph);
  printf("ph2 = %d\n",ph2);

  /* compute bezier degree elevation coefficients   */
  bezalfs[0][0] = bezalfs[ph][d] = 1.0;
  //bezalfs[0][0] = bezalfs[d][ph] = 1.0;
  //printf("bezalfs = \n");
  //print_matrix(bezalfs,d+1,d+t+1);

  for (i = 1; i <= ph2; i++)
  {
    inv = 1.0 / bincoeff(ph,i);
    mpi = min(d,i);

    for (j = max(0,i-t); j <= mpi; j++){
        printf("(%d, %d) = %e\n",i,j, inv * bincoeff(d,j) * bincoeff(t,i-j));
      bezalfs[i][j] = inv * bincoeff(d,j) * bincoeff(t,i-j);
  }
  }

  for (i = ph2+1; i <= ph-1; i++)
  {
    mpi = min(d, i);
    for (j = max(0,i-t); j <= mpi; j++)
      bezalfs[i][j] = bezalfs[ph-i][d-j];
  }
  // printf("bezalfs = \n");
  // print_matrix(bezalfs,d+1,d+t+1);

  mh = ph;
  kind = ph+1;
  r = -1;
  a = d;
  b = d+1;
  cind = 1;
  ua = k[0];

  for (ii = 0; ii < mc; ii++)
    ictrl[0][ii] = ctrl[0][ii];

  for (i = 0; i <= ph; i++)
    ik[i] = ua;

  /* initialise first bezier seg */
  for (i = 0; i <= d; i++)
    for (ii = 0; ii < mc; ii++)
      bpts[i][ii] = ctrl[i][ii];

  /* big loop thru knot vector */
  while (b < m)
  {
    i = b;
    while (b < m && k[b] == k[b+1])
      b++;

    mul = b - i + 1;
    mh += mul + t;
    ub = k[b];
    oldr = r;
    r = d - mul;

    /* insert knot u(b) r times */
    if (oldr > 0)
      lbz = (oldr+2) / 2;
    else
      lbz = 1;

    if (r > 0)
      rbz = ph - (r+1)/2;
    else
      rbz = ph;

    if (r > 0)
    {
      /* insert knot to get bezier segment */
      numer = ub - ua;
      for (q = d; q > mul; q--)
        alfs[q-mul-1] = numer / (k[a+q]-ua);
      for (j = 1; j <= r; j++)
      {
        save = r - j;
        s = mul + j;

        for (q = d; q >= s; q--)
          for (ii = 0; ii < mc; ii++)
            bpts[q][ii] = alfs[q-s]*bpts[q][ii]+(1.0-alfs[q-s])*bpts[q-1][ii];

        for (ii = 0; ii < mc; ii++)
          Nextbpts[save][ii] = bpts[d][ii];
      }
    }
    /* end of insert knot */

    /* degree elevate bezier */
    for (i = lbz; i <= ph; i++)
    {
      for (ii = 0; ii < mc; ii++)
        ebpts[i][ii] = 0.0;
      mpi = min(d, i);
      for (j = max(0,i-t); j <= mpi; j++)
        for (ii = 0; ii < mc; ii++)
          ebpts[i][ii] = ebpts[i][ii] + bezalfs[i][j]*bpts[j][ii];
    }
    /* end of degree elevating bezier */

    if (oldr > 1)
    {
      /* must remove knot u=k[a] oldr times */
      first = kind - 2;
      last = kind;
      den = ub - ua;
      bet = (ub-ik[kind-1]) / den;

      /* knot removal loop */
      for (tr = 1; tr < oldr; tr++)
      {
        i = first;
        j = last;
        kj = j - kind + 1;
        while (j - i > tr)
        {
          /* loop and compute the new control points */
      /* for one removal step    */
          if (i < cind)
          {
            alf = (ub-ik[i])/(ua-ik[i]);
            for (ii = 0; ii < mc; ii++)
              ictrl[i][ii] = alf * ictrl[i][ii] + (1.0-alf) * ictrl[i-1][ii];
          }
          if (j >= lbz)
          {
            if (j-tr <= kind-ph+oldr)
            {
              gam = (ub-ik[j-tr]) / den;
              for (ii = 0; ii < mc; ii++)
                ebpts[kj][ii] = gam*ebpts[kj][ii] + (1.0-gam)*ebpts[kj+1][ii];
            }
            else
            {
              for (ii = 0; ii < mc; ii++)
                ebpts[kj][ii] = bet*ebpts[kj][ii] + (1.0-bet)*ebpts[kj+1][ii];
            }
          }
          i++;
          j--;
          kj--;
        }

        first--;
        last++;
      }
    }
    /* end of removing knot n=k[a] */

    /* load the knot ua  */
    if (a != d)
      for (i = 0; i < ph-oldr; i++)
      {
        ik[kind] = ua;
        kind++;
      }

    /* load ctrl pts into ic  */
    for (j = lbz; j <= rbz; j++)
    {
      for (ii = 0; ii < mc; ii++)
        ictrl[cind][ii] = ebpts[j][ii];
      cind++;
    }

    if (b < m)
    {
      /* setup for next pass thru loop  */
      for (j = 0; j < r; j++)
        for (ii = 0; ii < mc; ii++)
          bpts[j][ii] = Nextbpts[j][ii];
      for (j = r; j <= d; j++)
        for (ii = 0; ii < mc; ii++)
          bpts[j][ii] = ctrl[b-d+j][ii];
      a = b;
      b++;
      ua = ub;
    }
    else
      /* end knot  */
      for (i = 0; i <= ph; i++)
        ik[kind+i] = ub;
  }
  /* end while loop  */

  *nh = mh - ph - 1;

  freevec2mat(ctrl);
  freevec2mat(ictrl);
  freematrix(bezalfs);
  freematrix(bpts);
  freematrix(ebpts);
  freematrix(Nextbpts);
  free(alfs);

  return(ierr);
}


void print_vec(double *v, int size){
    int i;
    for (i = 0; i < size; i ++)
        printf("v[%d] = %e \n",i,v[i]);

}

double * allocate_vec_double(int size){
    double * v = (double*)malloc(size*sizeof(double));
    int i = 0;
    for (i = 0; i< size; i++)
        v[i] = 0;

    return v;
}

int findspan(int n, int p, double u, double *U)
{
  int low, high, mid;

  /* special case */
  if (u == U[n+1]) return(n);

  /* do binary search */
  low = p;
  high = n + 1;
  mid = (low + high) / 2;
  while (u < U[mid] || u >= U[mid+1])
  {
    if (u < U[mid])
      high = mid;
    else
      low = mid;
    mid = (low + high) / 2;
  }

  return(mid);
}

int bspkntins(int d, double *c, int mc, int nc, double *k, int nk,
              double *u, int nu, double *ic, double *ik)
{
  int ierr = 0;
  int a, b, r, l, i, j, m, n, s, q, ind;
  double alfa;

  double **ctrl  = vec2mat(c, mc, nc);
  double **ictrl = vec2mat(ic, mc, nc+nu);

  n = nc - 1;
  r = nu - 1;

  m = n + d + 1;
  a = findspan(n, d, u[0], k);
  b = findspan(n, d, u[r], k);
  ++b;

  for (q = 0; q < mc; q++)
  {
    for (j = 0; j <= a-d; j++) ictrl[j][q] = ctrl[j][q];
    for (j = b-1; j <= n; j++) ictrl[j+r+1][q] = ctrl[j][q];
  }
  for (j = 0; j <= a; j++)   ik[j] = k[j];
  for (j = b+d; j <= m; j++) ik[j+r+1] = k[j];

  i = b + d - 1;
  s = b + d + r;
  for (j = r; j >= 0; j--)
  {
    while (u[j] <= k[i] && i > a)
    {
      for (q = 0; q < mc; q++)
        ictrl[s-d-1][q] = ctrl[i-d-1][q];
      ik[s] = k[i];
      --s;
      --i;
    }
    for (q = 0; q < mc; q++)
      ictrl[s-d-1][q] = ictrl[s-d][q];
    for (l = 1; l <= d; l++)
    {
      ind = s - d + l;
      alfa = ik[s+l] - u[j];
      if (fabs(alfa) == 0.0)
        for (q = 0; q < mc; q++)
          ictrl[ind-1][q] = ictrl[ind][q];
      else
      {
        alfa /= (ik[s+l] - k[i-d+l]);
        for (q = 0; q < mc; q++)
          ictrl[ind-1][q] = alfa*ictrl[ind-1][q]+(1.0-alfa)*ictrl[ind][q];
      }
    }

    ik[s] = u[j];
    --s;
  }

  freevec2mat(ctrl);
  freevec2mat(ictrl);

  return ierr;
}

void test_degree_elevation(){

    int degree = 1;
    int n_knots = 4;

    double * knots  = allocate_vec_double(n_knots);


    knots[0] = 0;
    knots[1] = 0;
    knots[2] = 1;
    knots[3] = 1;

    print_vec(knots,n_knots);
    printf("...allocated knot vector.\n");

    int dim = 3;
    int n_control_points = 2;

    double * control_points  = allocate_vec_double(
        dim*n_control_points);


    control_points[0] = 0;
    control_points[1] = 0;
    control_points[2] = 0;
    control_points[3] = 1;
    control_points[4] = 0;
    control_points[5] = 0;

    print_vec(control_points,dim*n_control_points);

    printf("...allocated control points.\n");

    int degree_elevation = 1;

    int nh = 0; // output
    /* allocate work space t times larger than original number */
    /* of control points and knots */
    double * inc = allocate_vec_double((degree_elevation+1)*dim*n_control_points);
    double * ink = allocate_vec_double((degree_elevation+1)*n_knots);

     bspdegelev(degree,control_points,dim,n_control_points,
                knots,n_knots,degree_elevation,&nh,inc,ink);

    printf("nh = %d\n", nh);
    //
    printf("inc = \n");
    print_vec(inc,(degree_elevation+1)*dim*n_control_points);

    printf("ink = \n");
    print_vec(ink,(degree_elevation+1)*n_knots);

    printf("...done.\n");
    free(knots);
    free(control_points);
    free(inc);
    free(ink);

}

void test_knot_insertion(){

    int degree = 1;
    int n_knots = 4;

    double * knots  = allocate_vec_double(n_knots);


    knots[0] = 0;
    knots[1] = 0;
    knots[2] = 1;
    knots[3] = 1;

    print_vec(knots,n_knots);
    printf("...allocated knot vector.\n");

    int dim = 3;
    int n_control_points = 2;

    double * control_points  = allocate_vec_double(
        dim*n_control_points);


    control_points[0] = 0;
    control_points[1] = 0;
    control_points[2] = 0;
    control_points[3] = 1;
    control_points[4] = 0;
    control_points[5] = 0;

    print_vec(control_points,dim*n_control_points);

    printf("...allocated control points.\n");

    int n_new_k = 2;

    double * u = allocate_vec_double(n_new_k);

    u [0] = .25;
    u [1] = .5;

    print_vec(u,n_new_k);
    printf("...allocated new knots.\n");



    /* allocate work space t times larger than original number */
    /* of control points and knots */
    double * inc = allocate_vec_double(dim*(n_control_points+n_new_k));
    double * ink = allocate_vec_double(n_knots+n_new_k);

    int ier = bspkntins(degree, control_points, dim, n_control_points,
        knots,n_knots,u,n_new_k,inc,ink);

    //  bspdegelev(degree,control_points,dim,n_control_points,
    //             knots,n_knots,degree_elevation,&nh,inc,ink);
    //
    // printf("nh = %d\n", nh);
    // //
    printf("inc = \n");
    print_vec(inc,dim*(n_control_points+n_new_k));

    printf("ink = \n");
    print_vec(ink,n_knots+n_new_k);
    //
    // printf("...done.\n");
    free(knots);
    free(control_points);
    free(u);
    free(inc);
    free(ink);

}

int main(){

    test_degree_elevation();
    test_knot_insertion();

    return 0;
}
