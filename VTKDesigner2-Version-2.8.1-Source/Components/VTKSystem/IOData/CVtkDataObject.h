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

#ifndef C_VTK_DATA_OBJECT_H
#define C_VTK_DATA_OBJECT_H

#include "IVtkDataObjectIOData.h"
#include "IVtkObjectIOData.h"

class CVtkDataObject : virtual public IVtkDataObjectIOData,
                       virtual public IVtkObjectIOData
{
public:
    CVtkDataObject(vtkDataObject* dataObject=0);
    ~CVtkDataObject();

    void setDataObject(vtkDataObject* dataObject);

    // IQueryable implementation
    bool queryInterface(QString ifaceName, void** ifacePtr);
    bool destroySelf();

    // IVisSystemNodeIOData implementation
    void* dataPointer();
    QString dataTypeName();

    // IVtkDataObjectIOData implementation
    vtkDataObject* getVtkDataObject();

    // IVtkObjectIOData implementation
    vtkObject* getVtkObject();

private:
    vtkDataObject* m_vtkDataObject;
};

#endif

