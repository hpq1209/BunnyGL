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

#include "CVtkLocatorData.h"

CVtkLocatorData::CVtkLocatorData(vtkLocator* widget)
:m_vtkLocator(widget)
{

}

CVtkLocatorData::~CVtkLocatorData()
{

}

void CVtkLocatorData::setLocator(vtkLocator* widget)
{
    m_vtkLocator = widget;
}


void* CVtkLocatorData::dataPointer()
{
    return m_vtkLocator;
}

QString CVtkLocatorData::dataTypeName()
{
    return "vtkLocator";
}

GCF_BEGIN_QUERY_TABLE(CVtkLocatorData)
    GCF_IMPLEMENTS(IVtkLocatorIOData)
    GCF_IMPLEMENTS(IVtkObjectIOData)
GCF_END_QUERY_TABLE(CVtkLocatorData)

vtkLocator* CVtkLocatorData::getVtkLocator()
{
    return m_vtkLocator;
}

vtkObject* CVtkLocatorData::getVtkObject()
{
    return vtkObject::SafeDownCast( m_vtkLocator );
}

