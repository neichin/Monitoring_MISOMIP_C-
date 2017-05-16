#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <netcdfcpp.h>


using namespace std;

static const int NC_ERR = 2;
static const NcToken xDimNAME = "x";
static const NcToken yDimNAME = "y";

static const NcToken xVarNAME = "x";
static const NcToken yVarNAME = "y";
static const NcToken tVarNAME = "time_counter";

int main(int argc, char* argv[])
{
	string run, mesh;
	double AreaTotal, area;
	int GL1, GL2, GL3;
	bool Grounded;

	if (argc < 3)
	{
		cout << "ERROR in argas" << endl;
		return -1;
	}
	else
	{

		run = argv[1];
                mesh = argv[2];
	}

  /////////////////////////////////////////
  //  //OPEN AND READ NETCDF GRID
	NcError err(NcError::silent_nonfatal);
	NcFile dataFile(run.c_str(), NcFile::ReadOnly);
  
        NcFile dataFileMesh(mesh.c_str(), NcFile::ReadOnly);
 
        NcVar *melt, *x, *y;
        //GET DIMENSIONS
        NcDim *xdim = dataFile.get_dim(xVarNAME);
        NcDim *ydim = dataFile.get_dim(yVarNAME);
        NcDim *tdim = dataFile.get_dim(tVarNAME);
        long nx = xdim->size();
        long ny = ydim->size();
        long nt = tdim->size();

	x = dataFileMesh.get_var("e1t");
	double xMetric[1][ny][nx];
	x->get(&xMetric[0][0][0],1,ny,nx);


	NcDim *dim1 = x->get_dim(1);

        y = dataFileMesh.get_var("e2t");
        double yMetric[1][ny][nx];
        y->get(&yMetric[0][0][0],1,ny,nx);

	melt = dataFile.get_var("fwfisf");
  	float MeltData[nt][ny][nx];
  	melt->get(&MeltData[0][0][0],nt,ny,nx);

	double MeltInteg = 0;
	double MeltAv = 0;
	double AreaMelt = 0;

	for (int i=0;i<nt;i++)
	{
	 MeltInteg = 0.0;
	 AreaMelt = 0.0;

	 for (int j=0;j<ny;j++)
         {
	  for (int k=0;k<nx;k++)
          {
		if (isnan(MeltData[i][j][k])) {
			cout << "Nan " << i << " " << j << " " << k << endl;
			continue;
		}
                if (isnan(yMetric[0][j][k])) {
                        cout << "Nan x " << i << " " << j << " " << k << endl;
                        continue;
                }
                if (isnan(xMetric[0][j][k])) {
                        cout << "Nan y " << i << " " << j << " " << k <<  endl;
                        continue;
                }
		MeltInteg = MeltInteg + MeltData[i][j][k]*yMetric[0][j][k]*xMetric[0][j][k]/nt ;
		if (MeltData[i][j][k] > 0.0)
		{
			AreaMelt = AreaMelt + yMetric[0][j][k]*xMetric[0][j][k];
		}

		//MeltInteg = MeltInteg + ( MeltData[i][j][k]*yMetric[0][j][k]*xMetric[0][j][k]) / nt;
	  } //end i
	 } //end j

	 MeltAv = MeltAv + MeltInteg/AreaMelt;
	}//end i

	MeltAv = MeltAv / nt / 1000.0 * ( 365.0 * 24.0 * 3600.0 ) ;

        cout << "Averaged Melt Rate (m/a): " << MeltAv << endl;
}

