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

#ifndef UPDATE_SELECTION_DIALOG_H
#define UPDATE_SELECTION_DIALOG_H

#include <QDialog>
#include <GCF/UpdateInfo>

class QTreeWidgetItem;

namespace GCF
{
struct UpdateSelectionDialogData;
class UpdateSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    UpdateSelectionDialog(const GCF::UpdateInfoList& list, QWidget* parent=0);
    ~UpdateSelectionDialog();

    GCF::UpdateInfoList listedUpdates() const;
    GCF::UpdateInfoList selectedUpdates() const;

protected slots:
    void slotItemChanged(QTreeWidgetItem* item);

private:
    UpdateSelectionDialogData* d;
};
}

#endif

