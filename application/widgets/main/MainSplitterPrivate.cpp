#include "MainSplitterPrivate.h"

#include "MsgModel.h"

MainSplitterPrivate::MainSplitterPrivate(MainSplitter *parent) :
    q_ptr(parent)
{

}

void MainSplitterPrivate::qDealWithData(const int &nType, const QString &sValue)
{
    if (nType == MSG_WIDGET_THUMBNAILS_VIEW) {
        setThumbnailState(sValue);
    } else if (nType == MSG_LEFTBAR_STATE) {
        SetLeftWidgetIndex(sValue);
    } else if (nType == MSG_VIEWCHANGE_DOUBLE_SHOW) {
        OnSetViewChange(sValue);
    } else if (nType == MSG_VIEWCHANGE_ROTATE) {
        OnSetViewRotate(sValue);
    } else if (nType == MSG_FILE_SCALE) {
        OnSetViewScale(sValue);
    } else if (nType == MSG_VIEWCHANGE_FIT) {
        OnSetViewHit(sValue);
    } else if (nType == MSG_FILE_PAGE_CHANGE) {
        OnSetCurPage(sValue);
    }
}

QString MainSplitterPrivate::qGetPath() const
{
    return m_strPath;
}

void MainSplitterPrivate::qSetPath(const QString &strPath)
{
    m_strPath = strPath;
}

void MainSplitterPrivate::setThumbnailState(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(Thumbnail, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::SetLeftWidgetIndex(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(LeftIndex, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::OnSetViewChange(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(DoubleShow, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::OnSetViewScale(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(Scale, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::OnSetViewRotate(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(Rotate, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::OnSetViewHit(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(Fit, sValue.toInt());

    qSetFileData(fdm);
}

void MainSplitterPrivate::OnSetCurPage(const QString &sValue)
{
    FileDataModel fdm = qGetFileData();
    fdm.qSetData(CurPage, sValue.toInt());

    qSetFileData(fdm);
}

FileDataModel MainSplitterPrivate::qGetFileData() const
{
    return m_pFileDataModel;
}

void MainSplitterPrivate::qSetFileData(const FileDataModel &fd)
{
    m_pFileDataModel = fd;
}

void MainSplitterPrivate::qSetFileChange(const int &nState)
{
    m_pChangeState = nState;
}

int MainSplitterPrivate::qGetFileChange()
{
    return m_pChangeState;
}