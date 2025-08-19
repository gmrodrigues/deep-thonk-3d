#include "RuleModel.h"
#include "TreeItem.h"
#include <QStringList>

RuleModel::RuleModel(deep_thonk::Engine* engine, QObject *parent)
    : QAbstractItemModel(parent), m_engine(engine)
{
    rootItem = new TreeItem({tr("Locale"), tr("Category"), tr("Pattern"), tr("Templates"), tr("Hits")});
    setupModelData(m_engine->getRulePacks(), rootItem);
}

RuleModel::~RuleModel()
{
    delete rootItem;
}

int RuleModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant RuleModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    QVariant value;

    switch (role) {
        case Qt::DisplayRole:
        case NameRole:
            value = item->data(0);
            break;
        case CategoryRole:
            value = item->data(1);
            break;
        case PatternRole:
            value = item->data(2);
            break;
        case TemplatesRole:
            value = item->data(3);
            break;
        case HitsRole:
            value = item->data(4);
            break;
        default:
            return QVariant();
    }

    return value.isValid() ? value : QVariant(QString(""));
}

QHash<int, QByteArray> RuleModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[CategoryRole] = "category";
    roles[PatternRole] = "pattern";
    roles[TemplatesRole] = "templates";
    roles[HitsRole] = "hits";
    return roles;
}

Qt::ItemFlags RuleModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant RuleModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex RuleModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex RuleModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int RuleModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void RuleModel::onRuleMatched(const QString& ruleId)
{
    if (ruleId.isEmpty()) return;

    TreeItem* item = findRuleItem(ruleId, rootItem);
    if (item) {
        QModelIndex index = createIndex(item->row(), 4, item);
        item->setData(4, item->data(4).toULongLong() + 1);
        emit dataChanged(index, index, {HitsRole});
    }
}

TreeItem* RuleModel::findRuleItem(const QString& ruleId, TreeItem* parent)
{
    for (int i = 0; i < parent->childCount(); ++i) {
        TreeItem* child = parent->child(i);
        if (child->data(0).toString() == ruleId) {
            return child;
        }
        TreeItem* result = findRuleItem(ruleId, child);
        if (result) {
            return result;
        }
    }
    return nullptr;
}

void RuleModel::setupModelData(const std::map<std::string, deep_thonk::RulePack>& rulePacks, TreeItem *parent)
{
    for(auto const& [locale, pack] : rulePacks)
    {
        TreeItem *localeItem = new TreeItem({QString::fromStdString(locale), QVariant(), QVariant(), QVariant(), QVariant()}, parent);
        parent->appendChild(localeItem);

        std::map<std::string, TreeItem*> categoryItems;

        for(const auto& rule : pack.rules)
        {
            TreeItem* categoryItem;
            if(categoryItems.find(rule.category) == categoryItems.end())
            {
                categoryItem = new TreeItem({QString::fromStdString(rule.category), QVariant(), QVariant(), QVariant(), QVariant()}, localeItem);
                localeItem->appendChild(categoryItem);
                categoryItems[rule.category] = categoryItem;
            }
            else
            {
                categoryItem = categoryItems[rule.category];
            }

            TreeItem *ruleItem = new TreeItem({
                QString::fromStdString(rule.id),
                QString::fromStdString(rule.category),
                QString::fromStdString(rule.patternString),
                QVariant(static_cast<qulonglong>(rule.outs.size())),
                QVariant(static_cast<qulonglong>(rule.hits))
            }, categoryItem);
            categoryItem->appendChild(ruleItem);
        }
    }
}
