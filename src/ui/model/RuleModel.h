#ifndef RULEMODEL_H
#define RULEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "../../core/rogerian/Engine.h"

class TreeItem;

class RuleModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Role {
        NameRole = Qt::UserRole + 1,
        CategoryRole,
        TemplatesRole,
        HitsRole
    };

    explicit RuleModel(deep_thonk::Engine* engine, QObject *parent = nullptr);
    ~RuleModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
    void setupModelData(const std::map<std::string, deep_thonk::RulePack>& rulePacks, TreeItem *parent);

    TreeItem *rootItem;
    deep_thonk::Engine* m_engine;
};

#endif // RULEMODEL_H
