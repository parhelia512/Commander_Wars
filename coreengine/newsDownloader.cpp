#include "coreengine/newsDownloader.h"
#include "coreengine/settings.h"

NewsDownloader::NewsDownloader()
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &NewsDownloader::onResponseFinished, Qt::QueuedConnection);
}

void NewsDownloader::startDownloadNews()
{
    if (!Settings::getInstance()->getNewsDownloaded())
    {
        QUrl requestUrl(Settings::getInstance()->getNewsUrl());
        QNetworkRequest request(requestUrl);
        m_reply = m_webCtrl.get(request);
        connect(m_reply, &QNetworkReply::downloadProgress, this, &NewsDownloader::downloadProgress);
        m_downloadingNews = true;        
    }
}

void NewsDownloader::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0 && bytesReceived > 0)
    {
        m_newsData.append(m_reply->readAll());
    }
}

void NewsDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    if (m_downloadingNews && pReply->error() == QNetworkReply::NoError)
    {
        m_downloadingNews = false;
        m_newsData.append(result);
        QString newsHash = QString::fromUtf8(QCryptographicHash::hash(m_newsData, QCryptographicHash::Sha256).toHex());
        bool newNews = false;
        if (newsHash != Settings::getInstance()->getLastNewsHash())
        {
            Settings::getInstance()->setLastNewsHash(newsHash);
            newNews = true;
        }
        Settings::getInstance()->setLastNews(QString::fromUtf8(result));
        Settings::getInstance()->setNewsDownloaded(true);
        emit sigNewsDownloaded(newNews);
    }
    pReply->deleteLater();
}