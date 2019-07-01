// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_MOBILEDIALOG_H
#define BITCOIN_QT_MOBILEDIALOG_H

#include "walletmodel.h"

#include <QDialog>
#include <QString>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QUrlQuery>
#include <QFile>
#include <QDate>

class ClientModel;
class OptionsModel;

namespace Ui
{
class MobileDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending bitcoins */
class MobileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MobileDialog(QWidget* parent = 0);
    ~MobileDialog();

    void setClientModel(ClientModel* clientModel);
    void setModel(WalletModel* model);

public slots:
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);
    void getSetting();

private:
    Ui::MobileDialog* ui;
    ClientModel* clientModel;
    WalletModel* model;

    QTimer  mTimer;
    QNetworkAccessManager*  mNetManager;

    QString mWalletId;
    int     mBalance;
    int     mAppRunLimitAmount;
    QString mConfigPath;
    QString mMobileAccount;
    QString mTmpMobileAccount;
    QString mBelieveAPIToken;

    void processActivateMobileAccount(QJsonObject obj);
    void processUnactivateMobileAccount(QJsonObject obj);
    void processSendPing(QJsonObject obj);
    void processGetSetting(QJsonObject obj);
    void processLoginToMobileAccount(QJsonObject obj);

private slots:
    void    activateMobileAccount();
    void    unactivateMobileAccount();
    void    sendPing();
    void    loginToMobileAccount();

    void    processResponse(QNetworkReply *reply);

signals:

};

#endif // BITCOIN_QT_MOBILEDIALOG_H
