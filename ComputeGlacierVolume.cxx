#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
#include "vtkXMLPUnstructuredGridReader.h"
#include <vtkUnstructuredGrid.h>
#include <vtkPoints.h>
#include <vtkPointSet.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkTriangle.h>
#include <vtkType.h>
#include "vtkConfigure.h"
#include "vtk_kwiml.h"
#include <vector>
#include <netcdfcpp.h>


using namespace std;

int main(int argc, char* argv[])
{
	string run;
	double VolTotal, VolCell, area, H1, H2, H3, HMean;
	if (argc < 2)
	{
		cout << "ERROR in argas" << endl;
		return -1;
	}
	else
	{
		run = argv[1];
	}

	vtkSmartPointer<vtkXMLPUnstructuredGridReader> source = vtkSmartPointer<vtkXMLPUnstructuredGridReader>::New();
	source->SetFileName(run.c_str());
	source->Update();
	vtkSmartPointer<vtkUnstructuredGrid> output = source->GetOutput();
	vtkSmartPointer<vtkPointData> pointsData = output->GetPointData();

	vtkSmartPointer<vtkDataArray> arrayH = pointsData->GetScalars("h");
	vtkSmartPointer<vtkDoubleArray> arrayDoubleH = vtkDoubleArray::SafeDownCast(arrayH);
	
	VolTotal = 0 ;
	for (vtkIdType cellId = 0; cellId < output->GetNumberOfCells(); cellId++)
	{
		vtkSmartPointer<vtkCell> cell = output->GetCell(cellId);
		if ( cell->GetCellType() != 5 ) continue; // cell is not a triangle
		vtkSmartPointer<vtkTriangle> triangle = vtkTriangle::SafeDownCast(cell);
		area = triangle->ComputeArea();	
		H1 = arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(0));
                H2 = arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(1));
                H3 = arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(2));
		HMean = ( H1 + H2 + H3 )/2;
		VolCell = HMean * area ;
		VolTotal = VolTotal + VolCell;
	}

	cout << "Vol Total " << VolTotal << endl;
}
