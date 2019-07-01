// Copyright (c) 2011-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2017-2018 The Believe developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "mobiledialog.h"
#include "ui_mobiledialog.h"

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

MobileDialog::MobileDialog(QWidget* parent) : QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint),
                                                    ui(new Ui::MobileDialog),
                                                    clientModel(0),
                                                    model(0)
{
    ui->setupUi(this);

    mNetManager = new QNetworkAccessManager();
    QObject::connect(mNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(processResponse(QNetworkReply*)));

    // Send Ping every 10 seconds
    QObject::connect(&mTimer, SIGNAL(timeout()), this, SLOT(sendPing()));
    mTimer.start(15000);

    // Login Slot
    QObject::connect(ui->btnLogin, SIGNAL(clicked()), this, SLOT(loginToMobileAccount()));

    // Get configuration path
    mConfigPath = QString::fromStdString(GetDataDir().string() + QDir::separator().toLatin1()) + QString("wallet_info.conf");

    // Initialize values
    mAppRunLimitAmount = 0;
    mBalance = 0;

    ui->widLogin->setVisible(true);
    ui->widLoginSuccess->setVisible(false);

    getSetting();
    sendPing();
    unactivateMobileAccount();
}

void MobileDialog::setClientModel(ClientModel* clientModel)
{
    this->clientModel = clientModel;

    if (clientModel) {
        //connect(clientModel, SIGNAL(numBlocksChanged(int)), this, SLOT(updateSmartFeeLabel()));
    }
}

void MobileDialog::setModel(WalletModel* model)
{
    this->model = model;

    if (model && model->getOptionsModel()) {
        setBalance(model->getBalance(), model->getUnconfirmedBalance(), model->getImmatureBalance(),
                   model->getWatchBalance(), model->getWatchUnconfirmedBalance(), model->getWatchImmatureBalance());
    }
}

MobileDialog::~MobileDialog()
{
    delete ui;
}

void MobileDialog::setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchBalance, const CAmount& watchUnconfirmedBalance, const CAmount& watchImmatureBalance)
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

void MobileDialog::getSetting()
{
    QNetworkRequest         request;
    QString strSvr = API_SERVER + QString("/believesvr/api/getsetting");
    request.setUrl(QUrl(strSvr));

    mNetManager->get(request);
}

void MobileDialog::activateMobileAccount()
{
    if(mTmpMobileAccount.isEmpty())
        return;

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("email", mTmpMobileAccount);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/activatemobileaccount")));
    mNetManager->post(request,  postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MobileDialog::unactivateMobileAccount()
{
    if(mTmpMobileAccount.isEmpty())
        return;

    QNetworkRequest         request;

    QUrlQuery postData;
    postData.addQueryItem("email", mTmpMobileAccount);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/unactivatemobileaccount")));
    mNetManager->post(request,  postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MobileDialog::sendPing()
{
    QNetworkRequest         request;

    QUrlQuery postData;

    if(mWalletId.isEmpty())
    {
        QFile file(mConfigPath);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        in >> mWalletId;
        file.close();
    }

    if(!mWalletId.isEmpty())
        postData.addQueryItem("wallet_id", mWalletId);

    if(!mMobileAccount.isEmpty())
        postData.addQueryItem("email", mMobileAccount);

    request.setUrl(QUrl(API_SERVER + QString("/believesvr/api/sendping")));
    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MobileDialog::loginToMobileAccount()
{
    if(mBalance < mAppRunLimitAmount)
    {
        QMessageBox::information(NULL,
            tr("Activate Mobile App"),
            "Your balance is too low to login mobile account!",
            QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    QString email = ui->lineEditEmail->text();
    QString password = ui->lineEditPassword->text();

    if(email.isEmpty() || password.isEmpty())
    {
        QMessageBox::information(NULL,
            tr("Activate Mobile App"),
            "Please input email and password!",
            QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    mTmpMobileAccount = email;

    QUrlQuery postData;
    postData.addQueryItem("email", email);
    postData.addQueryItem("password", password);

    QNetworkRequest         request;
    QString strSvr = "https://api.believecore.io/v1/loginWallet";
    request.setRawHeader("X-ApiTokenBelieve", mBelieveAPIToken.toStdString().c_str());

    request.setUrl(QUrl(strSvr));

    mNetManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void MobileDialog::processResponse(QNetworkReply *reply)
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

    if(!strApi.compare("activatemobileaccount"))
        processActivateMobileAccount(obj);
    else if(!strApi.compare("unactivatemobileaccount"))
        processUnactivateMobileAccount(obj);
    else if(!strApi.compare("sendping"))
        processSendPing(obj);
    else if(!strApi.compare("getsetting"))
        processGetSetting(obj);
    else if(!strApi.compare("loginWallet"))
        processLoginToMobileAccount(obj);
}

void MobileDialog::processActivateMobileAccount(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
        mMobileAccount = mTmpMobileAccount;

        ui->widLogin->setVisible(false);
        ui->widLoginSuccess->setVisible(true);

        sendPing();
    }else {
        qDebug() << "Failed";

        QMessageBox::information(NULL,
            tr("Activate Mobile App"),
            "Login is failed! \n Please try again.",
            QMessageBox::Ok | QMessageBox::Default);

    }
}

void MobileDialog::processUnactivateMobileAccount(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
    }else {
        qDebug() << "Failed";
    }
}

void MobileDialog::processSendPing(QJsonObject obj)
{
    int status_code = obj.value("status_code").toInt();
    if(status_code == 1000)
    {
        qDebug() << "Success";
    }else {
        qDebug() << "Failed";
    }
}

void MobileDialog::processGetSetting(QJsonObject obj)
{
    // Check status code
    int str_status = obj.value("status_code").toInt();
    if(str_status != 1000)
    {
        return;
    }

    mAppRunLimitAmount = obj.value("app_run_limit_amount").toString().toInt();
    mBelieveAPIToken = obj.value("believe_api_token").toString();
}

void MobileDialog::processLoginToMobileAccount(QJsonObject obj)
{
    ui->lineEditEmail->setText("");
    ui->lineEditPassword->setText("");


    bool status = obj.value("success").toBool();
    if(status == false)
    {
        QMessageBox::information(NULL,
            tr("Activate Mobile App"),
            "Login is failed! \n Please try again.",
            QMessageBox::Ok | QMessageBox::Default);
        return;
    }

    activateMobileAccount();
}
