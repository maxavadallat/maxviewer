#ifndef DUPLICATELISTMODEL_H
#define DUPLICATELISTMODEL_H

#include <QString>
#include <QAbstractListModel>
#include <QMultiMap>

class MainBrowserWindow;

//==============================================================================
// File Duplicate List Model Class
//==============================================================================
class DuplicateListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    // Roles
    enum DuplicateRoles {
        ItemKey = Qt::UserRole + 1,
        ImageSource,
        ImageList
    };

    // Constructor
    DuplicateListModel(QObject* aParent = NULL);

    // Add Data
    void addData(const int& aKey, const QString& aFileName);

    // Clear
    void clear();

    // Get Index Of Value
    int indexOfValue(const QString& aValue);

    // Get Role Names
    virtual QHash<int, QByteArray> roleNames() const;
    // Get Row Count
    virtual int rowCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Column Count
    virtual int columnCount(const QModelIndex& aParent = QModelIndex()) const;
    // Get Data
    virtual QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const;
    // Set Data
    virtual bool setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole = Qt::EditRole);

    // Destructor
    virtual ~DuplicateListModel();

public:


protected:
    friend class MainBrowserWindow;

    // Duplicates Map
    QMultiMap<int, QString>         duplicateMap;

};

#endif // DUPLICATELISTMODEL_H
