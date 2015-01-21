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

#include "PropertyEditor.h"
#include "Property.h"
#include "IEnumValueEditorCreator.h"
#include "IEditableProperties.h"
#include "IValueEditorFactory.h"
#include "IValueEditorCreator.h"
#include "IDefaultProperty.h"
#include "IPropertyEditorNotifications.h"
#include "IObjectPropertyVisibility.h"
#include "PropertyEditorComponent.h"

#include <QPainter>
#include <QPointer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QHeaderView>
#include <QMetaObject>
#include <QMetaProperty>
#include <QMetaClassInfo>
#include <QTextDocument>
#include <QAbstractItemDelegate>
#include <QTreeWidgetItemIterator>
#include <QAbstractTextDocumentLayout>

#include <QStyle>
#include <QStyleOptionButton>
#include <QApplication>
#include <QtDebug>

namespace GCF
{
    namespace Components
    {
    	class PropertyEditorDelegate : public QAbstractItemDelegate
    	{
    	public:
    	    PropertyEditorDelegate(QAbstractItemDelegate* old, QObject* parent, PropertyEditorData* d);
    	    ~PropertyEditorDelegate();

    	    void setPropertyEditor(PropertyEditor* propEditor) { mPropertyEditor = propEditor; }
    	    PropertyEditor* propertyEditor() const { return mPropertyEditor; }

    	    void paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& index) const;
    	    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

    	    QWidget* createEditor(QWidget * parent, const QStyleOptionViewItem& opt, const QModelIndex& index) const;
    	    void setEditorData(QWidget * editor, const QModelIndex &index) const;
    	    bool editorEvent(QEvent *event, QAbstractItemModel* model, const QStyleOptionViewItem & opt, const QModelIndex& index);
    	    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    	    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;

    	private:
    	    QAbstractItemDelegate* mOldDelegate;
    	    PropertyEditor* mPropertyEditor;
    	    mutable int mOffset;
    	    PropertyEditorData* d;
    	};
    }
}

namespace GCF { namespace Components {
struct PropertyEditorData
{
    GCF::Components::PropertyEditor* propertyEditor;
    QObject* object;
    QObjectList objectList;
    bool recursive;
    bool readOnly;

    QList<GCF::Components::Property*> propertyList;
    IValueEditorFactory* valueEditorFactory;
    bool inCurrentItemChanged;

    // Keep track of the currently edited property
    GCF::Components::Property* currentProperty;
    QPointer<QWidget> currentEditor;

    QTreeWidgetItem* groupItem(QString name, QTreeWidget* treeWidget);
    QTreeWidgetItem* nextItem(QTreeWidgetItem* item);
    QTreeWidgetItem* previousItem(QTreeWidgetItem* item);

    // IDefaultPropertyManager structures
    QMap<QObject*, IDefaultProperty*> defaultPropertyMap;
    QList<QTreeWidgetItem*> noDefaultsList;

    // Get skip class names list
    QStringList getSkipClassList(QObject* object);
    int indexOfEditableProperty(IEditableProperties* editableProps, const QString& propName);
};
} } // namespaces

GCF::Components::PropertyEditor::PropertyEditor(QWidget* parent)
:QTreeWidget(parent)
{
    d = new GCF::Components::PropertyEditorData;
    d->propertyEditor = this;
    d->valueEditorFactory = 0;
    d->inCurrentItemChanged = false;
    d->object = 0;
    d->recursive = false;
    d->currentProperty = 0;
    d->currentEditor = 0;
    d->readOnly = false;

    setColumnCount(2);
    setHeaderLabels(QStringList() << qApp->translate("GCF", "Name") << qApp->translate("GCF", "Value"));

    GCF::Components::PropertyEditorDelegate* twDel = new GCF::Components::PropertyEditorDelegate(itemDelegate(), this, d);
    twDel->setPropertyEditor(this);
    setItemDelegate(twDel);
    setRootIsDecorated(false);
    setSortingEnabled(false);
    setAlternatingRowColors(true);
    setIndentation(0);

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
            this, SLOT(on_currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

    qApp->installEventFilter(this);
}

GCF::Components::PropertyEditor::~PropertyEditor()
{
    QList<IDefaultProperty*> defProps = d->defaultPropertyMap.values();
    for(int i=0; i<defProps.count(); i++)
        defProps[i]->destroySelf();

    delete d;
}

void GCF::Components::PropertyEditor::setReadOnly(bool val)
{
    if(d->readOnly == val)
        return;

    d->readOnly = val;

    if(!d->readOnly)
    {
        if(!d->currentEditor.isNull())
            d->currentEditor->deleteLater();
    }
}

bool GCF::Components::PropertyEditor::isReadOnly() const
{
    return d->readOnly;
}

QObject* GCF::Components::PropertyEditor::containerObject()
{
    return this;
}

void GCF::Components::PropertyEditor::setObject(QObject* object, bool recursive)
{
    GCF::HourGlass hourGlass;

    if( d->object == object )
        return;

    clear();
    qDeleteAll(d->propertyList);
    d->propertyList.clear();
    for(int i=0; i<d->objectList.count(); i++)
        disconnect(d->objectList[i], 0, this, 0);
    d->objectList.clear();
    d->object = object;
    d->recursive = recursive;
    d->noDefaultsList.clear();

    if(!object)
        return;

    QObjectList objects;
    objects.push_back(object);
    if(recursive)
        objects += object->findChildren<QObject*>();
    d->objectList = objects;

    for(int i=0; i<objects.count(); i++)
    {
        QObject* obj = objects[i];
        addPropertiesFrom(obj);
        connect(object, SIGNAL(destroyed(QObject*)), this, SLOT(on_object_destroyed(QObject*)));
        if(object->metaObject()->indexOfSignal("propertyChanged(QString)") > -1)
            connect(object, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));
    }
}

QObject* GCF::Components::PropertyEditor::object()
{
    return d->object;
}

int GCF::Components::PropertyEditor::propertyCount()
{
    return d->propertyList.count();
}

IProperty* GCF::Components::PropertyEditor::property(int index)
{
    if(index < 0 || index >= d->propertyList.count())
        return 0;

    return d->propertyList[index];
}

void GCF::Components::PropertyEditor::setEditorFactory(IValueEditorFactory* factory)
{
    if(d->valueEditorFactory == factory)
        return;
    if(d->valueEditorFactory)
        d->valueEditorFactory->destroySelf();
    d->valueEditorFactory = factory;
}

IValueEditorFactory* GCF::Components::PropertyEditor::editorFactory()
{
    return d->valueEditorFactory;
}

void GCF::Components::PropertyEditor::on_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* old)
{
    if(d->inCurrentItemChanged || !d->object || !d->valueEditorFactory)
        return;

    d->inCurrentItemChanged = true;
    if(old)
    {
        GCF::Components::Property* prop = old->data(0, Qt::UserRole).value<GCF::Components::Property*>();
        QWidget* editor = itemWidget(old, 1);
        if(prop && editor)
        {
            QVariant value;
            if(prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()))
            {
                value = prop->editableProperties()->editorValue(prop->editablePropertyIndex(), editor);
            }
            else
            {
                IValueEditorCreator* creator = prop->valueEditorCreator();
                value = creator->editorValue(editor);
            }
            prop->setPropertyValue(value);
            this->on_editor_destroyed(); // deliver notifications now.
            setItemWidget(old, 1, 0);
            editor->hide();
            editor->deleteLater();
        }
    }

    d->currentProperty = 0;
    d->currentEditor = 0;

    if(current)
    {
        GCF::Components::Property* prop = current->data(0, Qt::UserRole).value<GCF::Components::Property*>();
        IValueEditorCreator* creator = prop ? prop->valueEditorCreator() : 0;
        bool isEnum = prop ? prop->isEnum() : false;
        QWidget* editor = 0;
        QVariant value = prop ? prop->propertyValue() : QVariant();

        if( !d->readOnly )
        {
            if(prop && prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()) )
                editor = prop->editableProperties()->createEditor( prop->editablePropertyIndex() );

            if(!editor)
                editor = creator ? creator->createEditor(viewport()) : 0;
        }

        if(editor)
        {
            editor->setFocus();
            if(isEnum && d->valueEditorFactory->enumCreator())
                d->valueEditorFactory->enumCreator()->initialize(editor, prop->enumStruct());
            setItemWidget(current, 1, editor);

            if(prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()))
                prop->editableProperties()->setEditorValue(prop->editablePropertyIndex(), editor, value);
            else
                creator->setEditorValue(editor, value);
            editor->setProperty("GCF::Components::Property", qVariantFromValue<GCF::Components::Property*>(prop));
            editor->setAutoFillBackground(true);
            editor->installEventFilter(this);

            if(prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()))
                prop->editableProperties()->connectEditor(prop->editablePropertyIndex(), editor, this, SLOT(on_editor_valueEdited()));
            else
                creator->connectEditor(editor, this, SLOT(on_editor_valueEdited()));

            connect(editor, SIGNAL(destroyed(QObject*)), this, SLOT(on_editor_destroyed()));

            IPropertyEditorNotifications* pen = prop->editorNotifications();
            if(pen)
                pen->editorCreatedNotification(prop, editor);
        }

        d->currentProperty = prop;
        d->currentEditor = editor;
    }
    d->inCurrentItemChanged = false;
}

void GCF::Components::PropertyEditor::on_editor_valueEdited()
{
    QWidget* editor = qobject_cast<QWidget*>(sender());
    if(!editor)
        return;

    GCF::Components::Property* prop = editor->property("GCF::Components::Property").value<GCF::Components::Property*>();
    if(!prop)
        return;

    QVariant oldVal = prop->propertyValue();
    QVariant value;
    if(prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()))
    {
        value = prop->editableProperties()->editorValue(prop->editablePropertyIndex(), editor);
    }
    else
    {
        IValueEditorCreator* creator = prop->valueEditorCreator();
        if(!creator)
            return;

        value = creator->editorValue(editor);
    }

	QObject* propObject = prop->propertyObject();

    QString propName = prop->propertyNameQString();
	QMetaObject::invokeMethod(this, "propertyValueChanged", Qt::QueuedConnection,
		Q_ARG(QObject*,propObject), Q_ARG(QString,propName),
		Q_ARG(QVariant,oldVal), Q_ARG(QVariant,value));

    IPropertyEditorNotifications* pen = prop->editorNotifications();
    if(pen)
        pen->editorEditedNotification(prop, editor);

    prop->setPropertyValue(value);
}

void GCF::Components::PropertyEditor::on_editor_destroyed()
{
    if(!d->currentProperty || d->currentEditor.isNull())
        return;

    IPropertyEditorNotifications* pen = d->currentProperty->editorNotifications();
    if(pen)
        pen->editorAboutToDestroyNotification(d->currentProperty, d->currentEditor);

    d->currentEditor = 0;
    d->currentProperty = 0;
}

void GCF::Components::PropertyEditor::on_propertyChanged(const QString& name)
{
    if(name.isEmpty())
    {
        // Refresh all the properties
        QObject* object = d->object;
        setObject(0);
        setObject(object);
        return;
    }

    QObject* object = sender();
    QList<QTreeWidgetItem*> items = findItems(name, Qt::MatchExactly|Qt::MatchRecursive, 0);
    if(!items.count() || items.count() > 1)
        return;

    QTreeWidgetItem* item = items.first();
    if(item->isSelected())
        return;

    GCF::Components::Property* prop = item->data(0, Qt::UserRole).value<GCF::Components::Property*>();
    if(prop->propertyObject() != object)
        return;

    QString valueStr;

    if( prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()) )
    {
        valueStr = prop->editableProperties()->propertyValueString(prop->editablePropertyIndex());
    }
    else
    {
        IValueEditorCreator* creator = prop ? prop->valueEditorCreator() : 0;
        QVariant value = prop ? prop->propertyValue() : QVariant();
        valueStr = creator ? creator->toString(value) : QString();
    }

    if(prop)
        item->setText(1, valueStr);
}

void GCF::Components::PropertyEditor::refresh()
{
    QObject* object = d->object;
    bool recursive = d->recursive;
    this->setObject(0);
    qApp->processEvents();
    this->setObject(object, recursive);
}

bool GCF::Components::PropertyEditor::eventFilter(QObject* object, QEvent* event)
{
    if(event->type() == QEvent::KeyPress && object->isWidgetType())
    {
        QWidget* w = (QWidget*)object;
        if(w->window() != this->window())
            return false;

        QKeyEvent* ke = (QKeyEvent*)(event);
        if(ke->key() == Qt::Key_Tab || ke->key() == Qt::Key_Backtab)
        {
            QTreeWidgetItem* item = this->currentItem();
            QTreeWidgetItem* nextItem = 0;

            if(ke->key() == Qt::Key_Tab)
                nextItem = d->nextItem(item);
            else
                nextItem = d->previousItem(item);
            if(nextItem)
            {
                setCurrentItem(nextItem);
                return true;
            }
        }
    }
    else if(event->type() == QEvent::DeferredDelete)
        on_object_destroyed(object);

    return false;
}

void GCF::Components::PropertyEditor::addPropertiesFrom(QObject* object)
{
    const QMetaObject* metaObject = object->metaObject();
    IDefaultProperty* defProp = defaultProperty(object);
    IObjectPropertyVisibility* propVisibility = qobject_cast<IObjectPropertyVisibility*>(object);

    QStringList skipClassList = d->getSkipClassList(object);
    QTreeWidgetItem* objectItem = 0;

    QString defaultSectionName;

    if(d->recursive)
    {
        // Get a default section name, in case the class has now Q_PROPERTYies
        for(int i=metaObject->classInfoCount()-1; i>=0; i--)
        {
            QMetaClassInfo classInfo = metaObject->classInfo(i);
            if( qstrcmp(classInfo.name(), "SectionName") )
                continue;

            QString ciVal = QString("%1").arg(classInfo.value());
            if(ciVal[0] == '/')
            {
                QStringList comps = ciVal.split('/', QString::SkipEmptyParts);
                if(comps.count() >= 2)
                    defaultSectionName = defaultSectionName.replace( QRegExp(comps[0]), comps[1] );
            }
            else
                defaultSectionName = ciVal;
            break;
        }

        if(object != d->object)
            defaultSectionName += QString(" - (%1)").arg(object->objectName());

        objectItem = new QTreeWidgetItem(this);
        objectItem->setText(0, defaultSectionName);
    }

    while(metaObject && metaObject != &QObject::staticMetaObject)
    {
        if(!(metaObject->propertyCount()-metaObject->propertyOffset()))
        {
            metaObject = metaObject->superClass();
            continue;
        }

        QString sectionName(metaObject->className());
        if( skipClassList.contains(sectionName) )
        {
            metaObject = metaObject->superClass();
            continue;
        }

        if(!d->recursive)
        {
            for(int i=metaObject->classInfoCount()-1; i>=0; i--)
            {
                QMetaClassInfo classInfo = metaObject->classInfo(i);
                if( qstrcmp(classInfo.name(), "SectionName") )
                    continue;

                QString ciVal = QString("%1").arg(classInfo.value());
                if(ciVal[0] == '/')
                {
                    QStringList comps = ciVal.split('/', QString::SkipEmptyParts);
                    if(comps.count() >= 2)
                        sectionName = sectionName.replace( QRegExp(comps[0]), comps[1] );
                }
                else
                    sectionName = ciVal;
                break;
            }
        }

        QTreeWidgetItem* item = objectItem ? objectItem : new QTreeWidgetItem(this);
        if(item != objectItem)
            item->setText(0, qApp->translate("GCF", sectionName.toLatin1()));

        for(int i=metaObject->propertyOffset(); i<metaObject->propertyCount(); i++)
        {
            QMetaProperty prop = metaObject->property(i);
            if(!prop.isWritable())
                continue;

            if( propVisibility )
            {
                if(!propVisibility->isQPropertyVisible(i))
                    continue;
            }

            // 21st June 2012 - This is for translation
            // By default name of the property is picked from QMetaProperty.
            // If the user has provided a Q_CLASSINFO of the form
            // %name%.DisplayName
            QString propName;
            QByteArray displayNameKey = QString("%1.DisplayName").arg(prop.name()).toLatin1();
            int classInfoIndex = metaObject->indexOfClassInfo(displayNameKey);
            if(classInfoIndex >= 0)
                propName = QString("%1").arg(metaObject->classInfo(classInfoIndex).value());
            else
                propName = QString("%1").arg(prop.name());
            QTreeWidgetItem* propItem = new QTreeWidgetItem(item);
            propItem->setText(0, qApp->translate("GCF", propName.toLatin1()));

            GCF::Components::Property* prop2 = new GCF::Components::Property(object, prop.name());
            if(prop.isEnumType())
                prop2->setEnumStruct(prop.enumerator());
            if(d->valueEditorFactory)
            {
                if(prop.isEnumType())
                    prop2->setValueEditorCreator(d->valueEditorFactory->enumCreator());
                else
                    prop2->setValueEditorCreator(d->valueEditorFactory->creator(prop.typeName()));
            }
            propItem->setData(0, Qt::UserRole, qVariantFromValue<GCF::Components::Property*>(prop2));
            if(defProp->hasDefaultValue(propName))
                propItem->setData(1, Qt::UserRole, defProp->defaultValue(prop.name())); // store the default value.
            else
                d->noDefaultsList.append(propItem);
            d->propertyList.append(prop2);
        }
        item->setExpanded(true);
        metaObject = metaObject->superClass();
    }

    // Now add properties from the IEditableProperties interface
    IEditableProperties* editableProps = qobject_cast<IEditableProperties*>(object);
    if(!editableProps)
        return;

    for(int i=0; i<editableProps->propertyCount(); i++)
    {
        if( propVisibility )
        {
            if(!propVisibility->isEditablePropertyVisible(i))
                continue;
        }

        QString propName = editableProps->propertyName(i);
        QString groupName = editableProps->propertyGroup(i);
        QString propDisplayName = qApp->translate("GCF", propName.toLatin1());
        QString groupDisplayName = qApp->translate("GCF", groupName.toLatin1());
        QVariant propValue = editableProps->propertyValue(i);
        QTreeWidgetItem* groupItem = objectItem ? objectItem : d->groupItem(groupDisplayName, this);
        QTreeWidgetItem* propItem = new QTreeWidgetItem(groupItem);
        propItem->setText(0, propDisplayName);

        GCF::Components::Property* prop2 = new GCF::Components::Property(object, propName);
        prop2->setEditableProperties(editableProps);
        prop2->setEditbalePropertyIndex(i);
        if(d->valueEditorFactory)
            prop2->setValueEditorCreator(d->valueEditorFactory->creator(propValue.typeName()));
        propItem->setData(0, Qt::UserRole, qVariantFromValue<GCF::Components::Property*>(prop2));
        if(defProp->hasDefaultValue(propName))
            propItem->setData(1, Qt::UserRole, defProp->defaultValue(propName)); // store the default value.
        else
            d->noDefaultsList.append(propItem);
        d->propertyList.append(prop2);
        groupItem->setExpanded(true);
    }

    connect(object, SIGNAL(propertyConfigurationChanged()), this, SLOT(refresh()));
}

void GCF::Components::PropertyEditor::drawBranches(QPainter* p, const QRect & rect, const QModelIndex & index) const
{
    if(!index.parent().isValid())
        return;

    QPen pen = p->pen();
    p->setPen(palette().mid().color());
    p->drawLine(rect.bottomLeft(), rect.bottomRight());
    p->setPen(pen);

    if(!index.model()->hasChildren(index))
        return;

    // Draw custom brach indicators.
    // ### hardcoded in qcommonstyle.cpp
    static const int indRectConst = 9;
    QStyleOption branchOption;
    branchOption.init(this);
    branchOption.rect = QRect(rect.left() + indRectConst/2, rect.top() + (rect.height() - indRectConst)/2,
        indRectConst, indRectConst);
    branchOption.state = QStyle::State_Children;
    if(isExpanded(index))
        branchOption.state |= QStyle::State_Open;
    style()->drawPrimitive(QStyle::PE_IndicatorBranch, &branchOption, p, this);
}

void GCF::Components::PropertyEditor::mousePressEvent(QMouseEvent* me)
{
    QTreeWidgetItem* item = itemAt(me->pos());
    if(!item || !item->parent())
    {
        QTreeWidget::mousePressEvent(me);
        return;
    }

    bool modified = item->data(2, Qt::UserRole).toBool();
    if(!modified || me->x() < viewport()->width()-20)
    {
        QTreeWidget::mousePressEvent(me);
        return;
    }

    // Time to reset the property.
    GCF::Components::Property* prop = item->data(0, Qt::UserRole).value<GCF::Components::Property*>();
    if(!prop)
        return;
    QVariant value = item->data(1, Qt::UserRole);
    prop->setPropertyValue(value);
    viewport()->repaint();
}

void GCF::Components::PropertyEditor::setDefaultProperty(QObject* object, IDefaultProperty* prop)
{
    bool containsFlag = d->defaultPropertyMap.contains(object);
    if(containsFlag)
        d->defaultPropertyMap[object]->destroySelf();

    if(prop)
    {
        d->defaultPropertyMap[object] = prop;
        if(!containsFlag)
        {
            connect(object, SIGNAL(destroyed(QObject*)),
                    this, SLOT(on_object_destroyed(QObject*)));
            if(object->metaObject()->indexOfSignal("propertyConfigurationChanged()") > -1)
                connect(object, SIGNAL(propertyConfigurationChanged()),
                    this, SLOT(on_object_propertyConfigurationChanged()));
        }
    }
    else
    {
        d->defaultPropertyMap.remove(object);
        disconnect(object, 0, this, 0);
    }
}

IDefaultProperty* GCF::Components::PropertyEditor::defaultProperty(QObject* object)
{
    if(d->defaultPropertyMap.contains(object))
        return d->defaultPropertyMap[object];

    GCF::Components::DefaultProperty* dp = new GCF::Components::DefaultProperty(object);
    setDefaultProperty(object, dp);
    return dp;
}

void GCF::Components::PropertyEditor::on_object_destroyed(QObject* obj)
{
    if(d->defaultPropertyMap.contains(obj))
    {
        IDefaultProperty* dp = d->defaultPropertyMap[obj];
        dp->destroySelf();
        d->defaultPropertyMap.remove(obj);
    }

    if( d->object == obj )
        setObject(0);
}

void GCF::Components::PropertyEditor::on_object_propertyConfigurationChanged()
{
    QObject* obj = sender();

    if(d->defaultPropertyMap.contains(obj))
    {
        IDefaultProperty* dp = d->defaultPropertyMap[obj];
        GCF::Components::DefaultProperty* cdp = dynamic_cast<GCF::Components::DefaultProperty*>(dp);
        if(cdp)
            cdp->updateDefaultValues();
    }
}

bool GCF::Components::PropertyEditor::saveProperties(QObject* obj, QDomDocument doc, QDomElement parentE)
{
    return GCF::Components::PropertyEditorComponent::instance().saveProperties(obj, doc, parentE);
}

bool GCF::Components::PropertyEditor::loadProperties(QObject* object, QDomDocument doc, QDomElement parentE)
{
    // First manage the default properties of this object.
    defaultProperty(object);
    return GCF::Components::PropertyEditorComponent::instance().loadProperties(object, doc, parentE);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
static QTextDocument & PropertyEditorDocument()
{
    static QTextDocument theDoc;
    return theDoc;
}

GCF::Components::PropertyEditorDelegate::PropertyEditorDelegate(QAbstractItemDelegate* old, QObject* parent, GCF::Components::PropertyEditorData* pd)
: QAbstractItemDelegate(parent), mOldDelegate(old), d(pd)
{
}

GCF::Components::PropertyEditorDelegate::~PropertyEditorDelegate()
{

}

void GCF::Components::PropertyEditorDelegate::paint(QPainter* p, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    void *ip = index.internalPointer();
    if(!index.isValid() || !ip)
    {
        mOldDelegate->paint(p, opt, index);
        return;
    }

    QTreeWidgetItem* twItem = static_cast<QTreeWidgetItem*>(ip);
    QTreeWidget* treeWidget = twItem->treeWidget();
    QStyle* style = treeWidget->style();

    if(twItem->parent())
    {
        bool painted = false;
        bool modified = false;

        QRect textRect = opt.rect.adjusted(2, 0, 0, 0);
        static QPixmap revertPixmap(":/PropertyEditor/revert.png");

        if(index.column() == 1)
        {
            GCF::Components::Property* prop = twItem->data(0, Qt::UserRole).value<GCF::Components::Property*>();
            IValueEditorCreator* veCreator = prop ? prop->valueEditorCreator() : 0;
            QVariant value = prop ? prop->propertyValue() : QVariant();
            QString stringValue;
            IEnumValueEditorCreator* enumCreator = 0;

            QVariant defValue = twItem->data(1, Qt::UserRole);
            modified = !d->noDefaultsList.contains(twItem) && (defValue != value);
            twItem->setData(2, Qt::UserRole, modified);

            if(modified)
                textRect.setRight( textRect.right() - 4 - 2*revertPixmap.width() ); // leave some space for the revert icon

            if( prop->editableProperties() && prop->editableProperties()->hasEditor(prop->editablePropertyIndex()) )
            {
                value = prop->editableProperties()->propertyValue(prop->editablePropertyIndex());
                stringValue = prop->editableProperties()->propertyValueString(prop->editablePropertyIndex());
                stringValue = p->fontMetrics().elidedText(stringValue, Qt::ElideRight, textRect.width());
                if(!stringValue.isEmpty())
                {
                    p->drawText(textRect, Qt::AlignVCenter|Qt::AlignLeft, stringValue);
                    painted = true;
                }
            }
            else
            {
                if(veCreator)
                {
                    if(prop->isEnum())
                        veCreator->queryInterface("IEnumValueEditorCreator", (void**)&enumCreator);
                    if(prop->isEnum() && enumCreator)
                        enumCreator->setCurrentEnum(prop->enumStruct());
                    painted = veCreator->paintValue(p, opt.rect.adjusted(2, 0, 0, 0), value);
                    if(prop->isEnum() && enumCreator)
                        enumCreator->setCurrentEnum(QMetaEnum());
                }

                if(!painted && veCreator)
                {
                    stringValue = veCreator->toString(value);
                    stringValue = p->fontMetrics().elidedText(stringValue, Qt::ElideRight, textRect.width());
                    if(!stringValue.isEmpty())
                    {
                        p->drawText(textRect, Qt::AlignVCenter|Qt::AlignLeft, stringValue);
                        painted = true;
                    }
                }
            }
        }

        if(!painted)
        {
            QStyleOptionViewItem* optPtr = const_cast<QStyleOptionViewItem*>(&opt);
            QRect oldRect = opt.rect;
            optPtr->rect = textRect;
            optPtr->textElideMode = Qt::ElideRight;
            mOldDelegate->paint(p, opt, index);
            optPtr->rect = oldRect;
        }

        // Paint a close button if the property value is not default.
        if(modified)
        {
            int size = revertPixmap.width();
            QRect rect( opt.rect.right()-size-2, opt.rect.center().y()-size/2, size, size );
            p->setRenderHint(QPainter::SmoothPixmapTransform, true);
            p->drawPixmap(rect, revertPixmap);
            p->setRenderHint(QPainter::SmoothPixmapTransform, false);
        }

        QPen pen = p->pen();
        p->setPen(opt.palette.mid().color());
        p->drawLine(opt.rect.bottomLeft(), opt.rect.bottomRight());

        if(index.column() != treeWidget->columnCount()-1)
            p->drawLine(opt.rect.topRight(), opt.rect.bottomRight());

        p->setPen(pen);
    }
    else
    {
        if(!index.column())
            mOffset = opt.rect.left();

        QStyleOptionButton hopt;
        hopt.rect = opt.rect;
        hopt.palette = opt.palette;
        hopt.state = QStyle::State_Active|QStyle::State_Enabled|QStyle::State_Horizontal|QStyle::State_Enabled|QStyle::State_Raised;
        hopt.rect.setLeft(0);
        hopt.rect.setRight(treeWidget->viewport()->width());

        // p->fillRect(opt.rect, opt.palette.mid());
        p->save();
        p->setClipping(true);
        p->setClipRect(opt.rect);
        treeWidget->style()->drawControl(QStyle::CE_PushButtonBevel, &hopt, p, 0);
        // p->translate(mOffset, opt.rect.top());
        p->drawText(hopt.rect, Qt::AlignCenter, twItem->text(0));
        p->restore();
    }
}

QSize GCF::Components::PropertyEditorDelegate::sizeHint(const QStyleOptionViewItem & opt, const QModelIndex & index) const
{
    QSize size;

    void *ip = index.internalPointer();
    if(!index.isValid() || !ip)
        size = mOldDelegate->sizeHint(opt, index);
    else
    {
        QTreeWidgetItem* twItem = static_cast<QTreeWidgetItem*>(ip);
        if(twItem->parent())
            size = mOldDelegate->sizeHint(opt, index) + QSize(4, 6);
        else
            size = mOldDelegate->sizeHint(opt, index) + QSize(4,8);
    }

    size.setHeight(qMin(26, size.height()));
    return size;
}

QWidget* GCF::Components::PropertyEditorDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem& opt, const QModelIndex& index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(index);
    Q_UNUSED(opt);
    return 0;
}

void GCF::Components::PropertyEditorDelegate::setEditorData(QWidget * editor, const QModelIndex &index) const
{
    Q_UNUSED(editor);
    Q_UNUSED(index);
}

bool GCF::Components::PropertyEditorDelegate::editorEvent(QEvent *event, QAbstractItemModel* model, const QStyleOptionViewItem & opt, const QModelIndex& index)
{
    Q_UNUSED(event);
    Q_UNUSED(model);
    Q_UNUSED(opt);
    Q_UNUSED(index);
    return false;
}

void GCF::Components::PropertyEditorDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    if(mOldDelegate)
        mOldDelegate->setModelData(editor, model, index);
}

void GCF::Components::PropertyEditorDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    editor->setGeometry(option.rect);
    Q_UNUSED(index);
}

QTreeWidgetItem* GCF::Components::PropertyEditorData::groupItem(QString name, QTreeWidget* treeWidget)
{
    if(!treeWidget)
        return 0;

    for(int i=0; i<treeWidget->topLevelItemCount(); i++)
    {
        QTreeWidgetItem* item = treeWidget->topLevelItem(i);
        if(item->text(0) == name)
            return item;
    }

    QTreeWidgetItem* item = new QTreeWidgetItem(treeWidget);
    item->setText(0, name);
    return item;
}

QTreeWidgetItem* GCF::Components::PropertyEditorData::nextItem(QTreeWidgetItem* item)
{
    if(!item)
        return 0;

    QTreeWidgetItemIterator it(item, QTreeWidgetItemIterator::NoChildren);
    ++it;
    return *it;
}

QTreeWidgetItem* GCF::Components::PropertyEditorData::previousItem(QTreeWidgetItem* item)
{
    if(!item)
        return 0;

QTreeWidgetItemIterator it(item, QTreeWidgetItemIterator::NoChildren);
    --it;
    return *it;
}

QStringList GCF::Components::PropertyEditorData::getSkipClassList(QObject* object)
{
    QStringList ret;

    const QMetaObject* mo = object->metaObject();
    while( mo && mo != &QObject::staticMetaObject )
    {
        for(int i=mo->classInfoOffset(); i<mo->classInfoCount(); i++)
        {
            QMetaClassInfo ci = mo->classInfo(i);
            if( qstrcmp(ci.name(), "SkipClassProperties") )
                continue;

            QString value( ci.value() );
            QStringList valueList = value.split(" ", QString::SkipEmptyParts);
            ret += valueList;
        }

        mo = mo->superClass();
    }

    return ret;
}

int GCF::Components::PropertyEditorData::indexOfEditableProperty(IEditableProperties* editableProps, const QString& propName)
{
    if( !editableProps || propName.isEmpty() )
        return -1;

    for(int i=0; i<editableProps->propertyCount(); i++)
    {
        QString pName = editableProps->propertyName(i);
        if( pName == propName )
            return i;
    }

    return -1;
}

/////////////////////////////////////////////////////////////////////////////////
// GCF::Components::DefaultProperty implementation
/////////////////////////////////////////////////////////////////////////////////

struct GCF::Components::DefaultPropertyData
{
    QObject* object;
    QMap<QString, QVariant> propertyValueMap;
};

GCF::Components::DefaultProperty::DefaultProperty(QObject* object)
{
    d = new GCF::Components::DefaultPropertyData;

    d->object = object;

    // Gather all the default properties now.
    const QMetaObject* mo = object->metaObject();
    for(int i=0; i<mo->propertyCount(); i++)
    {
        QMetaProperty prop = mo->property(i);
        d->propertyValueMap[ prop.name() ] = prop.read(object);
    }
    IEditableProperties* props = qobject_cast<IEditableProperties*>(object);
    if(props)
    {
        for(int i=0; i<props->propertyCount(); i++)
        {
            QString propName = props->propertyName(i);
            QVariant propValue = props->propertyValue(i);
            d->propertyValueMap[propName] = propValue;
        }
    }
}

GCF::Components::DefaultProperty::~DefaultProperty()
{
    delete d;
}

void GCF::Components::DefaultProperty::updateDefaultValues()
{
    // This usually happens for property values that were previously
    // in the object, but now are not or vice versa. We only need
    // to refresh from IEditableProperties for this.
    IEditableProperties* props = qobject_cast<IEditableProperties*>(d->object);
    if(!props)
        return;

    QStringList keys = d->propertyValueMap.keys();

    const QMetaObject* mo = d->object->metaObject();
    for(int i=0; i<mo->propertyCount(); i++)
    {
        QMetaProperty prop = mo->property(i);
        keys.removeAll(prop.name());
    }

    for(int i=0; i<props->propertyCount(); i++)
    {
        QString propName = props->propertyName(i);
        keys.removeAll(propName);
        if(d->propertyValueMap.contains(propName))
            continue;

        QVariant propValue = props->propertyValue(i);
        d->propertyValueMap[propName] = propValue;
    }

    for(int i=0; i<keys.count(); i++)
        d->propertyValueMap.remove(keys[i]);
}

GCF_BEGIN_QUERY_TABLE(GCF::Components::DefaultProperty)
    GCF_IMPLEMENTS(IDefaultProperty)
GCF_END_QUERY_TABLE(GCF::Components::DefaultProperty)

QObject* GCF::Components::DefaultProperty::object()
{
    return d->object;
}

bool GCF::Components::DefaultProperty::hasDefaultValue(QString propName)
{
    return d->propertyValueMap.contains(propName);
}

QVariant GCF::Components::DefaultProperty::defaultValue(QString propName)
{
    if(d->propertyValueMap.contains(propName))
        return d->propertyValueMap[propName];

    return QVariant();
}



