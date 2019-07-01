// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_ADDIALOG_H
#define BITCOIN_QT_ADDIALOG_H

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

static const int MAX_UPLOAD_AD = 4;

class ClientModel;
class OptionsModel;

namespace Ui
{
class AdDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending bitcoins */
class AdDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdDialog(QWidget* parent = 0);
    ~AdDialog();

    void setClientModel(ClientModel* clientModel);
    void setModel(WalletModel* model);

public slots:
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);
    void getSetting();

private:
    Ui::AdDialog* ui;
    ClientModel* clientModel;
    WalletModel* model;

    QNetworkAccessManager*  mNetManager;

    int mAdsCount;
    QString mAdsId1, mAdsId2, mAdsId3, mAdsId4;
    QString mAdsStatus1, mAdsStatus2, mAdsStatus3, mAdsStatus4;
    QString mWalletId;
    int     mBalance;
    int     mUploadLimitAmount;
    QString mConfigPath;

    void processGenerateWalletId(QJsonObject obj);
    void processGetMyAdvertisement(QJsonObject obj);
    void processUploadAdvertisement(QJsonObject obj);
    void processStartAdvertisement(QJsonObject obj);
    void processStopAdvertisement(QJsonObject obj);
    void processDeleteAdvertisement(QJsonObject obj);
    void processGetSetting(QJsonObject obj);

private slots:
    void    generateWalletId();
    void    getMyAdvertisement();
    void    uploadAdvertisement();
    void    startAdvertisement(QString id);
    void    stopAdvertisement(QString id);
    void    deleteAdvertisement(QString id);

    void    processResponse(QNetworkReply *reply);

    void    stopAd1();
    void    stopAd2();
    void    stopAd3();
    void    stopAd4();

    void    activateAd1();
    void    activateAd2();
    void    activateAd3();
    void    activateAd4();

    void    deleteAd1();
    void    deleteAd2();
    void    deleteAd3();
    void    deleteAd4();

signals:

};

#endif // BITCOIN_QT_ADDIALOG_H
