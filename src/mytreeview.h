#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H

#include <QTreeView>

//==============================================================================
// My Tree View Class
//==============================================================================
class MyTreeView : public QTreeView
{
    Q_OBJECT

public:
    // Constructor
    explicit MyTreeView(QWidget* aParent = NULL);

    // Destructor
    virtual ~MyTreeView();

signals:

    // Current Index Changed Signal
    void currentIndexChanged(const QModelIndex& aCurrent);

protected:

    // Current Changed
    virtual void currentChanged(const QModelIndex& aCurrent, const QModelIndex& aPrevious);

};

#endif // MYTREEVIEW_H
