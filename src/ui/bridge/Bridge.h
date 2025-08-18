#ifndef DEEPTHONK3D_BRIDGE_H
#define DEEPTHONK3D_BRIDGE_H

#include <QObject>
#include <QAbstractItemModel>

class Bridge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel* ruleModel READ ruleModel CONSTANT)

public:
    explicit Bridge(QObject *parent = nullptr);

    QAbstractItemModel* ruleModel() const;

public slots:
    void submitMessage(const QString &message);
    void setLocale(const QString &locale);

signals:
    void rogerianReply(const QString &reply);

private:
    QAbstractItemModel* m_ruleModel;
};

#endif //DEEPTHONK3D_BRIDGE_H
