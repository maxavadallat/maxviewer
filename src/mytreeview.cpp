#include <QDebug>

#include "mytreeview.h"

//==============================================================================
// Constructor
//==============================================================================
MyTreeView::MyTreeView(QWidget* aParent)
    : QTreeView(aParent)
{
    //qDebug() << "MyTreeView::MyTreeView";
}

//==============================================================================
// Current Changed
//==============================================================================
void MyTreeView::currentChanged(const QModelIndex& aCurrent, const QModelIndex& aPrevious)
{
    QTreeView::currentChanged(aCurrent, aPrevious);

    //qDebug() << "MyTreeView::currentChanged";

    // Emit Current Index Changed Signal
    emit currentIndexChanged(aCurrent);

    // ...
}

//==============================================================================
// Destructor
//==============================================================================
MyTreeView::~MyTreeView()
{
    // ...

    //qDebug() << "MyTreeView::~MyTreeView";
}

