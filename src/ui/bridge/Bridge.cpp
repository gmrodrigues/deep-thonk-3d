#include "Bridge.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

Bridge::Bridge(QObject *parent) : QObject(parent)
{
    // Load rule files from resources
    QFile enFile(":/resources/rules/en-US.json");
    if (enFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&enFile);
        std::string content = in.readAll().toStdString();
        m_engine.loadRulesFromString(content);
        enFile.close();
    }

    QFile ptFile(":/resources/rules/pt-BR.json");
    if (ptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ptFile);
        std::string content = in.readAll().toStdString();
        m_engine.loadRulesFromString(content);
        ptFile.close();
    }

    m_ruleModel = new RuleModel(&m_engine, this);

    // Set default locale
    setLocale("en-US");
}

Bridge::~Bridge()
{
    delete m_ruleModel;
}

QAbstractItemModel* Bridge::ruleModel() const
{
    return m_ruleModel;
}

void Bridge::submitMessage(const QString &message)
{
    qDebug() << "Message received:" << message;
    std::string response = m_engine.respond(message.toStdString());
    emit rogerianReply(QString::fromStdString(response));
}

void Bridge::setLocale(const QString &locale)
{
    qDebug() << "Locale set to:" << locale;
    m_engine.setLocale(locale.toStdString());
}
