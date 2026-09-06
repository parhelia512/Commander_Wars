#include "objects/qrcodeactor.h"

#include <QImage>

#include "3rd_party/qrcodegen/qrcodegen.hpp"

#include "coreengine/interpreter.h"
#include "coreengine/memorymanagement.h"
#include "coreengine/gameconsole.h"

static constexpr qint32 AVERAGE_CODE_SIZE = 50;

QrCodeActor::QrCodeActor()
{
#ifdef GRAPHICSUPPORT
    setObjectName("QrCodeActor");
#endif
    Interpreter::setCppOwnerShip(this);
    m_pSprite = MemoryManagement::create<oxygine::Sprite>();
    m_pSprite->setVisible(false);
    addChild(m_pSprite);
    setSize(getImageSize(AVERAGE_CODE_SIZE), getImageSize(AVERAGE_CODE_SIZE));
}

qint32 QrCodeActor::getImageSize(qint32 modules) const
{
    const qint32 imageSize = (modules + 2 * m_quietZoneModules) * m_pixelPerModule;
    return imageSize;
}

void QrCodeActor::setQrData(const QString & data)
{
    m_data = data;
    if (data.isEmpty())
    {
        m_pSprite->setVisible(false);
        return;
    }
    CONSOLE_PRINT("Generating QR code size: " + QString::number(data.size()) + " bytes", GameConsole::eDEBUG);
    CONSOLE_PRINT("Generating QR code for data: " + data, GameConsole::eDEBUG);
    // medium error correction: enough payload for otpauth urls and robust scanning
    const qrcodegen::QrCode qrCode = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::MEDIUM);
    const qint32 modules = qrCode.getSize();
    auto imageSize = getImageSize(modules);
    QImage image(imageSize, imageSize, QImage::Format_RGBA8888);
    image.fill(Qt::white);
    CONSOLE_PRINT("Qr Code Image size: " + QString::number(imageSize) + "x" + QString::number(imageSize) + " with modules: " + QString::number(modules), GameConsole::eDEBUG);
    for (qint32 y = 0; y < modules; ++y)
    {
        for (qint32 x = 0; x < modules; ++x)
        {
            if (qrCode.getModule(x, y))
            {
                const qint32 startX = (x + m_quietZoneModules) * m_pixelPerModule;
                const qint32 startY = (y + m_quietZoneModules) * m_pixelPerModule;
                for (qint32 dy = 0; dy < m_pixelPerModule; ++dy)
                {
                    uchar* scanline = image.scanLine(startY + dy);
                    for (qint32 dx = 0; dx < m_pixelPerModule; ++dx)
                    {
                        uchar* pixel = scanline + (startX + dx) * 4;
                        pixel[0] = 0;
                        pixel[1] = 0;
                        pixel[2] = 0;
                        pixel[3] = 255;
                    }
                }
            }
        }
    }
    m_pResAnim = MemoryManagement::create<oxygine::SingleResAnim>();
    // nearest filter keeps the code crisp and scannable
    Mainapp::getInstance()->loadResAnim(m_pResAnim, image, 1, 1, 1.0f, true);
    m_pSprite->setResAnim(m_pResAnim.get());
    m_pSprite->setSize(imageSize, imageSize);
    m_pSprite->setVisible(true);
    setSize(imageSize, imageSize);
}

qint32 QrCodeActor::getQrSize() const
{
    if (m_pSprite->getVisible() && m_pResAnim.get() != nullptr)
    {
        return static_cast<qint32>(m_pSprite->getWidth());
    }
    return 0;
}

void QrCodeActor::setPixelPerModule(qint32 pixelPerModule)
{
    m_pixelPerModule = pixelPerModule;
    setSize(getImageSize(AVERAGE_CODE_SIZE), getImageSize(AVERAGE_CODE_SIZE));
    setQrData(m_data);
}
qint32 QrCodeActor::getPixelPerModule() const
{
    return m_pixelPerModule;
}
void QrCodeActor::setQuietZoneModules(qint32 quietZoneModules)
{
    m_quietZoneModules = quietZoneModules;
    setSize(getImageSize(AVERAGE_CODE_SIZE), getImageSize(AVERAGE_CODE_SIZE));
    setQrData(m_data);
}
qint32 QrCodeActor::getQuietZoneModules() const
{
    return m_quietZoneModules;
}