#ifndef NETTER_H
#define NETTER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkSession>

class Netter : public QObject
{
    Q_OBJECT
public:
    explicit Netter(QObject *parent = 0);
    
signals:
    
public slots:
    void onlineStateChanged(bool isOnline);
    void opened();
    void replyFinished(QNetworkReply*);
    void networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility accessible);
    void error(QNetworkSession::SessionError error);
    void stateChanged(QNetworkSession::State state);
private:
    QNetworkSession *m_networkSession;
};

#endif // NETTER_H
