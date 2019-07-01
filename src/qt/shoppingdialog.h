// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_QT_SHOPPINGDIALOG_H
#define BITCOIN_QT_SHOPPINGDIALOG_H

#include "walletmodel.h"

#include <QDialog>
#include <QString>

class ClientModel;
class OptionsModel;
class SendCoinsEntry;
class SendCoinsRecipient;

namespace Ui
{
class ShoppingDialog;
}

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** Dialog for sending bitcoins */
class ShoppingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ShoppingDialog(QWidget* parent = 0);
    ~ShoppingDialog();

    void setClientModel(ClientModel* clientModel);
    void setModel(WalletModel* model);

public slots:
    void setBalance(const CAmount& balance, const CAmount& unconfirmedBalance, const CAmount& immatureBalance, const CAmount& watchOnlyBalance, const CAmount& watchUnconfBalance, const CAmount& watchImmatureBalance);
    void gotoShopUrl();

private:
    Ui::ShoppingDialog* ui;
    ClientModel* clientModel;
    WalletModel* model;

private slots:

signals:

};

#endif // BITCOIN_QT_SHOPPINGDIALOG_H
