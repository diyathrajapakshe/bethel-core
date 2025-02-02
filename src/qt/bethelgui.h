// Copyright (c) 2011-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BETHEL_QT_BETHELGUI_H
#define BETHEL_QT_BETHELGUI_H

#if defined(HAVE_CONFIG_H)
#include "config/bethel-config.h"
#endif

#include "amount.h"

#include <QLabel>
#include <QMainWindow>
#include <QEvent>
#include <QHoverEvent>
#include <QMap>
#include <QMenu>
#include <QPoint>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QtNetwork>
#include <QAbstractButton>
#include <QPainter>

class ClientModel;
class NetworkStyle;
class Notificator;
class OptionsModel;
class PlatformStyle;
class RPCConsole;
class SendCoinsRecipient;
class UnitDisplayStatusBarControl;
class WalletFrame;
class WalletModel;
class HelpMessageDialog;

class CWallet;

QT_BEGIN_NAMESPACE
class QAction;
class QProgressBar;
class QProgressDialog;
class QAbstractButton;
QT_END_NAMESPACE

/**
  Bethel GUI main class. This class represents the main window of the Bethel UI. It communicates with both the client and
  wallet models to give the user an up-to-date view of the current core state.
*/
class BethelGUI : public QMainWindow
{
    Q_OBJECT

public:
    static const QString DEFAULT_WALLET;
    static const std::string DEFAULT_UIPLATFORM;

    explicit BethelGUI(const PlatformStyle *platformStyle, const NetworkStyle *networkStyle, QWidget *parent = 0);
    ~BethelGUI();

    /** Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel *clientModel);
    void showVotingDialog();

#ifdef ENABLE_WALLET
    /** Set the wallet model.
        The wallet model represents a bethel wallet, and offers access to the list of transactions, address book and sending
        functionality.
    */
    bool addWallet(const QString& name, WalletModel *walletModel);
    bool setCurrentWallet(const QString& name);
    void removeAllWallets();
#endif // ENABLE_WALLET
    bool enableWallet;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void showEvent(QShowEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

private:
    ClientModel *clientModel;
    WalletFrame *walletFrame;

    UnitDisplayStatusBarControl *unitDisplayControl;
    QLabel *labelEncryptionIcon;
    QLabel *labelConnectionsIcon;
    QLabel *labelBlocksIcon;
    QLabel *labelStakingIcon;
    QLabel *labelPrice;
    QLabel *progressBarLabel;
    QProgressBar *progressBar;
    QProgressDialog *progressDialog;

    QMenuBar *appMenuBar;
    QAction *overviewAction;
    QAction *historyAction;
    QAction *quitAction;
    QAction *sendCoinsAction;
    QAction *sendCoinsMenuAction;
    QAction *usedSendingAddressesAction;
    QAction *usedReceivingAddressesAction;
    QAction *repairWalletAction;
    QAction *importPrivateKeyAction;
    QAction *bootstrapBlockchainAction;
    QAction *exportMasterPrivateKeyAction;
    QAction *signMessageAction;
    QAction *verifyMessageAction;
    QAction *aboutAction;
    QAction *infoAction;
    QAction *receiveCoinsAction;
    QAction *receiveCoinsMenuAction;
    QAction *optionsAction;
    QAction *cfundProposalsAction;
    QAction *cfundPaymentRequestsAction;
    QAction *toggleHideAction;
    QAction *encryptWalletAction;
    QAction *backupWalletAction;
    QAction *changePassphraseAction;
    QAction *aboutQtAction;
    QAction *openRPCConsoleAction;
    QAction *openAction;
    QAction *showHelpMessageAction;
    QAction *unlockWalletAction;
    QAction *lockWalletAction;
    QAction *toggleStakingAction;
    QPushButton *topMenu1; // Home
    QPushButton *topMenu2; // Send
    QPushButton *topMenu3; // Recieve
    QPushButton *topMenu4; // Transaction History
    QPushButton *topMenu5; // Community Fund

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    Notificator *notificator;
    RPCConsole *rpcConsole;
    HelpMessageDialog *helpMessageDialog;

    /** Keep track of previous number of blocks, to detect progress */
    int prevBlocks;
    int spinnerFrame;

    bool fDontShowAgain;
    int64_t lastDialogShown;

    uint64_t nWeight;

    const PlatformStyle *platformStyle;

    QAction *updatePriceAction;
    bool fShowingVoting;

    /** Create the main UI actions. */
    void createActions();
    /** Create the menu bar and sub-menus. */
    void createMenuBar();
    /** Create the toolbars */
    void createToolBars();
    /** Create system tray icon and notification */
    void createTrayIcon(const NetworkStyle *networkStyle);
    /** Create system tray menu (or setup the dock menu) */
    void createTrayIconMenu();

    /** Enable or disable all wallet-related actions */
    void setWalletActionsEnabled(bool enabled);

    /** Connect core signals to GUI client */
    void subscribeToCoreSignals();
    /** Disconnect core signals from GUI client */
    void unsubscribeFromCoreSignals();


    void updateWeight();

    void cfundProposalsOpen(bool fMode);



Q_SIGNALS:
    /** Signal raised when a URI was entered or dragged to the GUI */
    void receivedURI(const QString &uri);

public Q_SLOTS:
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set number of blocks and last block date shown in the UI */
    void setNumBlocks(int count, const QDateTime& blockDate, double nVerificationProgress, bool headers);

    /** Notify the user of an event from the core network or transaction handling code.
       @param[in] title     the message box / notification title
       @param[in] message   the displayed text
       @param[in] style     modality and style definitions (icon and used buttons - buttons only for message boxes)
                            @see CClientUIInterface::MessageBoxFlags
       @param[in] ret       pointer to a bool that will be modified to whether Ok was clicked (modal only)
    */
    void message(const QString &title, const QString &message, unsigned int style, bool *ret = NULL);
    void replyFinished(QNetworkReply *reply);
    void replyVotingFinished(QNetworkReply *reply);
    void startVotingCounter();

#ifdef ENABLE_WALLET
    /** Set the encryption status as shown in the UI.
       @param[in] status            current encryption status
       @see WalletModel::EncryptionStatus
    */
    void setEncryptionStatus(int status);

    bool handlePaymentRequest(const SendCoinsRecipient& recipient);

    /** Show incoming transaction notification for new transactions. */
    void incomingTransaction(const QString& date, int unit, const CAmount& amount, const QString& type, const QString& address, const QString& label);
    void onCurrencySelection(QAction* action);
#endif // ENABLE_WALLET

private Q_SLOTS:
#ifdef ENABLE_WALLET
    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
    /** Switch to community fund page*/
    void gotoCommunityFundPage();
    /** Switch to receive coins page */
    void gotoReceiveCoinsPage();
    /** Switch to send coins page */
    void gotoSendCoinsPage(QString addr = "");
    void gotoRequestPaymentPage();

    /** Show Sign/Verify Message dialog and switch to sign message tab */
    void gotoSignMessageTab(QString addr = "");
    /** Show Sign/Verify Message dialog and switch to verify message tab */
    void gotoVerifyMessageTab(QString addr = "");

    /** Show open dialog */
    void openClicked();
    /** Update Staking status **/
    void updateStakingStatus();
    /** Fetch Price from CMC **/
    void updatePrice();

    void getVotingInfo();

    /** Repairs wallet **/
    void repairWallet();

#endif // ENABLE_WALLET
    /** Show configuration dialog */
    void optionsClicked();
    /** Community Fund related */
    void cfundProposalsClicked();
    void cfundPaymentRequestsClicked();
    /** Show about dialog */
    void aboutClicked();
    /** Open Bethel Knowledge base */
    void infoClicked();
    /** Show debug window */
    void showDebugWindow();
    /** Show debug window and set focus to the console */
    void showDebugWindowActivateConsole();
    /** Show help message dialog */
    void showHelpMessageClicked();
    /** Toggle Staking **/
    void toggleStaking();
    /** Bootstrap blockchain **/
    void bootstrapBlockchain();
#ifndef Q_OS_MAC
    /** Handle tray icon clicked */
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
#endif

    /** Show window if hidden, unminimize when minimized, rise when obscured or show if hidden and fToggleHidden is true */
    void showNormalIfMinimized(bool fToggleHidden = false);
    /** Simply calls showNormalIfMinimized(true) for use in SLOT() macro */
    void toggleHidden();

    /** called by a timer to check if fRequestShutdown has been set **/
    void detectShutdown();

    /** Show progress dialog e.g. for verifychain */
    void showProgress(const QString &title, int nProgress);

    /** When hideTrayIcon setting is changed in OptionsModel hide or show the icon accordingly. */
    void setTrayIconVisible(bool);
};

class UnitDisplayStatusBarControl : public QLabel
{
    Q_OBJECT

public:
    explicit UnitDisplayStatusBarControl(const PlatformStyle *platformStyle);
    /** Lets the control know about the Options Model (and its signals) */
    void setOptionsModel(OptionsModel *optionsModel);

protected:
    /** So that it responds to left-button clicks */
    void mousePressEvent(QMouseEvent *event);

private:
    OptionsModel *optionsModel;
    QMenu* menu;

    /** Shows context menu with Display Unit options by the mouse coordinates */
    void onDisplayUnitsClicked(const QPoint& point);
    /** Creates context menu, its actions, and wires up all the relevant signals for mouse events. */
    void createContextMenu();

private Q_SLOTS:
    /** When Display Units are changed on OptionsModel it will refresh the display text of the control on the status bar */
    void updateDisplayUnit(int newUnits);
    /** Tells underlying optionsModel to update its current display unit. */
    void onMenuSelection(QAction* action);
};

#endif // BETHEL_QT_BETHELGUI_H
