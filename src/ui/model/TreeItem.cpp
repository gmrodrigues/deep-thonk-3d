#include "TreeItem.h"
#include <QDebug>

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
    : m_itemData(data), m_parentItem(parent)
{
    qDebug() << "Created TreeItem with data:" << m_itemData;
}

TreeItem::~TreeItem()
{
    qDebug() << "Deleting TreeItem with data:" << m_itemData;
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

void TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return;
    qDebug() << "Setting data for item" << m_itemData[0].toString() << "at column" << column << "from" << m_itemData[column] << "to" << value;
    m_itemData[column] = value;
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
