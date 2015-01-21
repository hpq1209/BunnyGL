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
#ifndef C_VTK_BOX_H
#define C_VTK_BOX_H

#include "vtkBox.h"
#include "CVtkImplicitFunction.h"
#include "UserDefinedDataTypes.h"

class CVtkBox : public CVtkImplicitFunction
{
    Q_OBJECT
    Q_PROPERTY(Point3D XMin READ xMin WRITE setXMin)
    Q_PROPERTY(Point3D XMax READ xMax WRITE setXMax)
    Q_PROPERTY(BoundingBox3D Bounds READ bounds WRITE setBounds)
    DECLARE_VTK_OBJECT

public:

    CVtkBox();
    ~CVtkBox();

    Point3D xMin() const;
    void setXMin(Point3D val);

    Point3D xMax() const;
    void setXMax(Point3D val);

    BoundingBox3D bounds() const;
    void setBounds(BoundingBox3D val);


private:
    vtkBox *m_vtkBox;
};
#endif//C_VTK_BOX_H