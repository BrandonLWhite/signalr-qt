/*
 *  Copyright (c) 2013, p3root - Patrik Pfaffenbauer (patrik.pfaffenbauer@p3.co.at)
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification,
 *  are permitted provided that the following conditions are met:
 *
 *    Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or
 *    other materials provided with the distribution.
 *
 *    Neither the name of the {organization} nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "SignalException.h"
#include <QMap>
#include <QMutex>
#include <QSslError>
#include <QPair>

#if defined(Q_OS_QNX)
	#include <QtNetwork/qnetworkaccessmanager.h>
	#include <QtNetwork/qnetworkreply.h>
#else
	#include <QNetworkAccessManager>
	#include <QNetworkReply>
#endif

class Connection;

class HttpClient : public QObject
{
    Q_OBJECT
public:
    HttpClient(Connection* con);
    virtual ~HttpClient();

    void get(QString url);
    void post(QString url, QMap<QString, QString> arguments);

    void abort();

Q_SIGNALS:
    void getRequestCompleted(const QString& httpResponse, SignalException* error);
    void postRequestCompleted(const QString& httpResponse, SignalException* error);

private Q_SLOTS:
    void requestFinished(QNetworkReply* reply);

    void onIgnoreSSLErros(QNetworkReply *reply, QList<QSslError> error);

private:
    void getRequestFinished(QNetworkReply *reply);
    void replyError(QNetworkReply::NetworkError, QNetworkReply *reply);

    void postRequestFinished(QNetworkReply *reply);


private:
    bool _isAborting;
    QNetworkAccessManager *_man;
    QMutex *_getMutex;
    QMutex *_postMutex;
    Connection *_connection;

    QList<QNetworkReply*> _currentConnections;
};

#endif // HTTPCLIENT_H
