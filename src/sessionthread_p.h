/*
  Copyright 2010 BetterInbox <contact@betterinbox.com>
      Author: Christophe Laveault <christophe@betterinbox.com>
              Gregory Schlomoff <gregory.schlomoff@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KSMTP_SESSIONTHREAD_P_H
#define KSMTP_SESSIONTHREAD_P_H

#include <QThread>
#include <QMutex>
#include <QQueue>

#include <ktcpsocket.h>

class QFile;
namespace KSmtp
{

class ServerResponse;
class Session;

class SessionThread : public QThread
{
    Q_OBJECT

public:
    explicit SessionThread(const QString &hostName, quint16 port, Session *session);
    ~SessionThread() Q_DECL_OVERRIDE;

    QString hostName() const;
    quint16 port() const;

public Q_SLOTS:
    void reconnect();
    void closeSocket();
    void startTls();
    void tlsConnected();
    void sendData(const QByteArray &payload);

Q_SIGNALS:
    void responseReceived(const ServerResponse &response);
    void sslError(const KSslErrorUiData &);

protected:
    void run() Q_DECL_OVERRIDE;

private Q_SLOTS:
    void writeDataQueue();
    void readResponse();
    void doCloseSocket();

private:
    ServerResponse parseResponse(const QByteArray &response);

    KTcpSocket *m_socket;
    QMutex m_mutex;
    QQueue<QByteArray> m_dataQueue;
    QFile *m_logFile;

    Session *m_parentSession;
    QString m_hostName;
    quint16 m_port;
};

}

#endif // KSMTP_SESSIONTHREAD_H