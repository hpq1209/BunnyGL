/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
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

#ifndef ABSTRACT_COMPONENT_H
#define ABSTRACT_COMPONENT_H

#include "Common.h"
#include "ComponentGui.h"
#include "DataStore.h"
#include "ObjectIterator.h"

#include <QStringList>
#include <QPair>

class QAction;
class QWidget;
class QMenu;
class QToolBar;
class QStatusBar;
class QMainWindow;
class QWidgetAction;
class IComponentHostWindow;
class ComponentGuiXmlParser;

namespace GCF
{
void GCF_EXPORT_METHOD initialize();
void GCF_EXPORT_METHOD finalize();

class ComponentSettings;

struct AbstractComponentData;
class GCF_EXPORT_INTERFACE AbstractComponent : public QObject
{
    Q_OBJECT

public: // static methods
    static void setGuiXmlFileDirectory(const QString& absDirPath);
    static QString guiXmlFileDirectory();
    static void setMainWindow(QMainWindow* mainWindow);
    static QMainWindow* mainWindow();
    static void setComponentHostWindow(IComponentHostWindow* hostWindow);
    static IComponentHostWindow* componentHostWindow();
    static QWidget* componentHostWidget();
    static void setMenuStripEnabled(bool val);
    static bool isMenuStripEnabled();

public:
    virtual ~AbstractComponent();

    QString componentName() const;

    ComponentGui componentGui() const;
    QString guiXmlFileName() const;

    bool isActive() const;

    QList<AbstractComponent*> dependentComponents() const;
    QList<AbstractComponent*> dependingComponents() const;

    DataStore componentData() const;

public slots:
    void initialize();
    void finalize();
    void setActive(bool val);

protected: // service methods
    AbstractComponent();
    void setComponentName(const QString & name);
    void setGuiXmlFileName(const QString & name);
    static void addDependency(AbstractComponent* comp1, AbstractComponent* comp2);
    static void removeDependency(AbstractComponent* comp1, AbstractComponent* comp2);

    QStringList componentNames() const;
    AbstractComponent* findComponent(const QString& componentName) const;
    QAction* findAction(const QString& completeName, AbstractComponent** component=0) const;
    QObject* findObject(const QString& completeName, AbstractComponent** component=0) const;
    QWidget* findWidget(const QString& completeName, AbstractComponent** component=0) const;
    QMenu* findMenu(const QString& completeName, AbstractComponent** component=0) const;
    QToolBar* findToolBar(const QString& completeName, AbstractComponent** component=0) const;
    QObject* findObject(const QString& ifaceName, QString& completeName, AbstractComponent** component=0) const;
    QList<QObject*> findObjects(const QString& ifaceName, QStringList& completeNames, QList<AbstractComponent*>* components=0) const;

protected: // virtual methods
    virtual QAction* fetchAction(const QString& completeName) const;
    virtual QObject* fetchObject(const QString& completeName) const;
    virtual QWidget* fetchWidget(const QString& completeName) const;
    virtual QMenu* fetchMenu(const QString& completeName) const;
    virtual QToolBar* fetchToolBar(const QString& completeName) const;
    virtual void addChildObject(QObject* parent, QObject* child, const QString& hint=QString());
    virtual void removeChildObject(QObject* parent, QObject* child, const QString& hint=QString());
    virtual void addChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    virtual void removeChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    virtual void showChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    virtual void hideChildWidget(QWidget* parent, QWidget* child, const QString& hint=QString());
    virtual void componentCreated(AbstractComponent* comp);
    virtual void componentDestroyed(AbstractComponent* comp);
    virtual void creationAnnounced();
    virtual void destructionAnnounced();
    virtual void registerDependentComponent(AbstractComponent* comp);
    virtual void unregisterDependentComponent(AbstractComponent* comp);
    virtual void registerDependingComponent(AbstractComponent* comp);
    virtual void unregisterDependingComponent(AbstractComponent* comp);
    virtual void initializeComponent() = 0;
    virtual void finalizeComponent() = 0;
    virtual void aboutToActivate();
    virtual void justActivated();
    virtual void aboutToDeactivate();
    virtual void justDeactivated();
    virtual void aboutToFinalize();

private: // Do not use these methods
    void addDependentComponent(AbstractComponent* comp);
    void removeDependentComponent(AbstractComponent* comp);
    void addDependingComponent(AbstractComponent* comp);
    void removeDependingComponent(AbstractComponent* comp);

private slots:
    void objectDestroyed(QObject* obj);

private: // data members
    friend class ComponentGui;
    friend class ComponentGuiNode;
    friend class ComponentManager;
    friend class ComponentFactory;
    friend class AbstractNodeInitializer;
    friend class AppAccessPoint;
    AbstractComponentData* d;

public:
    // Component contents cloning
    virtual QObject* createObject(const QString& completeName) const;
    virtual QWidget* createWidget(const QString& completeName) const;
    void operator delete(void* mem);
    QList<QObject*> findObjects(const QString& ifaceName, QList< QPair<QString,AbstractComponent*> >* objectInfoList ) const;

protected:
    // For loading and storing settings
    GCF::ComponentSettings& settings();

public:
    const GCF::ComponentSettings& constSettings() const;
    friend class ComponentGuiXmlParser;
};

inline QObject* findObject(const QString& completeName)
{
    QString compName = completeName.split(".").first();
    ObjectIterator objIt(compName);
    QObject* obj = objIt.firstObject();
    while(obj)
    {
        if(objIt.currentObjectName() == completeName)
            return obj;

        obj = objIt.nextObject();
    }

    return 0;
}

template <typename T>
inline T findObject(QString* name=0, GCF::AbstractComponent** comp=0)
{
    ObjectIterator objIt;
    QObject* obj = objIt.firstObject();
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(name)
                *name = objIt.currentObjectName();
            if(comp)
                *comp = objIt.currentObjectComponent();
            return ret;
        }

        obj = objIt.nextObject();
    }

    return 0;
}

template <typename T>
inline QList<T> findObjects(QStringList* names=0, QList<GCF::AbstractComponent*>* comps=0)
{
    ObjectIterator objIt;
    QObject* obj = objIt.firstObject();
    QList<T> retList;
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(names)
                *names << objIt.currentObjectName();
            if(comps)
                *comps << objIt.currentObjectComponent();

            retList.append(ret);
        }

        obj = objIt.nextObject();
    }

    return retList;
}

template <typename T>
inline T findObject(GCF::AbstractComponent* component, QString* name=0)
{
    if(!component)
        return T(0);

    ObjectIterator objIt(component);
    QObject* obj = objIt.firstObject();
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(name)
                *name = objIt.currentObjectName();

            return ret;
        }

        obj = objIt.nextObject();
    }

    return 0;
}

template <typename T>
inline QList<T> findObjects(GCF::AbstractComponent* component, QStringList* names=0)
{
    if(!component)
        return QList<T>();

    ObjectIterator objIt(component);
    QObject* obj = objIt.firstObject();
    QList<T> retList;
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(names)
                *names << objIt.currentObjectName();

            retList.append(ret);
        }

        obj = objIt.nextObject();
    }

    return retList;
}

template <typename T>
inline T findObject(const QString& componentName, QString* name=0)
{
    ObjectIterator objIt(componentName);
    if(!objIt.iteratingOnComponent())
        return (T)0;

    QObject* obj = objIt.firstObject();
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(name)
                *name = objIt.currentObjectName();

            return ret;
        }

        obj = objIt.nextObject();
    }

    return 0;
}

template <typename T>
inline QList<T> findObjects(const QString& componentName, QStringList* names=0)
{
    ObjectIterator objIt(componentName);
    if(!objIt.iteratingOnComponent())
        return QList<T>();

    QObject* obj = objIt.firstObject();
    QList<T> retList;
    while(obj)
    {
        T ret = qobject_cast<T>(obj);
        if(ret)
        {
            if(names)
                *names << objIt.currentObjectName();

            retList.append(ret);
        }

        obj = objIt.nextObject();
    }

    return retList;
}
}

Q_DECLARE_METATYPE(GCF::AbstractComponent*)
// We already have QDeclarativeItem doing this
#ifdef VCL_MAKE_DLL
Q_DECLARE_METATYPE(QAction*)
#endif
Q_DECLARE_METATYPE(QWidget*)
Q_DECLARE_METATYPE(QMenu*)
Q_DECLARE_METATYPE(QToolBar*)
Q_DECLARE_METATYPE(QStatusBar*)
Q_DECLARE_METATYPE(QWidgetAction*)

#define GCF_DECLARE_COMPONENT(ClassName) public: static GCF::AbstractComponent* CreateInstance(); private:
#define GCF_DEFINE_COMPONENT(ClassName) \
    GCF::AbstractComponent* ClassName::CreateInstance() \
{ \
    static ClassName* ret = new ClassName; \
    return ret; \
    }

#define GCF_DECLARE_STD_COMPONENT(ClassName) public: static GCF::AbstractComponent* CreateInstance(); private:
#define GCF_DEFINE_STD_COMPONENT(ClassName) \
    GCF::AbstractComponent* GCF::Components::ClassName::CreateInstance() \
{ \
    static ClassName* ret = new ClassName; \
    return ret; \
    }

#define GCF_EXPORT_COMPONENT GCF_EXPORT_CLASS
#define GCF_CREATE_COMPONENT(ClassName) (ClassName*)(GCF::ComponentFactory::instance().createComponent(#ClassName))
#define GCF_CHECK_COMPONENT_PTR(ptr) GCF::ComponentFactory::instance().components().contains(ptr)
typedef GCF::AbstractComponent* (*CreateComponentFunction)();
#define GCF_REGISTER_COMPONENT(ClassName) GCF::ComponentFactory::instance().registerComponentInfo(#ClassName, ClassName::CreateInstance, &ClassName::staticMetaObject);

#include "ComponentFactory.h"
#include "IComponentPlugin.h"

#endif
