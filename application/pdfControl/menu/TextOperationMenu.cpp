#include "TextOperationMenu.h"

#include "ColorWidgetAction.h"
#include "TitleMenu.h"

#include "business/AppInfo.h"

#include "gof/bridge/IHelper.h"
#include "CentralDocPage.h"

TextOperationMenu::TextOperationMenu(DWidget *parent)
    : CustomMenu(TEXT_OPERATION_MENU, parent)
{
    initActions();
}

void TextOperationMenu::execMenu(const QPoint &showPoint, const bool &bHigh, const QString &sSelectText, const QString &sUuid)
{
    CentralDocPage *pMtwe = CentralDocPage::Instance();
    QString sCurPath = pMtwe->qGetCurPath();

    QList<int> pageList = dApp->m_pDBService->getBookMarkList(sCurPath);

    bool bBookState = pageList.contains(m_nClickPage);

    if (bBookState) {
        m_pAddBookMark->setProperty("data", 0);
        m_pAddBookMark->setText(tr("Remove bookmark"));
    } else {
        m_pAddBookMark->setProperty("data", 1);
        m_pAddBookMark->setText(tr("Add bookmark"));
    }

    m_strSelectText = sSelectText;

    if (m_nType == NOTE_ICON) {
        m_pRemoveHighLight->setText(tr("Remove annotation"));
        disconnect(m_pColorWidgetAction, SIGNAL(sigBtnGroupClicked(const int &)), this, SLOT(slotSetHighLight(const int &)));
        removeAction(m_pColorWidgetAction);
        // m_pColorWidgetAction->setVisible(false);
    } else if (m_nType == NOTE_HIGHLIGHT) {
        m_pRemoveHighLight->setText(tr("Remove highlight"));
        insertAction(m_pRemoveHighLight, m_pColorWidgetAction);
        connect(m_pColorWidgetAction, SIGNAL(sigBtnGroupClicked(const int &)), this, SLOT(slotSetHighLight(const int &)));
        //  m_pColorWidgetAction->setVisible(true);
    }
    m_strNoteUuid = sUuid;
    qDebug() << bHigh;
    m_pColorWidgetAction->setBtnAddLightState(bHigh);

    //  当前显示状态状态
//    int nState = dApp->m_pAppInfo->qGetCurShowState();
//    if (nState == FILE_FULLSCREEN) {
//        m_pExitFullScreen->setVisible(true);
//    } else {
//        m_pExitFullScreen->setVisible(false);
//    }

    this->exec(showPoint);
}

void TextOperationMenu::setClickPoint(const QPoint &clickPoint)
{
    m_pClickPoint = clickPoint;
}

void TextOperationMenu::setClickPage(int nClickPage)
{
    m_nClickPage = nClickPage;
}

void TextOperationMenu::initActions()
{
    m_pCopy = createAction(tr("Copy"), SLOT(slotCopyClicked()));

    m_pColorWidgetAction = new ColorWidgetAction(this);
    connect(m_pColorWidgetAction, SIGNAL(sigBtnGroupClicked(const int &)), this, SLOT(slotSetHighLight(const int &)));
    this->addAction(m_pColorWidgetAction);
    this->addSeparator();

    m_pRemoveHighLight = createAction(tr("Remove highlight"), SLOT(slotRemoveHighLightClicked()));
    m_pAddNote = createAction(tr("Add note"), SLOT(slotAddNoteClicked()));

    m_pAddBookMark = createAction(tr("Add bookmark"), SLOT(slotAddBookMarkClicked()));


//    m_pExitFullScreen = createAction(tr("Exit fullscreen"), SLOT(slotExitFullScreenClicked()));
}

QAction *TextOperationMenu::createAction(const QString &text, const char *member)
{
    auto action = new  QAction(text, this);
    connect(action, SIGNAL(triggered()), member);
    this->addAction(action);
    this->addSeparator();
    return action;
}

void TextOperationMenu::notifyMsgToFrame(const int &msgType, const QString &msgContent)
{
    dApp->m_pModelService->notifyMsg(msgType, msgContent);
    this->hide();
}

void TextOperationMenu::slotSetHighLight(const int &nColor)
{
    qDebug() << __FUNCTION__ << "**++";
    m_pLightColor = nColor;

    bool bEnable = m_pRemoveHighLight->isEnabled();
    if (bEnable) {  //  移除高亮可以点,说明已有高亮, 点击操作 是 更新高亮
        QString sContent = QString::number(m_pLightColor) + Constant::sQStringSep +
                           m_strNoteUuid + Constant::sQStringSep +
                           QString::number(m_nClickPage);
        emit sigActionTrigger(MSG_NOTE_UPDATE_HIGHLIGHT_COLOR, sContent);
    } else {    //  移除高亮不可点,说明没有高亮, 点击操作 是 添加高亮
        int nSx = m_pStartPoint.x();
        int nSy = m_pStartPoint.y();

        int nEx = m_pEndPoint.x();
        int nEy = m_pEndPoint.y();

        QString sContent = QString::number(nSx) + Constant::sQStringSep +
                           QString::number(nSy) + Constant::sQStringSep +
                           QString::number(nEx) + Constant::sQStringSep +
                           QString::number(nEy) + Constant::sQStringSep +
                           QString::number(m_pLightColor);

        emit sigActionTrigger(MSG_NOTE_ADD_HIGHLIGHT_COLOR, sContent);
    }
    this->hide();
}

void TextOperationMenu::slotCopyClicked()
{
    Utils::copyText(m_strSelectText);
}

void TextOperationMenu::slotRemoveHighLightClicked()
{
    QString sContent = QString::number(m_pClickPoint.x()) + Constant::sQStringSep +  QString::number(m_pClickPoint.y());
    emit sigActionTrigger(MSG_NOTE_REMOVE_HIGHLIGHT, sContent);
}

void TextOperationMenu::slotAddNoteClicked()
{
    if (m_strNoteUuid == "") {
        QString msgContent = QString("%1").arg(m_nClickPage) + Constant::sQStringSep + QString("%1").arg(m_pClickPoint.x()) + Constant::sQStringSep + QString("%1").arg(m_pClickPoint.y());
        emit sigActionTrigger(MSG_OPERATION_TEXT_ADD_ANNOTATION, msgContent);
    } else {
        QString msgContent = m_strNoteUuid.trimmed() + Constant::sQStringSep + QString::number(m_nClickPage);
        emit sigActionTrigger(MSG_OPERATION_TEXT_SHOW_NOTEWIDGET, msgContent);
    }
}

void TextOperationMenu::slotAddBookMarkClicked()
{
    int nData = m_pAddBookMark->property("data").toInt();
    if (nData == 0) {
        emit sigActionTrigger(MSG_OPERATION_DELETE_BOOKMARK, QString("%1").arg(m_nClickPage));
    } else {
        emit sigActionTrigger(MSG_OPERATION_ADD_BOOKMARK, QString("%1").arg(m_nClickPage));
    }
}

void TextOperationMenu::slotExitFullScreenClicked()
{
    notifyMsgToFrame(MSG_NOTIFY_KEY_MSG, KeyStr::g_esc);
}

void TextOperationMenu::setPEndPoint(const QPoint &pEndPoint)
{
    m_pEndPoint = pEndPoint;
}

void TextOperationMenu::setRemoveEnabled(bool enable)
{
    m_pRemoveHighLight->setEnabled(enable);
}

void TextOperationMenu::setPStartPoint(const QPoint &pStartPoint)
{
    m_pStartPoint = pStartPoint;
}