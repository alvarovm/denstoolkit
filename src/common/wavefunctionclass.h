/*
    wavefunctionclass.h
 
   This class is for easy storaging/handling of a gaussian wave function. 
   The main class, namely gaussWaveFunc, is a class that storages all the information
   of a Gauss-Type wave function. It may contain also other common information contained in 
   *.wfn and *.wfx files, such as some title(s), some keywords/descriptor, atom labels, etc. 
   Below you can find the complete list of variables. For all arrays, you should remember
   that in c, arrays starts with index 0, rather than the standard 1 in fortran.
 
      int nTit          --> The number of titles (1 in wfn, arbitrary in wfx)
      string *title     --> The title(s) used in the *.wfn(*.wfx) file.
      string orbDesc    --> The orbital description (GAUSSIAN in wfn, GTO in wfx)
      int nNuc          --> The number of nuclei
      int nMOr          --> The number of Molecular Orbitals
      int nPri          --> The number of Primitives
      string *atLbl     --> The atom labels (for the wfn, the atom labels will be created as in the
                            following examples)
                              O    1    (CENTRE  1)  ----->   O1
                              C    10   (CENTRE 10)  ----->   C10
      int *primType     --> The primitive type (Type Assignments) 
      int *primCent     --> The primitive center (Centre assignments)
      int *myPN         --> The number of primitives associated with each nuclear center.
      solreal *R           --> The coordinates of the nuclei. It is a 1-dimensional array. It is 
                               this way to facilitate
                               the implementation of cuda code. Later, a function getR(i,j) will be 
                               provided to access easily the j-th coordinate of the i-th nucleus.
                               (Beware the 0 index in c arrays)
      solreal *atCharge    --> The atomic charge
      solreal *primExp     --> The primitive exponent
      solreal *MOCoeff     --> The Molecular Orbital Coefficients. Also a 1D array. The function getCoeff(i,j) is provided
                              to get easy access to the j-th coefficient of the i-th Molecular Orbital. 
                              (Beware the 0 index in c arrays)
      solreal *occN        --> The occupation number of each molecular orbital.
      solreal *MOEner      --> The energy of each Molecular Orbital.
      solreal *cab         --> The matrix whose coefficients are $cab[a][b]=\sum_{i=1}^{nMOr}occN[i]*c_{ai}*c_{bi}$
      solreal *chi         --> Auxiliar array used in some functions
      solreal *gx,*gy,*gz  --> Auxiliar arrays used in some functions
      solreal *hxx,*hyy
           *hzz,*hxy
           *hxz,*hyz    --> Auxiliar arrays used in some functions
      solreal totener      --> The total energy of the system.
      solreal virial       --> The virial ratio (-V/T)
      bool imldd        --> true if the wave function have been loaded (from a wf? file), false otherwise.
 
 The description of the functions is given below, right after every single function. 
 
 ------------------------
 
 Juan Manuel Solano Altamirano
 Adscription at the moment this project is initiated:
 Department of Chemistry, University of Guelph,
 Guelph, Ontario, Canada.
 e-mail: jmsolanoalt@gmail.com
 
 ------------------------
 
 This code is free code; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software 
 Foundation, Inc., 59 Temple Place - Suite 330, 
 Boston, MA  02111-1307, USA.
 
 WWW:  http://www.gnu.org/copyleft/gpl.html
 
 ----------------------
 
 */

#ifndef _SOLWAVEFUNCTIONCLASS_H_
#define _SOLWAVEFUNCTIONCLASS_H_

#ifndef _HAVE_DEF_SOLREAL_TYPE_
#define _HAVE_DEF_SOLREAL_TYPE_
typedef double solreal;
//typedef float solreal;
#endif

#ifndef PARALLELISEDTK
#define PARALLELISEDTK 0
#endif

#define MAXPRIMTYPEDEFINED 20
#define MAXSTEPSIZEBCPSEARCH 0.4
#define MAXSTEPSIZERCPSEARCH 0.35
#define MAXSTEPSIZECCPSEARCH 0.3
#define EPSGRADMAG 1.00000e-14

#ifndef MAXITERATIONBCPSEARCH
#define MAXITERATIONBCPSEARCH 80
#endif
#ifndef MAXITERATIONRCPSEARCH
#define MAXITERATIONRCPSEARCH 100
#endif
#define MAXITERATIONCCPSEARCH 240
#define MAXNUMBEROFPRIMITIVESFORMEMALLOC 10000

#ifndef SIGNF(a)
#define SIGNF(a) ((a)>=0?(1):(-1))
#endif

#include <iostream>
using std::cout;
using std::cin;
using std::endl;
using std::ios;
#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;
#include <cstdlib>
using std::exit;
#include <cmath>
#include <string>
using namespace std;
#include <iomanip>
using std::setprecision;
#include <complex>
#if PARALLELISEDTK
#include <omp.h>
#endif

class gaussWaveFunc
{
public:
   /* *********************************************************************************** */
   gaussWaveFunc(); //Default constructor
   ~gaussWaveFunc(); //Destructor
   /* *********************************************************************************** */
   string *title,orbDesc; /* title */
   int nTit,nNuc,nMOr,nPri;
   string *atLbl;
   int *primType, *primCent,*myPN;
   solreal *R, *atCharge, *primExp, *MOCoeff, *occN, *MOEner;
   solreal *cab,*chi,*gx,*gy,*gz,*hxx,*hyy,*hzz,*hxy,*hxz,*hyz;
   solreal totener,virial;
   bool imldd;
   /* *********************************************************************************** */
   solreal getR(const int nucnum,const int cart);
   /*
      This function returns the value of the cart-th Cartesian coordinate of the nucnum-th nucleus.
      (0 for x, 1 for y and 1 for z coordinates.)
    */
   /* *********************************************************************************** */
   int getAng(int primn,int cartn); //This function returns the angular exponent of the 
                                       //cartn-th cartesian coordinate of the primn-th primivite.
   /* *********************************************************************************** */
   solreal getCoef(const int orbn,const int primn);
   /*
      This function returns the value of the primitive coefficient of the orbn-th orbital
      and primn-th primitive. As any function in c, the indices run from 0 to some number.
      You should be careful about this.
    */
   /* *********************************************************************************** */
   solreal evalDensityArray(solreal x,solreal y, solreal z);
   /* 
      This function returns the value of the density ($\rho$) at the point $\vec{r}=(x,y,z)$ using
      a non optimized algorithm. It remains in this class for having a reference for the
      subsequent optimization of the function evalDensity. Unless you change evalDensity, use the 
      latter to obtain the density (it is the faster function).
    */
   /* *********************************************************************************** */
   void displayAllFieldProperties(solreal x,solreal y,solreal z);
   /* This function calculates and prints to screen all the programmed field properties.
    */
   /* *********************************************************************************** */
   void writeAllFieldProperties(solreal x,solreal y,solreal z,ofstream &ofil);
   /* This function calculates and writes to ofil all the programmed field properties.
    */
   /* *********************************************************************************** */
   solreal evalDensity(solreal x,solreal y,solreal z);
   /*
      This function returns the value of the density ($\rho$) at the point $\vec{r}=(x,y,z)$ using
      the most optimized algorithm.
    */
   /* *********************************************************************************** */
   bool sameMolOrbOccNums(void);
   /*
      This function returns true if all the Molecular Orbital Occupation Numers have the same
      value. It only takes one occupation number to be different in order to this function returning false.
    */
   /* ************************************************************************************** */
   void evalRhoGradRho(solreal &x, solreal &y, solreal &z,solreal &rho, solreal &dx, solreal &dy, solreal &dz);
   /*
      This function returns the value of the density ($\rho$) and the gradient of it 
      ($\nabla\rho=dx\hat{\mathi}+dy\hat{\mathj}+dz\hat{k}$) at the point 
      $\vec{r}=(x,y,z)$ using the most optimized algorithm.
    */
   /* ************************************************************************************** */
   void evalRhoGradRho(solreal &x, solreal &y, solreal &z,solreal &rho, solreal (&grd)[3]);
   /*
      This function is the same as evalRhoGraRho, but using an array for the gradient instead of
      individual components of the gradient. i.e.
      evalRhoGradRho(x,y,z,rho,g[3])=evalRhoGradRho(x,y,z,rho,g[0],g[1],g[2])
    */
   /* ************************************************************************************** */
   bool readFromFileWFN(string inname);
   /*
      This function will load all the values of the wave function (title, orbDesc, etc.) from a 
      file, which name is inname. As the name of the function suggest, the file must be *.wfn The function
      will automatically allocate the corresponding arrays. And since a destructor is given, you
      one does not need to deallocate the arrays of the wave function.
    */
   /* *********************************************************************************** */
   bool readFromFileWFX(string inname);
   /*
      This function is essentially the same as readFromFileWFN, but using a *wfx file. In the future, it is
      expected that both functions differ from each other, since the files wfx are/will be capable of
      containing a larger amount of information.
    */
   /* *********************************************************************************** */
   bool readFromFile(string inname);
   /*
      This function just look for the extension of the inname, if it is wfn(wfx), then calls
      readFromFileWFN(readFromFileWFX)
    */
   /* *********************************************************************************** */
   bool allocAuxArrays(void);
   /*
      This function allocates memory space for the auxiliar arrays the gaussWaveFunction object
      uses for calculating numerical properties (rho, grad(rho), hess(rho), etc.).
    */
   /* *********************************************************************************** */
   void countPrimsPerCenter(void);
   /*
      This function counts the number of primitives associated with each one
      of the nuclear centers.
    */
   /* *********************************************************************************** */
   void calcCab(void);
   /*
      This function calculates the values of the matrix coefficients array $C_{\dot{A}\dot{B}}$
      (see notes ******* for notation details.)
    */
   /* *********************************************************************************** */
   bool writeToFileWFX(string outname);
   /*
      This function will write the wave function into a wfx file which name is outname. 
    */
   /* *********************************************************************************** */
   bool testSupport(void);
   /*
      This function returns true if the *.wfn or *.wfx is supported. At the current version,
      only gaussian wave functions are handled.
    */
   /* *********************************************************************************** */
   solreal evalPrimCases(int &pty,solreal &alp, solreal x, solreal y, solreal z);
   /* *********************************************************************************** */
   solreal evalAngACases(int &pty, solreal x, solreal y, solreal z);
   /*
      This funtion returns the value of the angular part of the primitive, i.e., it returns the
      value of $x^{a_1}y^{a_2}z^{a^3}$, where a1,a2,a3 are the values of the angular exponents.
      The values of a_i are coded in the value of pty (the type of primitive). For correct results,
      x must be the difference between the field point and the primitive center.
    */
   /* *********************************************************************************** */
   void evalDkPrimCases(int pty,solreal alp,solreal x, solreal y, solreal z, solreal &grx, solreal &gry, solreal &grz);
   /*
      This function evaluates the gradient of $phi_{\dot{A}}(x,y,z)$. $\nabla\phi=(grx,gry,grz)$.
    For correct results, x must be the difference between the field point and the primitive center. 
    */
   /* *********************************************************************************** */
   void evalDkAngCases(int &pty,solreal alp,solreal x, solreal y, solreal z, solreal &anx, solreal &any, solreal &anz);
   /*
      Let the field point be $\vec{\xi}$, and a primitive $\phi_{\dot{A}}$, with center 
      $R_{\dot{A}}$, type pty, and primitive exponent alp. The function
      evalDkAngCases returns the value of
      ($\nabla\phi_{\dot{A}}(x,y,z))/exp(-2\alp r^2)$, where x=\xi_x-R^x_{\dot{A}}..., and
      $r^2=x^2+y^2+z^2$. The individual components are anx, any, anz, respectively
    */
   /* *********************************************************************************** */
   void evalDkDlAngCases(int &pty,solreal alp,solreal x,solreal y,solreal z,
                         solreal &axx,solreal &ayy,solreal &azz,solreal &axy,solreal &axz,solreal &ayz);
   /*
    Let the field point be $\vec{\xi}$, and a primitive $\phi_{\dot{A}}$, with center 
    $R_{\dot{A}}$, type pty, and primitive exponent alp. The function
    evalDkDlAngCases returns the value of
    ($\partial_k\partial_l\phi_{\dot{A}}(x,y,z))/exp(-2\alp r^2)$, where x=\xi_x-R^x_{\dot{A}}..., and
    $r^2=x^2+y^2+z^2$. The individual components are axx,ayy,azz,axy,axz,ayz, respectively.
    */
   /* *********************************************************************************** */
   solreal evalLapAngCases(int &pty,solreal alp,solreal x,solreal y,solreal z,solreal rr);
   /*
    Let the field point be $\vec{\xi}$, and a primitive $\phi_{\dot{A}}$, with center 
    $R_{\dot{A}}$, type pty, and primitive exponent alp. The function
    evalLapAngCases returns the value of
    ($\nabla^2\phi_{\dot{A}}(x,y,z))/exp(-2\alp rr)$, where x=\xi_x-R^x_{\dot{A}}..., and
    $rr=x^2+y^2+z^2$.
    */
   /* *********************************************************************************** */
   void evalHessian(solreal x, solreal y, solreal z,solreal &dxx, solreal &dyy, solreal &dzz,
                    solreal &dxy, solreal &dxz, solreal &dyz);
   /*
      This function evaluates the six independent components of the Hessian of $\rho$ 
      (dxx,dyy,dzz,dxy,dxz,dyz) at the point $(x,y,z)$
    */
   /* *********************************************************************************** */
   void evalHessian(solreal x, solreal y, solreal z,solreal (&h)[3][3]);
   /*
      This function evaluates the Hessian of $\rho$ at the point $(x,y,z)$ and store them in the
      array h.
    */
   /* *********************************************************************************** */
   void evalHessian(solreal x, solreal y, solreal z,solreal &dens,solreal (&g)[3],solreal (&h)[3][3]);
   /*
    This function evaluates the gradient and Hessian of $\rho$ at the point 
    $(x,y,z)$ and store them in the arrays g and h.
    */
   /* *********************************************************************************** */
   solreal evalLapRho(solreal x, solreal y, solreal z);
   /*
      This function returns the value of the Laplacian of the density
      ($\nabla^2\rho$) at the point $(x,y,z)$
    */
   /* *********************************************************************************** */
   solreal evalELF(solreal x,solreal y,solreal z);
   /*
      This function returns the value of the Electron Localization Function, EFL 
      ($\eta(x,y,z)=\frac{1}{1+[D(x,y,z)/D_h(x,y,z)]^2}$, where $D(x,y,z)=\frac{1}{2}
        \sum_i|\nabla\phi_i(x,y,z)|^2-\frac{1}{8}\frac{|\nabla\rho(x,y,z)|^2}{\rho(x,y,z)}$,
      and $D_h(x,y,z)=(3/10)(3\phi^2)^{2/3}\rho(x,y,z)^{5/3}$) at the
      point (x,y,z).
    */
   /* *********************************************************************************** */
   solreal evalShannonEntropy(solreal x,solreal y,solreal z);
   /*
      This function returns the value of the Shannon entropy density ($-\rho\ln\rho$) at the
      point (x,y,z).
    */
   solreal evalMomentumShannonEntropy(solreal px,solreal py,solreal pz);
   /*
    This function returns the value of the Shannon entropy density in the momentum space 
    ($-\pi\ln\pi$) at the point (px,py,pz).
    */
   /* *********************************************************************************** */
   solreal evalMagGradRho(solreal x,solreal y,solreal z);
   /*
    This function returns the value of the Magnitude of the Density Gradient ($|\nabla\rho|$) at the
    point (x,y,z).
    */
   /* *********************************************************************************** */
   solreal evalLOL(solreal x,solreal y,solreal z);
//#if PARALLELISEDTK
//   solreal evalLOLNew(solreal x, solreal y,solreal z);
//#endif
   /*
    This function returns the value of the Localized Orbital Locator, LOL, 
    ($\gamma(x,y,z)=\frac{\tau(x,y,z)}{1+\tau(x,y,z)}$, where
     \tau=2D_h(x,y,z)/(\sum_i|\nabla\chi_i|^2)) at the field point (x,y,z)
    */
   /* *********************************************************************************** */
   solreal evalKineticEnergyG(solreal x,solreal y,solreal z);
   /*
    This function returns the value of the Kinetic Energy Density G, defined through
    $G(\vec{x})=\frac{1}{2}\sum_{\dot{A}\sum_{\dot{B}}}C_{\dot{A}\dot{B}}\nabla\phi_{\dot{A}}
     \cdot\nabla\phi_{\dot{B}}$
    */
   /* *********************************************************************************** */
   solreal evalKineticEnergyK(solreal x,solreal y,solreal z);
   /*
    This function returns the value of the Kinetic Energy Density K, defined through
    $K(\vec{x})=-\frac{1}{4}\sum_{\dot{A}\sum_{\dot{B}}}C_{\dot{A}\dot{B}}
    (\phi_{\dot{A}}\nabla^2\phi_{\dot{B}}+\phi_{\dot{B}}\nabla^2\phi_{\dot{A}})$
    */
   /* *********************************************************************************** */
   void seekBondCP(int ii,int jj,solreal &rx,solreal &ry,solreal &rz,solreal &gx,solreal &gy,solreal &gz);
   /*
      This function seeks for a Bond Critical Point. The integers ii and jj are used to set
      $\vec{x}_0=\frac{1}{2}(\vec{R}_i+\vec{R}_j)$. The final values of the search are 
      stored in rx,ry,rz; in addition the values of the gradient at the point rx,ry,rz are 
      saved in gx,gy,gz. If the maximum number of iterations is reached and the critical
      was not found, then rx,ry,rz are the last values obtained from the search (and
      the gradient at such a point).
    */
   /* *********************************************************************************** */
   void getBondCPStep(solreal (&x)[3],solreal (&hh)[3],solreal (&gg)[3]);
   /*
      This function uses the vector $\vec{x}$ as the original position, then it calculates 
      the step hh using the eigen-vector following algorithm. This algorithm is described with 
      detail for  this particular problem in 
         Chem. Phys. Lett. 228 (1994) 160--164, "A robust algorithm to locate automatically
            all types of critical points in the charge density and its Laplacian".
      More details (and ) can be found in references [9-11] of the above article.
      This particular function aims to locate Bond Critical Points.
   */
   /* *********************************************************************************** */
   void seekRingCP(solreal &r1,solreal &r2,solreal &r3,solreal &gx,solreal &gy,solreal &gz);
   /*
    This function seeks for a Ring Critical Point. The vector $\vec{x}_0=(r1,r2,r3)$ is used as
    the starting point for the search. The final values of the search are 
    stored in rx,ry,rz; in addition the values of the gradient at the point rx,ry,rz are 
    saved in gx,gy,gz. If the maximum number of iterations is reached and the critical
    was not found, then rx,ry,rz are the last values obtained from the search (and
    the gradient at such a point).
    */
   /* *********************************************************************************** */
   void getRingCPStep(solreal (&x)[3],solreal (&hh)[3],solreal (&g)[3]);
   /*
    This function uses the vector $\vec{x}$ as the original position, then it calculates 
    the step hh using the eigen-vector following algorithm. This algorithm is described with 
    detail for  this particular problem in 
    Chem. Phys. Lett. 228 (1994) 160--164, "A robust algorithm to locate automatically
    all types of critical points in the charge density and its Laplacian".
    More details (and ) can be found in references [9-11] of the above article.
    This particular function aims to locate Ring Critical Points.
    */
   /* *********************************************************************************** */
   void seekCageCP(solreal &r1,solreal &r2,solreal &r3,solreal &gx,solreal &gy,solreal &gz);
   /*
    This function seeks for a Cage Critical Point. The vector $\vec{x}_0=(r1,r2,r3)$ is used as
    the starting point for the search. The final values of the search are 
    stored in rx,ry,rz; in addition the values of the gradient at the point r1,r2,r3 are 
    saved in gx,gy,gz. If the maximum number of iterations is reached and the critical
    was not found, then rx,ry,rz are the last values obtained from the search (and
    the gradient at such a point).
    */
   /* *********************************************************************************** */
   void getCageCPStep(solreal (&x)[3],solreal (&hh)[3],solreal (&g)[3]);
   /*
    This function uses the vector $\vec{x}$ as the original position, then it calculates 
    the step hh using the eigen-vector following algorithm. This algorithm is described with 
    detail for  this particular problem in 
    Chem. Phys. Lett. 228 (1994) 160--164, "A robust algorithm to locate automatically
    all types of critical points in the charge density and its Laplacian".
    More details (and ) can be found in references [9-11] of the above article.
    This particular function aims to locate Cage Critical Points.
    */
   /* *********************************************************************************** */
   void evald3SingCartA(int &ang,solreal &t,solreal &f,solreal &x,solreal &x2,
                      solreal &d0,solreal &d1,solreal &d2,solreal &d3);
   /* *********************************************************************************** */
   void evald3Ang(int (&a)[3],solreal &alp,solreal (&x)[3],solreal (&x2)[3],
                  solreal (&d0)[3],solreal (&d1)[3],solreal (&d2)[3],solreal (&d3)[3]);
   /* *********************************************************************************** */
   void evald4SingCartA(int &ang,solreal &t,solreal &f,solreal &x,solreal &x2,
                        solreal &d0,solreal &d1,solreal &d2,solreal &d3,solreal &d4);
   /* *********************************************************************************** */
   void evald4Ang(int (&a)[3],solreal &alp,solreal (&x)[3],solreal (&x2)[3],
                  solreal (&d0)[3],solreal (&d1)[3],solreal (&d2)[3],solreal (&d3)[3],
                  solreal (&d4)[3]);
   /* *********************************************************************************** */
   solreal evalLapRhoUsingd2(solreal x,solreal y,solreal z);
   /* *********************************************************************************** */
   void evalDiDjDkChi(int &pty,solreal &alp,solreal x,solreal y,solreal z,
                      solreal (&dlm)[3][3],solreal (&dijk)[3][3][3]);
   /* void evalDkDlAngCases(int &pty,solreal alp,solreal x,solreal y,solreal z,
    solreal &axx,solreal &ayy,solreal &azz,solreal &axy,solreal &axz,solreal &ayz); */
   /* *********************************************************************************** */
   void evalHessLOL(solreal x, solreal y, solreal z, solreal &dens,solreal &keG, solreal &lol,
                                   solreal &ddx, solreal &ddy, solreal &ddz,
                                   solreal &dxx, solreal &dyy, solreal &dzz,
                                   solreal &dxy, solreal &dxz, solreal &dyz);
   /* *********************************************************************************** */
   void evalHessLOL(solreal (&x)[3],solreal &lol,solreal (&glol)[3],solreal (&hlol)[3][3]);
   /* ************************************************************************************ */
   void evalFTASingCartA(int &ang,solreal &a,solreal &ooa,solreal &osra,
                         solreal &px,solreal &px2,solreal &Rx,
                         solreal &RePhi,solreal &ImPhi);
   /*
     This function evaluates the "angular" part of the Fourier transform of every primitive.
       ang is the angular exponent for the component, ooa is 1/alpha, osra=1/sqrt(a),
       Rx is the component of the primitive center, px is the component of the momentum, and
       px2=px*px.
    */
   /* ************************************************************************************ */
   void evalFTAng(int (&a)[3],solreal &alp,solreal &ooalp,solreal (&p)[3],solreal (&p2)[3],
                  solreal (&Rx)[3],complex<solreal> &pang);
   /* ************************************************************************************ */
   void evalFTChi(int &pty,solreal &alp,solreal (&Rx)[3],solreal px,solreal py,solreal pz,
                      complex<solreal> &phi);
   /* ************************************************************************************ */
   solreal evalFTDensity(solreal px,solreal py,solreal pz);
   /* ************************************************************************************ */
   solreal evalFTKineticEnergy(solreal px,solreal py,solreal pz);
   /* ************************************************************************************ */
   solreal evalDensityMatrix1(solreal x,solreal y,solreal z,solreal xp,solreal yp,solreal zp);
   /* ************************************************************************************ */
   solreal evalMagGradLOL(solreal x,solreal y,solreal z);
   /* ************************************************************************************ */
   solreal evalMolElecPot(solreal x,solreal y,solreal z);
   /* ************************************************************************************ */
   void evalHermiteCoefs(int (&aia)[3],int (&aib)[3],solreal &alpab,
                         solreal (&ra)[3],solreal (&rb)[3],
                         solreal (&rp)[3],
                         int (&maxl)[3],solreal (&Eijl)[3][7]);//tested on Dec 28th, 2013
   /* ************************************************************************************ */
   void evalRlmnIntegs(const int (&lmn)[3],const solreal &alpp,const solreal (&cp)[3],
                       solreal (&Rlmn)[7][7][7]);
   /* ************************************************************************************ */
   solreal evalVAB(solreal (&xx)[3],int (&aa)[3],int (&ab)[3],solreal &alpa,solreal &alpb,
                          solreal (&ra)[3],solreal (&rb)[3]);
   /* ************************************************************************************ */
   solreal evalOverlapIntegralAB(int (&aa)[3],int (&ab)[3],solreal &alpa,solreal &alpb,
                         solreal (&ra)[3],solreal (&rb)[3]);
   /* ************************************************************************************ */
   solreal integralRho(void);
   /* ************************************************************************************ */
   solreal totalNuclearCharge(void);
   /* ************************************************************************************ */
   /* ************************************************************************************ */
private:
   static int prTy[MAXPRIMTYPEDEFINED*3];
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
   /* ************************************************************************************ */
};
/* *********************************************************************************** */
/* *********************************************************************************** */
#endif//_SOLWAVEFUNCTIONCLASS_H_

