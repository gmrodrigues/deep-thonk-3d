#include "Bridge.h"
#include <QDebug>

Bridge::Bridge(QObject *parent) : QObject(parent), m_ruleModel(nullptr)
{
    // TODO: Initialize rule model
}

QAbstractItemModel* Bridge::ruleModel() const
{
    return m_ruleModel;
}

void Bridge::submitMessage(const QString &message)
{
    qDebug() << "Message received:" << message;
    // TODO: Process message with Rogerian engine
    // For now, just echo back a reply
    emit rogerianReply("You said: " + message);
}

void Bridge::setLocale(const QString &locale)
{
    qDebug() << "Locale set to:" << locale;
    // TODO: Load the corresponding rule pack
}
