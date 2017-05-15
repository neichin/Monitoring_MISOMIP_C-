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
	double AreaTotal, area;
	int GL1, GL2, GL3;
	bool Grounded;

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

	vtkSmartPointer<vtkDataArray> arrayH = pointsData->GetScalars("groundedmask");
	vtkSmartPointer<vtkDoubleArray> arrayDoubleH = vtkDoubleArray::SafeDownCast(arrayH);
	
	AreaTotal = 0 ;
	for (vtkIdType cellId = 0; cellId < output->GetNumberOfCells(); cellId++)
	{
		vtkSmartPointer<vtkCell> cell = output->GetCell(cellId);
		if ( cell->GetCellType() != 5 ) continue; // cell is not a triangle
		vtkSmartPointer<vtkTriangle> triangle = vtkTriangle::SafeDownCast(cell);
		area = triangle->ComputeArea();	
		GL1 = (int)arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(0));
                GL2 = (int)arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(1));
                GL3 = (int)arrayDoubleH->GetValue(triangle->GetPointIds()->GetId(2));
		Grounded = ( GL1 + GL2 + GL3 ) == 3;
		if (Grounded)
		{
			AreaTotal = AreaTotal + area ;
		}
	}

	cout << "Area Total " << AreaTotal << endl;
}
