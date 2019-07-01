// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Believe developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "addialog.h"
#include "ui_addialog.h"

#include "addresstablemodel.h"
#include "askpassphrasedialog.h"
#include "bitcoinunits.h"
#include "clientmodel.h"
#include "coincontroldialog.h"
#include "guiutil.h"
#include "optionsmodel.h"
#include "walletmodel.h"
#include "util.h"
#include "base58.h"
#include "coincontrol.h"
#include "ui_interface.h"
#include "utilmoneystr.h"
#include "wallet.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QTextDocument>
#include <QDir>

AdDialog::AdDialog(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                    ui(new Ui::AdDialog),
                                                    clientModel(0),
                                                    model(0)
{
    ui->setupUi(this);

    ui->dateEditStart->setDate(QDate::currentDate());
    ui->dateEditEnd->setDate(QDate::currentDate().addDays(30));

    mNetManager = new QNetworkAccessManager();
    QObject::connect(mNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processResponse(QNetworkReply*)));

    QObject::connect(ui->btnSendAds, SIGNAL(clicked()), this, SLOT(uploadAdvertisement()));

    QObject::connect(ui->activateButtonAd1, SIGNAL(clicked()), this, SLOT(activateAd1()));
    QObject::connect(ui->activateButtonAd2, SIGNAL(clicked()), this, SLOT(activateAd2()));
    QObject::connect(ui->activateButtonAd3, SIGNAL(clicked()), this, SLOT(activateAd3()));
    QObject::connect(ui->activateButtonAd4, SIGNAL(clicked()), this, SLOT(activateAd4()));

    QObject::connect(ui->stopButtonAd1, SIGNAL(clicked()), this, SLOT(stopAd1()));
    QObject::connect(ui->stopButtonAd2, SIGNAL(clicked()), this, SLOT(stopAd2()));
    QObject::connect(ui->stopButtonAd3, SIGNAL(clicked()), this, SLOT(stopAd3()));
    QObject::connect(ui->stopButtonAd4, SIGNAL(clicked()), this, SLOT(stopAd4()));

    QObject::connect(ui->deleteButtonAd1, SIGNAL(clicked()), this, SLOT(deleteAd1()));
    QObject::connect(ui->deleteButtonAd2, SIGNAL(clicked()), this, SLOT(deleteAd2()));
    QObject::connect(ui->deleteButtonAd3, SIGNAL(clicked()), this, SLOT(deleteAd3()));
    QObject::connect(ui->deleteButtonAd4, SIGNAL(clicked()), this, SLOT(deleteAd4()));

    mConfigPath = QString::fromStdString(GetDataDir().string() + QDir::separator().toLatin1()) + QString("wallet_info.conf");

    QFile file(mConfigPath);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    in >> mWalletId;
    file.close();

    if(mWalletId.isEmpty())
    {
        generateWalletId();
    }

    ui->widAdLine1->setEnabled(false);
    ui->widAdLine2->setEnabled(false);
    ui->widAdLine3->setEnabled(false);
    ui->widAdLine4->setEnabled(false);

    ui->stopButtonAd1->setVisible(false);
    ui->activateButtonAd1->setVisible(true);
    ui->stopButtonAd2->setVisible(false);
    ui->activateButtonAd2->setVisible(true);
    ui->stopButtonAd3->setVisible(false);
    ui->activateButtonAd3->setVisible(true);
    ui->stopButtonAd4->setVisible(false);
    ui->activateButtonAd4->setVisible(true);

    mAdsCount = 0;
    mBalance = 0;

    getSetting();
}

void AdDialog::setClientModel(ClientModel* clientModel)
{
    this->clientModel = clientModel;

    if (clientModel) {
        //connect(clientModel, SIGNAL(numBlocksChanged(int)), this, SLOT(updateSmartFeeLabel()));
    }
}

void AdDialog::setModel(WalletModel* model)
{
    this->model = model;

    if (model && model->getOptionsModel()) {
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance(),
                   model->getWatchBalance(), model->getWatchUnconfirmedBalance(), model->getWatchImmatureBalance());
    }
}

AdDialog::~AdDialog()
{
    delete ui;
}

void AdDialog::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchBalance, const CAmount& watchUnconfirmedBalance, const CAmount& watchImmatureBalance)
{
    Q_UNUSED(unconfirmedBalance);
    Q_UNUSED(immatureBalance);
    Q_UNUSED(watchBalance);
    Q_UNUSED(watchUnconfirmedBalance);
    Q_UNUSED(watchImmatureBalance);

    if (model && model->getOptionsModel()) {
        uint64_t bal = 0;
        bal = balance;

        mBalance = (int)(bal / 100000000);
    }
}

void AdDialog::activateAd1()
{
    if(mBalance < mUploadLimitAmount)
        QMessageBox::information(NULL,
            tr("Activate Advertisement"),
            "Your balance is too low to start this advertisement!",
            QMessageBox::Ok | QMessageBox::Default);
    else
        startAdvertisement(mAdsId1);
}

void AdDialog::activateAd2()
{
    if(mBalance < mUploadLimitAmount * 2)
        QMessageBox::information(NULL,
            tr("Activate Advertisement"),
            "Your balance is too low to start this advertisement!",
            QMessageBox::Ok | QMessageBox::Default);
    else
        startAdvertisement(mAdsId2);
}

void AdDialog::activateAd3()
{
    if(mBalance <  mUploadLimitAmount * 3)
        QMessageBox::information(NULL,
            tr("Activate Advertisement"),
            "Your balance is too low to start this advertisement!",
            QMessageBox::Ok | QMessageBox::Default);
    else
        startAdvertisement(mAdsId3);
}

void AdDialog::activateAd4()
{
    if(mBalance <  mUploadLimitAmount * 4)
        QMessageBox::information(NULL,
            tr("Activate Advertisement"),
            "Your balance is too low to start this advertisement!",
            QMessageBox::Ok | QMessageBox::Default);
    else
        startAdvertisement(mAdsId4);
}

void AdDialog::stopAd1()
{
    stopAdvertisement(mAdsId1);
}

void AdDialog::stopAd2()
{
    stopAdvertisement(mAdsId2);
}

void AdDialog::stopAd3()
{
    stopAdvertisement(mAdsId3);
}

void AdDialog::stopAd4()
{
    stopAdvertisement(mAdsId4);
}

void AdDialog::deleteAd1()
{
    deleteAdvertisement(mAdsId1);
}

void AdDialog::deleteAd2()
{
    deleteAdvertisement(mAdsId2);
}

void AdDialog::deleteAd3()
{
    deleteAdvertisement(mAdsId3);
}

void AdDialog::deleteAd4()
{
    deleteAdvertisement(mAdsId4);
}

void AdDialog::getSetting()
{
    QNetworkRequest         request;
    QString strSvr = API_SERVER + QString("/believesvr/api/getsetting");
    request.setUrl(QUrl(strSvr));

    mNetManager->get(request);
}

void AdDialog::generateWalletId()
{
    QNetworkRequest         request;

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/generatewalletid")));
    mNetManager->post(request, "");
}

void AdDialog::getMyAdvertisement()
{
    if(mWalletId.isEmpty())
        return;

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("wallet_id", mWalletId);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/getmyadvertisement")));
    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void AdDialog::uploadAdvertisement()
{
    if(mWalletId.isEmpty())
        return;

    bool flg = true;

    if(mBalance < mUploadLimitAmount)
        flg = false;
    if(mAdsCount == 1 && mBalance < mUploadLimitAmount * 2)
        flg = false;
    if(mAdsCount == 2 && mBalance < mUploadLimitAmount * 3)
        flg = false;
    if(mAdsCount == 3 && mBalance < mUploadLimitAmount * 4)
        flg = false;
    if(mAdsCount >= 4)
        flg = false;

    if(flg == false)
    {
        QMessageBox::information(NULL,
            tr("Send Advertisement"),
            "Your balance is too low to upload this advertisement.",
            QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    QString wallet_id = mWalletId;
    QString title = ui->lineEditTitle->text();
    QString description = ui->textEditDescription->toPlainText();
    QString sitelink = ui->lineEditWebLink->text();

    if(title.isEmpty() ||
            description.isEmpty() ||
            sitelink.isEmpty())
    {
        QMessageBox::information(NULL,
            tr("Send Advertisement"),
            "Please fill all items of advertisment.",
            QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    QString start_at ="";
    QString end_at ="";
    if(ui->radioButtonDefault->isChecked())
    {
        start_at = QDate::currentDate().toString("yyyy-MM-dd");
        end_at = QDate::currentDate().addDays(30).toString("yyyy-MM-dd");
    }
    else if(ui->radioButtonDateRange->isChecked())
    {
        start_at = ui->dateEditStart->date().toString("yyyy-MM-dd");
        end_at = ui->dateEditEnd->date().toString("yyyy-MM-dd");
    }

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("wallet_id", wallet_id);
    postData.addQueryItem("title", title);
    postData.addQueryItem("description", description);
    postData.addQueryItem("sitelink", sitelink);
    postData.addQueryItem("start_at", start_at);
    postData.addQueryItem("end_at", end_at);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/uploadadvertisement")));
    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void AdDialog::startAdvertisement(QString id)
{
    if(id.isEmpty())
        return;

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("id", id);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/startadvertisement")));
    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void AdDialog::stopAdvertisement(QString id)
{
    if(id.isEmpty())
        return;

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("id", id);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/stopadvertisement")));
    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void AdDialog::deleteAdvertisement(QString id)
{
    if(id.isEmpty())
        return;

    if (QMessageBox::question(this, tr("Delete Advertisement"),
                              tr("Are you sure you want to delete this advertisement?"),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        QNetworkRequest         request;

        QUrlQuery postData;
        postData.addQueryItem("id", id);

        request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/deleteadvertisement")));
        mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }
}

void AdDialog::processResponse(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR : " << reply->errorString();
        return;
    }

    QNetworkRequest request = reply->request();
    QString strApi = request.url().fileName();

    QString response = reply->readAll();
    qDebug() << response;

    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());
    if(!doc.isObject())
    {
        qDebug() << "FOMART ERROR";
        return;
    }

    QJsonObject obj = doc.object();

    if(!strApi.compare("generatewalletid"))
        processGenerateWalletId(obj);
    else if(!strApi.compare("getmyadvertisement"))
        processGetMyAdvertisement(obj);
    else if(!strApi.compare("uploadadvertisement"))
        processUploadAdvertisement(obj);
    else if(!strApi.compare("startadvertisement"))
        processStartAdvertisement(obj);
    else if(!strApi.compare("stopadvertisement"))
        processStopAdvertisement(obj);
    else if(!strApi.compare("deleteadvertisement"))
        processDeleteAdvertisement(obj);
    else if(!strApi.compare("getsetting"))
        processGetSetting(obj);
}

void AdDialog::processGenerateWalletId(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        mWalletId = obj.value("wallet_id").toString();

        qDebug() << mWalletId;

        QFile file(mConfigPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << mWalletId;
        file.close();

    }else {
        qDebug() << "Failed";
    }
}

void AdDialog::processUploadAdvertisement(QJsonObject obj)
{

    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";

        ui->lineEditTitle->setText("");
        ui->textEditDescription->setText("");
        ui->lineEditWebLink->setText("");

        getSetting();
    }else {
        QMessageBox::information(NULL,
            tr("Send Advertisement"),
            "Operation is failed!",
            QMessageBox::Ok | QMessageBox::Default);
    }
}

void AdDialog::processStartAdvertisement(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
        getSetting();
    }else {
        qDebug() << "Failed";
    }
}

void AdDialog::processStopAdvertisement(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
        getSetting();
    }else {
        qDebug() << "Failed";
    }
}

void AdDialog::processDeleteAdvertisement(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
        getSetting();
    }else {
        qDebug() << "Failed";
    }
}

void AdDialog::processGetSetting(QJsonObject obj)
{
    // Check status code
    int str_status = obj.value("status_code").toInt();
    if(str_status != 1000)
    {
        return;
    }

    mUploadLimitAmount = obj.value("upload_limit_amount").toString().toInt();

    getMyAdvertisement();

}

void AdDialog::processGetMyAdvertisement(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
    }
    QJsonValue value = obj.value("data");

    mAdsCount = 0;
    mAdsId1 = mAdsId2 = mAdsId3 = mAdsId4 = "";
    mAdsStatus1 = mAdsStatus2 = mAdsStatus3 = mAdsStatus4 = "";

    ui->widAdLine1->setEnabled(false);
    ui->widAdLine2->setEnabled(false);
    ui->widAdLine3->setEnabled(false);
    ui->widAdLine4->setEnabled(false);

    ui->stopButtonAd1->setVisible(false);
    ui->activateButtonAd1->setVisible(true);
    ui->stopButtonAd2->setVisible(false);
    ui->activateButtonAd2->setVisible(true);
    ui->stopButtonAd3->setVisible(false);
    ui->activateButtonAd3->setVisible(true);
    ui->stopButtonAd4->setVisible(false);
    ui->activateButtonAd4->setVisible(true);

    ui->labelTitleOfAd1->setText("First Ad Title");
    ui->labelTitleOfAd2->setText("Second Ad Title");
    ui->labelTitleOfAd3->setText("Third Ad Title");
    ui->labelTitleOfAd4->setText("Fourth Ad Title");

    if(value.isArray())
    {
        QJsonArray data = value.toArray();

        mAdsCount = data.size();

        for(int i = 0 ; i < data.size(); i++)
        {
            QJsonObject item = data.at(i).toObject();

            QString id = item.value("id").toString();
            QString title = item.value("title").toString();
            QString description = item.value("description").toString();
            QString sitelink = item.value("sitelink").toString();
            QString status = item.value("status").toString();

            if(i == 0)
            {
                mAdsId1 = id;
                mAdsStatus1 = status;
                ui->widAdLine1->setEnabled(true);
                ui->labelTitleOfAd1->setText(title);
                if(!status.compare("running"))
                {
                    ui->stopButtonAd1->setVisible(true);
                    ui->activateButtonAd1->setVisible(false);
                }else {
                    ui->stopButtonAd1->setVisible(false);
                    ui->activateButtonAd1->setVisible(true);
                }
            }else if(i == 1)
            {
                mAdsId2 = id;
                mAdsStatus2 = status;
                ui->widAdLine2->setEnabled(true);
                ui->labelTitleOfAd2->setText(title);
                if(!status.compare("running"))
                {
                    ui->stopButtonAd2->setVisible(true);
                    ui->activateButtonAd2->setVisible(false);
                }else {
                    ui->stopButtonAd2->setVisible(false);
                    ui->activateButtonAd2->setVisible(true);
                }
            }else if(i == 2)
            {
                mAdsId3 = id;
                mAdsStatus3 = status;
                ui->widAdLine3->setEnabled(true);
                ui->labelTitleOfAd3->setText(title);
                if(!status.compare("running"))
                {
                    ui->stopButtonAd3->setVisible(true);
                    ui->activateButtonAd3->setVisible(false);
                }else {
                    ui->stopButtonAd3->setVisible(false);
                    ui->activateButtonAd3->setVisible(true);
                }
            }else if(i == 3)
            {
                mAdsId4 = id;
                mAdsStatus4 = status;
                ui->widAdLine4->setEnabled(true);
                ui->labelTitleOfAd4->setText(title);
                if(!status.compare("running"))
                {
                    ui->stopButtonAd4->setVisible(true);
                    ui->activateButtonAd4->setVisible(false);
                }else {
                    ui->stopButtonAd4->setVisible(false);
                    ui->activateButtonAd4->setVisible(true);
                }
            }
        }
    }

    ui->widNewAdvertisement->setEnabled(true);
    if(mBalance < mUploadLimitAmount)
    {
        ui->widNewAdvertisement->setEnabled(false);
        if(!mAdsStatus1.compare("running"))
            stopAdvertisement(mAdsId1);
        if(!mAdsStatus2.compare("running"))
            stopAdvertisement(mAdsId2);
        if(!mAdsStatus3.compare("running"))
            stopAdvertisement(mAdsId3);
        if(!mAdsStatus4.compare("running"))
            stopAdvertisement(mAdsId4);
    }
    else if(mAdsCount >= 1 && mBalance < mUploadLimitAmount * 2)
    {
        ui->widNewAdvertisement->setEnabled(false);
        if(!mAdsStatus2.compare("running"))
            stopAdvertisement(mAdsId2);
        if(!mAdsStatus3.compare("running"))
            stopAdvertisement(mAdsId3);
        if(!mAdsStatus4.compare("running"))
            stopAdvertisement(mAdsId4);
    }
    else if(mAdsCount >= 2 && mBalance < mUploadLimitAmount * 3)
    {
        ui->widNewAdvertisement->setEnabled(false);
        if(!mAdsStatus3.compare("running"))
            stopAdvertisement(mAdsId3);
        if(!mAdsStatus4.compare("running"))
            stopAdvertisement(mAdsId4);
    }
    else if(mAdsCount >= 3 && mBalance < mUploadLimitAmount * 4)
    {
        ui->widNewAdvertisement->setEnabled(false);
        if(!mAdsStatus4.compare("running"))
            stopAdvertisement(mAdsId4);
    }
    else if(mAdsCount >= 4)
    {
        ui->widNewAdvertisement->setEnabled(false);
    }
}
