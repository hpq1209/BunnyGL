/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore. Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "CVtkHierarchicalDataSetGeometryFilter.h"

DEFINE_VTK_OBJECT(CVtkHierarchicalDataSetGeometryFilter, CVtkPolyDataFilter, vtkHierarchicalDataSetGeometryFilter)
{
    pDesc->setNodeClassName("vtkHierarchicalDataSetGeometryFilter");
    pDesc->setNodeClassDescription("Extract geometry from hierarchical data");

}

CVtkHierarchicalDataSetGeometryFilter::CVtkHierarchicalDataSetGeometryFilter() : m_vtkHierarchicalDataSetGeometryFilter(0)
{
    CVtkHierarchicalDataSetGeometryFilter::InitializeObjectDesc();
    setVtkObject(vtkHierarchicalDataSetGeometryFilter::New());
}

CVtkHierarchicalDataSetGeometryFilter::~CVtkHierarchicalDataSetGeometryFilter()
{

}
