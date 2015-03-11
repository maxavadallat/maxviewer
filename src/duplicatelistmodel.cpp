
#include <QDebug>

#include "mainbrowserwindow.h"
#include "duplicatelistmodel.h"
#include "constants.h"

//==============================================================================
// Constructor
//==============================================================================
DuplicateListModel::DuplicateListModel(QObject* aParent)
    : QAbstractListModel(aParent)
{

}

//==============================================================================
// Add Data
//==============================================================================
void DuplicateListModel::addData(const int& aKey, const QString& aFileName)
{
    // Begin Insert Rows
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    // Insert Item
    duplicateMap.insert(aKey, aFileName);

    // End Insert Rows
    endInsertRows();
}

//==============================================================================
// Clear
//==============================================================================
void DuplicateListModel::clear()
{
    qDebug() << "DuplicateListModel::clear";

    // Begin Reset Model
    beginResetModel();

    // Clear
    duplicateMap.clear();

    // End Reset Model
    endResetModel();
}

//==============================================================================
// Get Index Of Value
//==============================================================================
int DuplicateListModel::indexOfValue(const QString& aValue)
{
    // Init Index
    int index = -1;

    // Check Value
    if (!aValue.endsWith(DUPLICATE_FILE_LIST_ITEM_SELECTED_SUFFIX)) {
        // Get Index With Suffix
        index = duplicateMap.values().indexOf(aValue + DUPLICATE_FILE_LIST_ITEM_SELECTED_SUFFIX);
    }

    return index < 0 ? duplicateMap.values().indexOf(aValue) : index;
}

//==============================================================================
// Get Role Names
//==============================================================================
QHash<int, QByteArray> DuplicateListModel::roleNames() const
{
    // Init Roles
    QHash<int, QByteArray> roles;
    // Set Image Selected
    roles[ItemKey] = "itemKey";
    // Set Image Source
    roles[ImageSource] = "imageSource";
    // Set Image List
    roles[ImageList] = "imageList";

    return roles;
}

//==============================================================================
// Get Row Count
//==============================================================================
int DuplicateListModel::rowCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return duplicateMap.uniqueKeys().count();
}

//==============================================================================
// Get Column Count
//==============================================================================
int DuplicateListModel::columnCount(const QModelIndex& aParent) const
{
    Q_UNUSED(aParent);

    return 2;
}

//==============================================================================
// Get Data
//==============================================================================
QVariant DuplicateListModel::data(const QModelIndex& aIndex, int aRole) const
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {
        // Switch Role
        switch (aRole) {
            case ItemKey: {
                // Get Unique Key
                int key = duplicateMap.uniqueKeys()[aIndex.row()];

                return QVariant::fromValue(key);
            } break;

            case ImageSource: {
                // Get Unique Key
                int key = duplicateMap.uniqueKeys()[aIndex.row()];
                // Image Source
                QString imageSource = duplicateMap.values(key).at(0);

                // Check Image Source
                if (imageSource.endsWith(" *"))
                    // Adjust Image Source
                    imageSource = imageSource.left(imageSource.length() - 2);

                // Return Value
                return imageSource;
            } break;

            case ImageList: {
                // Get Unique Key
                int key = duplicateMap.uniqueKeys()[aIndex.row()];
                // Init New String List
                QStringList newList = duplicateMap.values(key);
                // Sort List
                newList.sort();
                // Return Value
                return QVariant::fromValue(newList);
            } break;

            default:
            break;
        }
    }

    return QVariant::fromValue(NULL);
}

//==============================================================================
// Set Data
//==============================================================================
bool DuplicateListModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    // Check Index
    if (aIndex.row() >= 0 && aIndex.row() < rowCount()) {

        //qDebug() << "DuplicateListModel::setData - row: " << aIndex.row() << " - aValue: " << aValue << " - aRole: " << aRole;

        // Switch Role
        switch (aRole) {
            case ItemKey: {
                // Read Only
            } break;

            case ImageSource: {
                // Read Only
            } break;

            case ImageList: {
                // Get Unique Key
                int key = duplicateMap.uniqueKeys()[aIndex.row()];
                // Remove Items
                duplicateMap.remove(key);

                //qDebug() << "#### DuplicateListModel::setData - removedCount: " << removedCount;

                // Get New String List
                QStringList newList = aValue.toStringList();
                // Get New List Count
                int nlcount = newList.count();

                // Go Thru New List
                for (int i=0; i<nlcount; ++i) {
                    // Inser New List Items
                    duplicateMap.insert(key, newList[i]);
                }

                // Emit Data Changed
                emit dataChanged(aIndex, aIndex);

                return true;
            } break;

            default:
            break;
        }
    }

    return false;
}

//==============================================================================
// Destructor
//==============================================================================
DuplicateListModel::~DuplicateListModel()
{
    // Clear Duplicate Map
    duplicateMap.clear();

    qDebug() << "DuplicateListModel::~DuplicateListModel";
}

