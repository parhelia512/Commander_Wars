#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class NewsDownloader : public QObject
{
    Q_OBJECT
public:
    explicit NewsDownloader();
    virtual ~NewsDownloader() = default;

    void startDownloadNews();
signals:
    void sigNewsDownloaded(bool newNews);

private slots:
    void onResponseFinished(QNetworkReply* pReply);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
    QByteArray m_newsData;
    bool m_downloadingNews{false};

};