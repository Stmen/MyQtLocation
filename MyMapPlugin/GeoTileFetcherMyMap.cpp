#include "GeoTileFetcherMyMap.h"

#include "GeoTiledMapReplyMyMap.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtLocation/private/qgeotilespec_p.h>
#include <QtLocation/private/qgeotilefetcher_p_p.h>

#include <QDebug>

QT_BEGIN_NAMESPACE

GeoTileFetcherMyMap::GeoTileFetcherMyMap(
        const QVariantMap &parameters,
        QGeoMappingManagerEngine *parent)
    : QGeoTileFetcher(parent)
    , _networkManager(new QNetworkAccessManager(this))
{
    //链接地址
    if(parameters.contains("baseUrl")){
        _baseUrl=parameters.value("baseUrl").toString();
    }
    //瓦片图片格式
    if(parameters.contains("format")){
        _format=parameters.value("format").toString();
    }
}

QGeoTiledMapReply *GeoTileFetcherMyMap::getTileImage(const QGeoTileSpec &spec)
{
    QNetworkRequest request;
    request.setRawHeader("Accept", "*/*");//设置网络请求头
    //request.setHeader(QNetworkRequest::UserAgentHeader, _userAgent);
    request.setUrl(getUrl(spec));

    qDebug()<<"GeoTileFetcherMyMap::getTileImage"<<request.url();

    QNetworkReply *reply = _networkManager->get(request);
    return new GeoTiledMapReplyMyMap(reply, spec, _format);
}

QString GeoTileFetcherMyMap::getUrl(const QGeoTileSpec &spec) const
{
    //其实小写也可以，一般系统默认是不区分大小写的
    //用的arcgis 瓦片命名格式
    const QString zz=QString("%1").arg(spec.zoom(),2,10,QLatin1Char('0')).toUpper();
    const QString yy=QString("%1").arg(spec.y(),8,16,QLatin1Char('0')).toUpper();
    const QString xx=QString("%1").arg(spec.x(),8,16,QLatin1Char('0')).toUpper();

    return QString("file:///%1/L%2/R%3/C%4.%5")
            .arg(_baseUrl)
            .arg(zz)
            .arg(yy)
            .arg(xx)
            .arg(_format);
}

QT_END_NAMESPACE
