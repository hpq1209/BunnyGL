/****************************************************************************
**
** Copyright (C) VCreate Logic Pvt Ltd
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Pvt Ltd.
**
** Details of those terms are listed in licence.txt included as
** part of the distribution package of this file. This file may not
** be distributed without including the licence.txt file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef CVTKVOLUMETEXTUREMAPPER3D_VIS_NODE_H
#define CVTKVOLUMETEXTUREMAPPER3D_VIS_NODE_H

#include "UserDefinedDataTypes.h"
#include "CVtkVolumeMapper.h"

#ifdef ENABLE_ADVANCED_PROPERTIES
#include "IEditableProperties.h"
#endif

struct CVtkVolumeTextureMapper3DData;
class CVtkVolumeTextureMapper3D : public CVtkVolumeMapper
#ifdef ENABLE_ADVANCED_PROPERTIES
                       , virtual public IEditableProperties
                       , virtual public IEditablePropertiesExt
#endif
{
    Q_OBJECT
#ifdef ENABLE_ADVANCED_PROPERTIES
    Q_INTERFACES(IEditableProperties IEditablePropertiesExt)
#endif
	DECLARE_VIS_NODE

    Q_ENUMS(RenderMethod)
    Q_PROPERTY(RenderMethod PreferredRenderMethod READ preferredRenderMethod WRITE setPreferredRenderMethod)

public:
    CVtkVolumeTextureMapper3D();
    ~CVtkVolumeTextureMapper3D();

	Q_INVOKABLE QList<QVariant> volumeDimensions() const;
    Q_INVOKABLE QList<QVariant> volumeSpacing() const;
    Q_INVOKABLE int numberOfPolygons() const;
    Q_INVOKABLE float actualSampleDistance() const;

    enum RenderMethod
    {
        FragmentProgram,
        NVIDIA,
        ATI,
        None
    };
    void setPreferredRenderMethod(RenderMethod meth);
    RenderMethod preferredRenderMethod() const;

protected:
    bool hasInput(IVisSystemNodeConnectionPath* path);
    bool setInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool removeInput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* inputData);
    bool fetchOutput(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData** outputData);
    bool outputDerefed(IVisSystemNodeConnectionPath* path, IVisSystemNodeIOData* outputData);

#ifdef ENABLE_ADVANCED_PROPERTIES
public:
    // IEditableProperties implementation
    int propertyCount();
    QString propertyName(int index);
    QString propertyGroup(int index);
    QVariant propertyValue(int index);
    void setPropertyValue(int index, QVariant value);
    bool hasEditor(int index);
    QWidget* createEditor(int index);
    void setEditorValue(int index, QWidget* widget, QVariant value);
    QVariant editorValue(int index, QWidget* widget);
    void connectEditor(int index, QWidget* widget, QObject* receiver, const char* member);
    QString propertyValueString(int index);
    QVariant propertyValueFromString(int index, QString valueStr);

signals:
    void propertyConfigurationChanged();

public:
    // IEditablePropertiesExt implementation
    bool canLoadSaveProperty(int index);
#endif

protected:
    void setVtkObject(vtkObject* obj);

private:
    CVtkVolumeTextureMapper3DData* d;
};

#endif
