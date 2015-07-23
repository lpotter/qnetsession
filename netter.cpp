#include "netter.h"
#include <QNetworkSession>
#include <QNetworkConfiguration>
#include <QNetworkConfigurationManager>
#include <QNetworkAccessManager>
#include <QCoreApplication>
#include <QStringList>
Netter::Netter(QObject *parent) :
    QObject(parent)
{
    if (QCoreApplication::arguments().count() > 1) {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
        connect(manager,SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                this,SLOT(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
//        manager->get(QNetworkRequest(QUrl("http://qt-project.org")));
        networkAccessibleChanged(manager->networkAccessible());

        QNetworkConfigurationManager *m_nmanager = new QNetworkConfigurationManager(this);
        connect(m_nmanager,SIGNAL(onlineStateChanged(bool)),
                this,SLOT(onlineStateChanged(bool)));

    } else {

        QNetworkConfigurationManager *m_nmanager = new QNetworkConfigurationManager(this);
        connect(m_nmanager,SIGNAL(onlineStateChanged(bool)),
                this,SLOT(onlineStateChanged(bool)));
        QNetworkConfiguration defaultConfig = m_nmanager->defaultConfiguration();

        // If default configuration is not valid, look for a valid one
        qDebug() << Q_FUNC_INFO << "number of configs" << m_nmanager->allConfigurations().count();
        qDebug() << "default cofig" << defaultConfig.name();

        foreach (const QNetworkConfiguration& cfg, m_nmanager->allConfigurations()) {
            qDebug() << "Found" << cfg.name();
//            if (!defaultConfig.isValid()) {
//                if (cfg.isValid()) {
//                    defaultConfig = cfg;
//                }
//            }
        }

        if (!defaultConfig.isValid()) {
            qWarning() << Q_FUNC_INFO << "No valid  network configuration found.";
        } else {
            qDebug() << defaultConfig.name()
                     << defaultConfig.bearerTypeName()
                     << defaultConfig.state()
                     << defaultConfig.type();

            if ((defaultConfig.state() & QNetworkConfiguration::Defined) == QNetworkConfiguration::Defined)
                qDebug() << "Defined";
            if ((defaultConfig.state() & QNetworkConfiguration::Discovered) == QNetworkConfiguration::Discovered)
                qDebug() << "Discovered";
            if ((defaultConfig.state() & QNetworkConfiguration::Active) == QNetworkConfiguration::Active)
                qDebug() << "Active";

            m_networkSession = new QNetworkSession(defaultConfig);

            connect(m_networkSession,SIGNAL(opened()),this,SLOT(opened()));
            connect(m_networkSession,SIGNAL(error(QNetworkSession::SessionError)),
                    this,SLOT(error(QNetworkSession::SessionError)));
            connect(m_networkSession,SIGNAL(stateChanged(QNetworkSession::State)),
                    this,SLOT(stateChanged(QNetworkSession::State)));

            qDebug() << "isOpen"<< m_networkSession->isOpen();
            m_networkSession->open();

        }
    }

}

void Netter::onlineStateChanged(bool isOnline)
{
    qDebug() << Q_FUNC_INFO << isOnline;
}

void Netter::opened()
{
    qDebug() << Q_FUNC_INFO << "<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>";

    qDebug() << "isOpen"<< m_networkSession->isOpen();
}

void Netter::replyFinished(QNetworkReply *reply)
{
    qDebug() << Q_FUNC_INFO << reply->error();
}

void Netter::networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible)
{
    QString accesStr;
    switch (accessible) {
    case QNetworkAccessManager::UnknownAccessibility:
        accesStr = "Unknown Accessibility";
        break;
    case QNetworkAccessManager::NotAccessible:
        accesStr = "No Accessibility";
        break;
    case QNetworkAccessManager::Accessible:
        accesStr = "Has Accessibility";
        break;
    };

    qDebug() << accesStr << accessible;
}


void Netter::error(QNetworkSession::SessionError error)
{
    QString result;
    switch (error) {
    case QNetworkSession::UnknownSessionError:
        result = "QNetworkSession::UnknownSessionError";
        break;
    case QNetworkSession::SessionAbortedError:
        result = "QNetworkSession::SessionAbortedError";
        break;
    case QNetworkSession::RoamingError:
        result = "QNetworkSession::RoamingError";
        break;
    case QNetworkSession::OperationNotSupportedError:
        result = "QNetworkSession::OperationNotSupportedError";
        break;
    case QNetworkSession::InvalidConfigurationError:
        result = "QNetworkSession::InvalidConfigurationError";
        break;
    };
    qDebug() << result;
}

void Netter::stateChanged(QNetworkSession::State state)
{
    QString result;
    switch (state) {
    case QNetworkSession::Invalid:
        result = "QNetworkSession::Invalid";
        break;
    case QNetworkSession::NotAvailable:
        result = "QNetworkSession::NotAvailable";
        break;
    case QNetworkSession::Connecting:
        result = "QNetworkSession::Connecting";
        break;
    case QNetworkSession::Connected:
        result = "QNetworkSession::Connected";
        break;
    case QNetworkSession::Closing:
        result = "QNetworkSession::Closing";
        break;
    case QNetworkSession::Disconnected:
        result = "QNetworkSession::Disconnected";
        break;
    case QNetworkSession::Roaming:
        result = "QNetworkSession::Roaming";
        break;
    };
    qDebug() << result;
}
