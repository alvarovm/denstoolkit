/*

   crtflnms.cpp
   
   This file contains the implementation of the definitions to modify or to create names for the 
   different files used to record the information extracted from the program.

   ------------------------

   Juan Manuel Solano Altamirano
   Adscription at the moment this project is initiated:
   Centro de Investigaciones y Estudios Avanzados del 
   Instituto Politécnico Nacional, 
   Unidad Monterrey, Mexico.
   2011
   e-mail: jmsolanoalt@gmail.com
   
   Adscription at the moment the particular implementation for this program is started:
   University of Guelph,
   Guelph, Ontario, Canada.
   May 2013
	----------------------
*/
#ifndef _CRTFLNMS_CPP
#define _CRTFLNMS_CPP
#include "crtflnms.h"
#include "../common/solfileutils.h"
#include "../common/solscrutils.h"

void mkFileNames(char ** (&argv), optFlags &opts, string &i_fn,string &o_fn,string &p_fn,
                 string &n_fn,string &c_fn,ScalarFieldType &cpt)
{
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   /*
      If you need more names to be created by this function, you need to add the new
      string in the arguments list here and in the corresponding header file.
    */
   //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   i_fn=string(argv[1]);
   size_t pos;
   //string sl="wfn";
   //pos=i_fn.find(sl);
   if (!((i_fn.find("wfn")!=string::npos)||
       (i_fn.find("WFN")!=string::npos)||
       (i_fn.find("wfx")!=string::npos)||
       (i_fn.find("WFX")!=string::npos))) {
      setScrRedBoldFont();
      cout << "\nError: the file " << i_fn << " is not a valid wave function file." << endl << endl;
      setScrNormalFont();
      exit(1);
   }
   o_fn=i_fn.substr(0,(i_fn.length()-3));
   p_fn=n_fn=c_fn=o_fn;
   o_fn.append("log");
   p_fn.append("pov");
   n_fn.append("png");
   c_fn.append("cpx");
   pos=o_fn.find_last_of('.');
   if (pos!=string::npos) {
      string plbl;
      switch (cpt) {
         case LOLD:
            plbl=string("LOLCP");
            break;
         case DENS:
            plbl=string("RhoCP");
            break;
         default:
            displayErrorMessage("Not implemented/unknown field type.");
            break;
      }
      o_fn.insert(pos,plbl);
      p_fn.insert(pos,plbl);
      n_fn.insert(pos,plbl);
      c_fn.insert(pos,plbl);
   }
   if (opts.outfname) {
      o_fn=argv[opts.outfname];
      p_fn=n_fn=c_fn=o_fn;
      o_fn.append(".log");
      p_fn.append(".pov");
      n_fn.append(".png");
      c_fn.append(".cpx");
   }
   return;
}

void mkDatMatFileNames(string &lgfn,string &acfn,string &cpfn,string &bpfn)
{
   acfn=lgfn.substr(0,(lgfn.length()-4));
   cpfn=acfn;
   bpfn=acfn;
   acfn.append("-ATCrds.dat");
   cpfn.append("-CPCrds.dat");
   bpfn.append("-BPCrds.dat");
   return;
}

#endif //_CRTFLNMS_CPP
