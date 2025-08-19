#ifndef DEEPTHONK3D_BRIDGE_H
#define DEEPTHONK3D_BRIDGE_H

#include <QObject>
#include "../../core/rogerian/Engine.h"
#include "../model/RuleModel.h"

class Bridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* ruleModel READ ruleModel CONSTANT)

public:
    explicit Bridge(QObject *parent = nullptr);
    ~Bridge();

    QAbstractItemModel* ruleModel() const;

public slots:
    void submitMessage(const QString &message);
    void setLocale(const QString &locale);

signals:
    void rogerianReply(const QString &reply, const QString &ruleId);

private:
    RuleModel* m_ruleModel;
    deep_thonk::Engine m_engine;
};

#endif //DEEPTHONK3D_BRIDGE_H
