/****************************************************************************
**
** Copyright (C) VCreate Logic Private Limited, Bangalore
**
** Use of this file is limited according to the terms specified by
** VCreate Logic Private Limited, Bangalore.  Details of those terms
** are listed in licence.txt included as part of the distribution package
** of this file. This file may not be distributed without including the
** licence.txt file.
**
** Contact info@vcreatelogic.com if any conditions of this licensing are
** not clear to you.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <GCF/Message>
#include <GCF/IBinaryDataSerializer>
#include <GCF/ErrorDump>

#include <QDataStream>
#include <QTextStream>
#include <QSet>

#include <QColor>
#include <QPixmap>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QString>
#include <QByteArray>
#include <QBitArray>
#include <QImage>
#include <QBitmap>
#include <QPointF>
#include <QPoint>
#include <QLineF>
#include <QLine>
#include <QSizeF>
#include <QSize>
#include <QRectF>
#include <QRect>
#include <QMatrix>
#include <QTransform>
#include <QUrl>
#include <QSizePolicy>
#include <QBrush>
#include <QList>
#include <QMap>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

static const char GHexChars[] = "0123456789abcdef";
char CharIndex(const char ch)
{
    for(int i=0; i<16; i++)
        if(GHexChars[i] == ch)
            return (char)i;

    return (char)16;
}

bool GCF::Message::encodeVariant(QDataStream& ds, const QVariant& value)
{
    QString typeName;

    if( value.isValid() )
    {
        typeName = QString(value.typeName());
        if( typeName.isEmpty() )
            return false;
    }
    else
        typeName = "Invalid";

    // Serialize value on to the byte array
    switch( value.type() )
    {
    case QVariant::Invalid:
        ds << typeName;
        ds << qint32(0);
        break;
    case QVariant::Bool:
        ds << typeName;
        ds << value.toBool();
        break;
    case QVariant::Char:
        ds << typeName;
        ds << value.toChar();
        break;
    case QVariant::Color:
        ds << typeName;
        ds << value.value<QColor>();
        break;
    case QVariant::Date:
        ds << typeName;
        ds << value.toDate();
        break;
    case QVariant::DateTime:
        ds << typeName;
        ds << value.toDateTime();
        break;
    case QVariant::Double:
        ds << typeName;
        ds << value.toDouble();
        break;
    case QVariant::Int:
        ds << typeName;
        ds << value.toString();
        break;
    case QVariant::Time:
        ds << typeName;
        ds << value.toTime();
        break;
    case QVariant::UInt:
        ds << typeName;
        ds << value.toString();
    case QVariant::String:
        ds << typeName;
        ds << value.toString();
        break;
    case QVariant::Pixmap:
        ds << typeName;
        ds << value.value<QPixmap>();
        break;
    case QVariant::ByteArray:
        ds << typeName;
        ds << value.toByteArray();
        break;
    case QVariant::BitArray:
        ds << typeName;
        ds << value.toBitArray();
        break;
    case QVariant::Image:
        ds << typeName;
        ds << value.value<QImage>();
        break;
    case QVariant::Url:
        ds << typeName;
        ds << value.toUrl();
        break;
    case QVariant::StringList:
        ds << typeName;
        ds << value.toStringList();
        break;
    case QVariant::SizePolicy:
        ds << typeName;
        ds << value.value<QSizePolicy>();
        break;
    case QVariant::SizeF:
        ds << typeName;
        ds << value.toSizeF();
        break;
    case QVariant::Size:
        ds << typeName;
        ds << value.toSize();
        break;
    case QVariant::RegExp:
        ds << typeName;
        ds << value.toRegExp();
        break;
    case QVariant::RectF:
        ds << typeName;
        ds << value.toRectF();
        break;
    case QVariant::Rect:
        ds << typeName;
        ds << value.toRect();
        break;
    case QVariant::Polygon:
        ds << typeName;
        ds << value.value<QPolygon>();
        break;
    case QVariant::PointF:
        ds << typeName;
        ds << value.toPointF();
        break;
    case QVariant::Point:
        ds << typeName;
        ds << value.toPoint();
        break;
    case QVariant::Matrix:
        ds << typeName;
        ds << value.value<QMatrix>();
        break;
    case QVariant::LineF:
        ds << typeName;
        ds << value.toLineF();
        break;
    case QVariant::Line:
        ds << typeName;
        ds << value.toLine();
        break;
    case QVariant::Brush:
        ds << typeName;
        ds << value.value<QBrush>();
        break;
    case QVariant::Bitmap:
        ds << typeName;
        ds << value.value<QBitmap>();
        break;
    case QVariant::Transform:
        ds << typeName;
        ds << value.value<QTransform>();
        break;
    case QVariant::List: {
        ds << typeName;
        QList<QVariant> list = value.toList();
        ds << (qint32)list.count();
        Q_FOREACH(QVariant item, list)
            GCF::Message::encodeVariant(ds, item);
        } break;
    case QVariant::Map: {
        ds << typeName;
        QMap<QString,QVariant> map = value.toMap();
        ds << (qint32)map.count();
        QMap<QString,QVariant>::const_iterator it = map.constBegin();
        QMap<QString,QVariant>::const_iterator end = map.constEnd();
        while(it != end) {
            ds << it.key();
            GCF::Message::encodeVariant(ds, it.value());
            ++it;
        }
        } break;
    case QVariant::LongLong: {
        ds << typeName;
        ds << value.toLongLong();
        } break;
    case QVariant::ULongLong:{
        ds << typeName;
        ds << value.toULongLong();
        } break;
    case QVariant::Vector2D: {
        ds << typeName;
        ds << value.value< QVector2D >();
        } break;
    case QVariant::Vector3D: {
        ds << typeName;
        ds << value.value< QVector3D >();
        } break;
    case QVariant::Vector4D: {
        ds << typeName;
        ds << value.value< QVector4D >();
        } break;
    default: {
        QByteArray typeNameBA = typeName.toLatin1();
        IBinaryDataSerializer* serializer = GCF::Message::findSerializer(typeNameBA);
        if(serializer)
        {
            QByteArray binary;
            if( serializer->toBinary(value, binary) )
            {
                ds << typeName;
                ds << binary;
                break;
            }
        }
        }
        return false;
    }

    return true;
}

bool GCF::Message::extractVariant(QDataStream& ds, QVariant& value)
{
    QString typeName;

    // First figure out the typename
    ds >> typeName;
    QVariant::Type type = QVariant::Invalid;

    if( typeName != "Invalid" )
        type = QVariant::nameToType( qPrintable(typeName) );

    // Serialize value on to the byte array
    switch( type )
    {
    case QVariant::Bool: {
        bool val;
        ds >> val;
        value = val;
        } break;
    case QVariant::Char: {
        QChar ch;
        ds >> ch;
        value = ch;
        } break;
    case QVariant::Color: {
        QColor color;
        ds >> color;
        value = color;
        } break;
    case QVariant::Date: {
        QDate date;
        ds >> date;
        value = date;
        } break;
    case QVariant::DateTime: {
        QDateTime dateTime;
        ds >> dateTime;
        value = dateTime;
        } break;
    case QVariant::Double: {
        double val;
        ds >> val;
        value = val;
        } break;
    case QVariant::Int: {
        QString valStr;
        ds >> valStr;
        value = valStr.toInt();
        } break;
    case QVariant::Time: {
        QTime time;
        ds >> time;
        value = time;
        } break;
    case QVariant::UInt: {
        QString valStr;
        ds >> valStr;
        value = valStr.toUInt();
        } break;
    case QVariant::String: {
        QString val;
        ds >> val;
        value = val;
        } break;
    case QVariant::Pixmap: {
        QPixmap pm;
        ds >> pm;
        value = qVariantFromValue<QPixmap>(pm);
        } break;
    case QVariant::ByteArray: {
        QByteArray ba;
        ds >> ba;
        value = ba;
        } break;
    case QVariant::BitArray: {
        QBitArray ba;
        ds >> ba;
        value = ba;
        } break;
    case QVariant::Image: {
        QImage image;
        ds >> image;
        value = image;
        } break;
    case QVariant::Url: {
        QUrl url;
        ds >> url;
        value = url;
        } break;
    case QVariant::StringList: {
        QStringList stringList;
        ds >> stringList;
        value = stringList;
        } break;
    case QVariant::SizePolicy: {
        QSizePolicy sizePolicy;
        ds >> sizePolicy;
        value = sizePolicy;
        } break;
    case QVariant::SizeF: {
        QSizeF size;
        ds >> size;
        value = size;
        } break;
    case QVariant::Size: {
        QSize size;
        ds >> size;
        value = size;
        } break;
    case QVariant::RegExp: {
        QRegExp regExp;
        ds >> regExp;
        value = regExp;
        } break;
    case QVariant::RectF: {
        QRectF rect;
        ds >> rect;
        value = rect;
        } break;
    case QVariant::Rect: {
        QRect rect;
        ds >> rect;
        value = rect;
        } break;
    case QVariant::Polygon: {
        QPolygon polygon;
        ds >> polygon;
        value = polygon;
        } break;
    case QVariant::PointF: {
        QPointF point;
        ds >> point;
        value = point;
        } break;
    case QVariant::Point: {
        QPoint point;
        ds >> point;
        value = point;
        } break;
    case QVariant::Matrix: {
        QMatrix matrix;
        ds >> matrix;
        value = matrix;
        } break;
    case QVariant::LineF: {
        QLineF line;
        ds >> line;
        value = line;
        } break;
    case QVariant::Line: {
        QLine line;
        ds >> line;
        value = line;
        } break;
    case QVariant::Brush: {
        QBrush brush;
        ds >> brush;
        value = brush;
        } break;
    case QVariant::Bitmap: {
        QBitmap bitmap;
        ds >> bitmap;
        value = bitmap;
        } break;
    case QVariant::Transform: {
        QTransform tr;
        ds >> tr;
        value = tr;
        } break;
    case QVariant::List: {
        qint32 count = 0;
        ds >> count;
        QList<QVariant> list;
        for(int i=0; i<count; i++) {
            QVariant item;
            GCF::Message::extractVariant(ds, item);
            list.append(item);
        }
        value = list;
        } break;
    case QVariant::Map: {
        qint32 count = 0;
        ds >> count;
        QMap<QString,QVariant> map;
        for(int i=0; i<count; i++) {
            QString key;
            QVariant val;
            ds >> key;
            GCF::Message::extractVariant(ds, val);
            map[key] = val;
        }
        value = map;
        } break;
    case QVariant::LongLong: {
        qlonglong val = 0;
        ds >> val;
        value = val;
        } break;
    case QVariant::ULongLong: {
        qulonglong val = 0;
        ds >> val;
        value = val;
        } break;
    case QVariant::Vector2D: {
        QVector2D vec;
        ds >> vec;
        value = QVariant::fromValue(vec);
        } break;
    case QVariant::Vector3D: {
        QVector3D vec;
        ds >> vec;
        value = QVariant::fromValue(vec);
        } break;
    case QVariant::Vector4D: {
        QVector4D vec;
        ds >> vec;
        value = QVariant::fromValue(vec);
        } break;
    case QVariant::Invalid:
    default: {
        QByteArray typeNameBA = typeName.toLatin1();
        IBinaryDataSerializer* serializer = GCF::Message::findSerializer(typeNameBA);
        if(serializer)
        {
            QVariant tmpValue;
            QByteArray binary;
            ds >> binary;

            if( serializer->fromBinary(binary, tmpValue) )
            {
                value = tmpValue;
                break;
            }
        }
        else
        {
            qint32 val;
            ds >> val;
            value = QVariant();
        }
        }
        return false;
    }

    return true;
}

bool GCF::Message::encodeVariantList(QDataStream& ds, const QVariantList& list)
{
    bool success = true;
    ds << (qint32)list.count();
    for(int i=0; i<list.count(); i++)
    {
        if(!list[i].isValid())
            break;

        success &= encodeVariant(ds, list[i]);
    }

    return success;
}

bool GCF::Message::extractVariantList(QDataStream& ds, QVariantList& list)
{
    bool success = true;
    qint32 count = 0;

    ds >> count;
    for(int i=0; i<count; i++)
    {
        QVariant value;
        success &= extractVariant(ds, value);
        list << value;
    }

    return success;
}

QString GCF::Message::byteArrayToString(const QByteArray& ba)
{
    QString baText;
    QTextStream ts( &baText );
    for(int i=0; i<ba.size(); i++)
    {
        uchar s = (uchar) ba[i];
        ts << GHexChars[s >> 4];
        ts << GHexChars[s & 0x0f];
    }

    return baText;
}

QByteArray GCF::Message::stringToByteArray(const QString& str)
{
    const QString& baText = str;
    int baIndex = 0;
    int baLength = str.length()/2;
    QByteArray ba(baLength, 0);

    for(int i=0; i<baText.length(); i+=2)
    {
        unsigned char baChar = 0;

        // First character
        const char c1 = baText[i].toAscii();
        baChar = CharIndex(c1) << 4;

        // Second character
        const char c2 = baText[i+1].toAscii();
        baChar = baChar + CharIndex(c2);

        ba.data()[baIndex++] = baChar;
    }

    return ba;
}

// Global list of binary serializers
static QMap<QByteArray, IBinaryDataSerializer*> BinarySerializerMap;

void GCF::Message::registerSerializer(IBinaryDataSerializer* serializer)
{
    if(!serializer)
        return;

    if(BinarySerializerMap.contains(serializer->dataType()))
        return;

    BinarySerializerMap[serializer->dataType()] = serializer;
}

void GCF::Message::unregisterSerializer(IBinaryDataSerializer* serializer)
{
    if(!serializer)
        return;

    if( BinarySerializerMap.contains(serializer->dataType()) &&
        BinarySerializerMap[serializer->dataType()] == serializer)
    {
        BinarySerializerMap.remove( serializer->dataType() );
    }
}

IBinaryDataSerializer* GCF::Message::findSerializer(const QByteArray& typeName)
{
    if( BinarySerializerMap.contains(typeName) )
        return BinarySerializerMap[typeName];

    return 0;
}

/////////////////////////////////////////////////////////////////////////////

static qint32 GetNextMessageId()
{
    static qint32 msgId = 0;
    return msgId++;
}

class CMessageList : public QList<GCF::Message*>
{
public:
    CMessageList() { }
    ~CMessageList() {
        QList<GCF::Message*> copy = *this;
        this->clear();
        qDeleteAll(copy);
    }
};

static CMessageList& GetMessageList()
{
    static CMessageList theList;
    return theList;
}

struct GCF::MessageData
{
    MessageData() : incoming(false), messageId(-1), sessionId(-1),
        messageType(GCF::Message::InvalidMessage),
        inceptionMessage(0) { }

    GCF::DataStore customData;
    bool incoming;
    qint32 messageId;
    qint32 sessionId;
    GCF::Message::MessageType messageType;
    GCF::Message* inceptionMessage;
};

QByteArray GCF::Message::ErrorCode = "";

GCF::Message::MessageDirection GCF::Message::messageDirection(GCF::Message::MessageType type)
{
    switch(type)
    {
    case InvalidMessage: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case RequestAccess: return GCF::Message::MessageDirection(ClientToServer); break;
    case RequestAccessRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case RequestObject: return GCF::Message::MessageDirection(ClientToServer); break;
    case RequestObjectRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case GetPropertyNames: return GCF::Message::MessageDirection(ClientToServer); break;
    case GetPropertyNamesRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case SetProperty: return GCF::Message::MessageDirection(ClientToServer); break;
    case SetPropertyRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case GetProperty: return GCF::Message::MessageDirection(ClientToServer); break;
    case GetPropertyRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case InvokeMethod: return GCF::Message::MessageDirection(ClientToServer); break;
    case InvokeMethodRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case RequestConnection: return GCF::Message::MessageDirection(ClientToServer); break;
    case RequestConnectionRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case SignalDelivery: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseObject: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseObjectRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseAccess: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseAccessRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseConnection: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case ReleaseConnectionRes: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    case AsyncInvokeMethod: return GCF::Message::MessageDirection(ClientToServer); break;
    case AsyncInvokeMethodRes: return GCF::Message::MessageDirection(ServerToClient); break;
    case AsyncInvokeMethods: return GCF::Message::MessageDirection(ClientToServer); break;
    case AsyncInvokeMethodsRes: return GCF::Message::MessageDirection(ServerToClient); break;
    case UserSpecific: return GCF::Message::MessageDirection(ClientToServer|ServerToClient); break;
    }

    return GCF::Message::Both;
}

void GCF::Message::setObjectId(qint32 id)
{
    // do nothing
    Q_UNUSED(id);
}

qint32 GCF::Message::objectId() const
{
    return -1;
}

QString GCF::Message::toString() const
{
    QString ret;
    QTextStream ts(&ret);
    ts << "MessageID: " << int(d->messageId)
        << " of type " << GCF::Message::messageTypeString(d->messageType) << ". ";
    this->dumpMessageInfo(ts);

    return ret;
}

QString GCF::Message::messageTypeString(GCF::Message::MessageType type)
{
    static QMap<GCF::Message::MessageType, QString> typeNameMap;
    if( !typeNameMap.count() )
    {
#define TypeNameEntry(x) typeNameMap[GCF::Message::x] = QString(#x);

        TypeNameEntry(InvalidMessage)
        TypeNameEntry(RequestAccess)
        TypeNameEntry(RequestAccessRes)
        TypeNameEntry(RequestObject)
        TypeNameEntry(RequestObjectRes)
        TypeNameEntry(GetPropertyNames)
        TypeNameEntry(GetPropertyNamesRes)
        TypeNameEntry(SetProperty)
        TypeNameEntry(SetPropertyRes)
        TypeNameEntry(GetProperty)
        TypeNameEntry(GetPropertyRes)
        TypeNameEntry(InvokeMethod)
        TypeNameEntry(InvokeMethodRes)
        TypeNameEntry(RequestConnection)
        TypeNameEntry(RequestConnectionRes)
        TypeNameEntry(SignalDelivery)
        TypeNameEntry(ReleaseObject)
        TypeNameEntry(ReleaseObjectRes)
        TypeNameEntry(ReleaseAccess)
        TypeNameEntry(ReleaseAccessRes)
        TypeNameEntry(ReleaseConnection)
        TypeNameEntry(ReleaseConnectionRes)
        TypeNameEntry(AsyncInvokeMethod)
        TypeNameEntry(AsyncInvokeMethodRes)
        TypeNameEntry(AsyncInvokeMethods)
        TypeNameEntry(AsyncInvokeMethodsRes)

#undef TypeNameEntry
    }

    if( typeNameMap.contains(type) )
        return typeNameMap[type];

    return "UserSpecific";
}

GCF::Message::Message(qint32 id)
{
    d = new GCF::MessageData;
    d->messageId = (id < 0) ? GetNextMessageId() : id;
    d->incoming = false;
    d->messageType = InvalidMessage;

    GetMessageList().append(this);
}

GCF::Message::Message(GCF::Message* msg)
{
    d = new GCF::MessageData;
    d->messageId = (msg == 0) ? GetNextMessageId() : msg->id();
    d->incoming = false;
    d->messageType = InvalidMessage;
    d->inceptionMessage = msg;

    GetMessageList().append(this);
}

GCF::Message::~Message()
{
    GetMessageList().removeAll(this);

    delete d;
}

void GCF::Message::setIncoming(bool val)
{
    d->incoming = val;
}

void GCF::Message::setId(qint32 id)
{
    d->messageId = id;
}

void GCF::Message::setSessionId(qint32 id)
{
    d->sessionId = id;
}

void GCF::Message::setMessageType(GCF::Message::MessageType type)
{
    d->messageType = type;
}

bool GCF::Message::isValid() const
{
    return d->messageType != GCF::Message::InvalidMessage;
}

bool GCF::Message::isIncoming() const
{
    return d->incoming;
}

qint32 GCF::Message::id() const
{
    return d->messageId;
}

qint32 GCF::Message::sessionId() const
{
    return d->sessionId;
}

GCF::Message::MessageType GCF::Message::messageType() const
{
    return d->messageType;
}

GCF::Message::MessageDirection GCF::Message::messageDirection() const
{
    return GCF::Message::messageDirection( d->messageType );
}

GCF::DataStore GCF::Message::customData() const
{
    return d->customData;
}

QVariant & GCF::Message::operator [] (const QString& key)
{
    return d->customData[key];
}

bool GCF::Message::hasKey(const QString& key)
{
    return d->customData.hasKey(key);
}

void GCF::Message::removeKey(const QString& key)
{
    d->customData.removeKey(key);
}

QStringList GCF::Message::keys() const
{
    return d->customData.keys();
}

GCF::Message* GCF::Message::inceptionMessage() const
{
    return d->inceptionMessage;
}

void GCF::Message::setErrorCode(const QByteArray& errCode)
{
    d->customData["ErrorCode"] = errCode;
}

QByteArray GCF::Message::errorCode() const
{
    QByteArray errCode = d->customData.value("ErrorCode").toByteArray();
    return errCode;
}

QString GCF::Message::errorCodeAndMessage() const
{
    QByteArray errCode = d->customData.value("ErrorCode").toByteArray();
    QString errMsg = d->customData.value("ErrorMessage").toString();
    if(errMsg.isEmpty())
        return errCode;

    QString ret = QString::fromUtf8(errCode) + QString("\n\n") + errMsg;
    return ret;
}

void GCF::Message::includeErrorInfo()
{
    QByteArray bytes = d->customData.value("ErrorCode").toByteArray();
    if(bytes.isEmpty() && !GCF::Message::ErrorCode.isEmpty())
    {
        d->customData["ErrorCode"] = GCF::Message::ErrorCode;
        GCF::Message::ErrorCode.clear();
    }

    QString errorMessage = d->customData.value("ErrorMessage").toString();
    if(GCF::ErrorDump::instance().errorMessages().count())
    {
        QString errMsg = GCF::ErrorDump::instance().toString() + "\n\n" + errorMessage;
        d->customData["ErrorMessage"] = errMsg;
        GCF::ErrorDump::instance().clear();
    }
}

QByteArray GCF::Message::toByteArray() const
{
    // The byte array into which message data will be written
    QByteArray ba;

    // Use a data stream to write the packet into the byte array
    {
        QDataStream ds(&ba, QIODevice::WriteOnly);

        // First field has-to be message type
        ds << qint32( d->messageType );

        // Now write the remaining parts
        writeHeader(ds);
        writeBody(ds);
        writeFooter(ds);
    }

    // Compress the packet written so far
    ba = qCompress(ba);

    // Put the compressed packet with a length header into a new byte array
    // and return that.
    QByteArray retVal;
    {
        QDataStream ds(&retVal, QIODevice::WriteOnly);
        ds << qint32(ba.size());
        ds << ba;
    }

    // All done.
    return retVal;
}

GCF::Message* GCF::Message::fromByteArray(QByteArray& byteArray)
{
    if( !byteArray.size() )
        return 0;

    // Pick out the first packet in the byteArray
    QByteArray ba; // the byte array that contains one packet.
    {
        qint32 msgPacketSize = 0;
        QDataStream ds(byteArray);
        ds >> msgPacketSize;
        ds >> ba;
        if( ba.size() != msgPacketSize )
            return 0;

        // Now update byteArray so that another call to this function
        // can read the next packet.
        byteArray = byteArray.remove(0, 2*sizeof(qint32)+msgPacketSize);
    }

    if( !ba.size() )
        return 0;

    // Uncompress the byte array
    ba = qUncompress(ba);

    // First figure out the type
    QDataStream ds(ba);
    qint32 type;
    ds >> type;

    GCF::Message::MessageType msgType = GCF::Message::InvalidMessage;
    msgType = GCF::Message::MessageType(type);

    GCF::Message* retMsg = 0;

    switch(msgType)
    {
    case GCF::Message::InvalidMessage:
        retMsg = new GCF::Message(0);
        break;
    case GCF::Message::RequestAccess:
        retMsg = new GCF::RequestAccessMessage(0);
        break;
    case GCF::Message::RequestAccessRes:
        retMsg = new GCF::RequestAccessResMessage(0);
        break;
    case GCF::Message::RequestObject:
        retMsg = new GCF::RequestObjectMessage(0);
        break;
    case GCF::Message::RequestObjectRes:
        retMsg = new GCF::RequestObjectResMessage(0);
        break;
    case GCF::Message::GetPropertyNames:
        retMsg = new GCF::GetPropertyNamesMessage(0);
        break;
    case GCF::Message::GetPropertyNamesRes:
        retMsg = new GCF::GetPropertyNamesResMessage(0);
        break;
    case GCF::Message::SetProperty:
        retMsg = new GCF::SetPropertyMessage(0);
        break;
    case GCF::Message::SetPropertyRes:
        retMsg = new GCF::SetPropertyResMessage(0);
        break;
    case GCF::Message::GetProperty:
        retMsg = new GCF::GetPropertyMessage(0);
        break;
    case GCF::Message::GetPropertyRes:
        retMsg = new GCF::GetPropertyResMessage(0);
        break;
    case GCF::Message::InvokeMethod:
        retMsg = new GCF::InvokeMethodMessage(0);
        break;
    case GCF::Message::InvokeMethodRes:
        retMsg = new GCF::InvokeMethodResMessage(0);
        break;
    case GCF::Message::RequestConnection:
        retMsg = new GCF::RequestConnectionMessage(0);
        break;
    case GCF::Message::RequestConnectionRes:
        retMsg = new GCF::RequestConnectionResMessage(0);
        break;
    case GCF::Message::SignalDelivery:
        retMsg = new GCF::SignalDeliveryMessage(0);
        break;
    case GCF::Message::ReleaseObject:
        retMsg = new GCF::ReleaseObjectMessage(0);
        break;
    case GCF::Message::ReleaseObjectRes:
        retMsg = new GCF::ReleaseObjectResMessage(0);
        break;
    case GCF::Message::ReleaseAccess:
        retMsg = new GCF::ReleaseAccessMessage(0);
        break;
    case GCF::Message::ReleaseAccessRes:
        retMsg = new GCF::ReleaseAccessResMessage(0);
        break;
    case GCF::Message::ReleaseConnection:
        retMsg = new GCF::ReleaseConnectionMessage(0);
        break;
    case GCF::Message::ReleaseConnectionRes:
        retMsg = new GCF::ReleaseConnectionResMessage(0);
        break;
    case GCF::Message::AsyncInvokeMethod:
        retMsg = new GCF::AsyncInvokeMethodMessage(0);
        break;
    case GCF::Message::AsyncInvokeMethodRes:
        retMsg = new GCF::AsyncInvokeMethodResMessage(0);
        break;
    case GCF::Message::AsyncInvokeMethods:
        retMsg = new GCF::AsyncInvokeMethodsMessage(0);
        break;
    case GCF::Message::AsyncInvokeMethodsRes:
        retMsg = new GCF::AsyncInvokeMethodsResMessage(0);
        break;
    default:
        break;
    }

    if( !retMsg )
        return 0;

    retMsg->initFromByteArray(ba);
    retMsg->setIncoming(true);
    return retMsg;
}

// Methods that are called from toByteArray()
void GCF::Message::writeHeader(QDataStream& ds) const
{
    ds << d->messageId;
}

void GCF::Message::writeBody(QDataStream& ds) const
{
    // do nothing
    Q_UNUSED(ds);
}

void GCF::Message::writeFooter(QDataStream& ds) const
{
    QStringList keys = this->keys();
    ds << (qint32)keys.count();

    for(int i=0; i<keys.count(); i++)
    {
        QString key = keys[i];
        QVariant value = d->customData[key];

        ds << key;
        encodeVariant(ds, value);
    }

    // We have to extend the packet format from the end only!
    ds << d->sessionId;
}

// Methods that are called from fromByteArray()
void GCF::Message::initFromByteArray(const QByteArray& ba)
{
    QDataStream ds(ba);

    // Message type check
    qint32 type;
    ds >> type;
    if(type != qint32(d->messageType))
    {
        qWarning("Initializing message from an InvalidMessage byte array");
        return;
    }

    // Read other parts
    readHeader(ds);
    readBody(ds);
    readFooter(ds);
}

// Methods called from initFromByteArray()
void GCF::Message::readHeader(QDataStream& ds)
{
    ds >> d->messageId;
}

void GCF::Message::readBody(QDataStream& ds)
{
    // do nothing
    Q_UNUSED(ds);
}

void GCF::Message::readFooter(QDataStream& ds)
{
    d->customData.clear();

    qint32 count = 0;
    ds >> count;
    for(int i=0; i<count; i++)
    {
        QString key;
        QVariant value;

        ds >> key;
        extractVariant(ds, value);

        d->customData[key] = value;
    }

    ds >> d->sessionId;
}

void GCF::Message::dumpMessageInfo(QTextStream& ts) const
{
    // Do nothing
    Q_UNUSED(ts);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestAccessMessageData
{
    QString appName;
};

GCF::RequestAccessMessage::RequestAccessMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestAccessMessageData;

    setMessageType(RequestAccess);
}

GCF::RequestAccessMessage::RequestAccessMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestAccessMessageData;

    setMessageType(RequestAccess);
}

GCF::RequestAccessMessage::~RequestAccessMessage()
{
    delete d;
}

void GCF::RequestAccessMessage::setApplicationName(const QString& appName)
{
    if( isIncoming() )
        return;

    d->appName = appName;
}

QString GCF::RequestAccessMessage::applicationName() const
{
    return d->appName;
}

void GCF::RequestAccessMessage::writeBody(QDataStream& ds) const
{
    ds << d->appName;
}

void GCF::RequestAccessMessage::readBody(QDataStream& ds)
{
    ds >> d->appName;
}

void GCF::RequestAccessMessage::dumpMessageInfo(QTextStream& ts) const
{
    Q_UNUSED(ts);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestAccessResMessageData
{
    RequestAccessResMessageData() : accessGranted(false) { }

    bool accessGranted;
};

GCF::RequestAccessResMessage::RequestAccessResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestAccessResMessageData;

    setMessageType(RequestAccessRes);
}

GCF::RequestAccessResMessage::RequestAccessResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestAccessResMessageData;

    setMessageType(RequestAccessRes);
}

GCF::RequestAccessResMessage::~RequestAccessResMessage()
{
    delete d;
}

void GCF::RequestAccessResMessage::setAccessGranted(bool val)
{
    if( isIncoming() )
        return;

    d->accessGranted = val;
}

bool GCF::RequestAccessResMessage::isAccessGranted() const
{
    return d->accessGranted;
}

void GCF::RequestAccessResMessage::writeBody(QDataStream& ds) const
{
    ds << d->accessGranted;
}

void GCF::RequestAccessResMessage::readBody(QDataStream& ds)
{
    ds >> d->accessGranted;
}

void GCF::RequestAccessResMessage::dumpMessageInfo(QTextStream& ts) const
{
    if( d->accessGranted )
        ts << "Access Granted";
    else
        ts << "Access Denied";
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestObjectMessageData
{
    QString objectName;
};

GCF::RequestObjectMessage::RequestObjectMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestObjectMessageData;

    setMessageType( RequestObject );
}

GCF::RequestObjectMessage::RequestObjectMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestObjectMessageData;

    setMessageType(RequestObject);
}

GCF::RequestObjectMessage::~RequestObjectMessage()
{
    delete d;
}

void GCF::RequestObjectMessage::setObjectName(const QString& name)
{
    if( isIncoming() )
        return;

    d->objectName = name;
}

QString GCF::RequestObjectMessage::objectName() const
{
    return d->objectName;
}

void GCF::RequestObjectMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectName;
}

void GCF::RequestObjectMessage::readBody(QDataStream& ds)
{
    ds >> d->objectName;
}

void GCF::RequestObjectMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "Object Name: " << d->objectName;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestObjectResMessageData
{
    RequestObjectResMessageData() : objectId(-1) { }

    qint32 objectId;
};

GCF::RequestObjectResMessage::RequestObjectResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestObjectResMessageData;

    setMessageType( RequestObjectRes );
}

GCF::RequestObjectResMessage::RequestObjectResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestObjectResMessageData;

    setMessageType(RequestObjectRes);
}

GCF::RequestObjectResMessage::~RequestObjectResMessage()
{
    delete d;
}

void GCF::RequestObjectResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::RequestObjectResMessage::objectId() const
{
    return d->objectId;
}

bool GCF::RequestObjectResMessage::isAccessGranted() const
{
    return d->objectId >= 0;
}

void GCF::RequestObjectResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
}

void GCF::RequestObjectResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
}

void GCF::RequestObjectResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::GetPropertyNamesMessageData
{
    GetPropertyNamesMessageData() : objectId(-1) { }

    qint32 objectId;
};

GCF::GetPropertyNamesMessage::GetPropertyNamesMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::GetPropertyNamesMessageData;

    setMessageType(GetPropertyNames);
}

GCF::GetPropertyNamesMessage::GetPropertyNamesMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::GetPropertyNamesMessageData;

    setMessageType(GetPropertyNames);
}

GCF::GetPropertyNamesMessage::~GetPropertyNamesMessage()
{
    delete d;
}

void GCF::GetPropertyNamesMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::GetPropertyNamesMessage::objectId() const
{
    return d->objectId;
}

void GCF::GetPropertyNamesMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
}

void GCF::GetPropertyNamesMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
}

void GCF::GetPropertyNamesMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::GetPropertyNamesResMessageData
{
    GetPropertyNamesResMessageData() : objectId(-1) { }

    qint32 objectId;
    QStringList propertyNames;
};

GCF::GetPropertyNamesResMessage::GetPropertyNamesResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::GetPropertyNamesResMessageData;

    setMessageType( GetPropertyNamesRes );
}

GCF::GetPropertyNamesResMessage::GetPropertyNamesResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::GetPropertyNamesResMessageData;

    setMessageType(GetPropertyNamesRes);
}

GCF::GetPropertyNamesResMessage::~GetPropertyNamesResMessage()
{
    delete d;
}

void GCF::GetPropertyNamesResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::GetPropertyNamesResMessage::objectId() const
{
    return d->objectId;
}

void GCF::GetPropertyNamesResMessage::setPropertyNames(const QStringList& names)
{
    if( isIncoming() )
        return;

    d->propertyNames = names;
}

QStringList GCF::GetPropertyNamesResMessage::propertyNames() const
{
    return d->propertyNames;
}

void GCF::GetPropertyNamesResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->propertyNames;
}

void GCF::GetPropertyNamesResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->propertyNames;
}

void GCF::GetPropertyNamesResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Property Name(s): " << d->propertyNames.join(", ");
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::SetPropertyMessageData
{
    SetPropertyMessageData() : objectId(-1) { }

    qint32 objectId;
    QString propertyName;
    QVariant propertyValue;
};

GCF::SetPropertyMessage::SetPropertyMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::SetPropertyMessageData;

    setMessageType( SetProperty );
}

GCF::SetPropertyMessage::SetPropertyMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::SetPropertyMessageData;

    setMessageType(SetProperty);
}

GCF::SetPropertyMessage::~SetPropertyMessage()
{
    delete d;
}

void GCF::SetPropertyMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::SetPropertyMessage::objectId() const
{
    return d->objectId;
}

void GCF::SetPropertyMessage::setPropertyName(const QString& name)
{
    if( isIncoming() )
        return;

    d->propertyName = name;
}

QString GCF::SetPropertyMessage::propertyName() const
{
    return d->propertyName;
}

void GCF::SetPropertyMessage::setPropertyValue(const QVariant& value)
{
    if( isIncoming() )
        return;

    d->propertyValue = value;
}

QVariant GCF::SetPropertyMessage::propertyValue() const
{
    return d->propertyValue;
}

void GCF::SetPropertyMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->propertyName;
    encodeVariant(ds, d->propertyValue);
}

void GCF::SetPropertyMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->propertyName;
    extractVariant(ds, d->propertyValue);
}

void GCF::SetPropertyMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Property Name: " << d->propertyName
        << ", Value: " << d->propertyValue.toString();
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::SetPropertyResMessageData
{
    SetPropertyResMessageData() : objectId(-1), success(false) { }

    qint32 objectId;
    QString propertyName;
    QVariant propertyValue;
    bool success;
};

GCF::SetPropertyResMessage::SetPropertyResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::SetPropertyResMessageData;

    setMessageType(SetPropertyRes);
}

GCF::SetPropertyResMessage::SetPropertyResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::SetPropertyResMessageData;

    setMessageType(SetPropertyRes);
}

GCF::SetPropertyResMessage::~SetPropertyResMessage()
{
    delete d;
}

void GCF::SetPropertyResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::SetPropertyResMessage::objectId() const
{
    return d->objectId;
}

void GCF::SetPropertyResMessage::setPropertyName(const QString& name)
{
    if( isIncoming() )
        return;

    d->propertyName = name;
}

QString GCF::SetPropertyResMessage::propertyName() const
{
    return d->propertyName;
}

void GCF::SetPropertyResMessage::setPropertyValue(const QVariant& value)
{
    if( isIncoming() )
        return;

    d->propertyValue = value;
}

QVariant GCF::SetPropertyResMessage::propertyValue() const
{
    return d->propertyValue;
}

void GCF::SetPropertyResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::SetPropertyResMessage::isSuccess() const
{
    return d->success;
}

void GCF::SetPropertyResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->propertyName;
    encodeVariant(ds, d->propertyValue);
    ds << d->success;
}

void GCF::SetPropertyResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->propertyName;
    extractVariant(ds, d->propertyValue);
    ds >> d->success;
}

void GCF::SetPropertyResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Property Name: " << d->propertyName
        << ", Value: " << d->propertyValue.toString() << ", Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::GetPropertyMessageData
{
    GetPropertyMessageData() : objectId(-1) { }

    qint32 objectId;
    QString propertyName;
};

GCF::GetPropertyMessage::GetPropertyMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::GetPropertyMessageData;

    setMessageType(GetProperty);
}

GCF::GetPropertyMessage::GetPropertyMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::GetPropertyMessageData;

    setMessageType(GetProperty);
}

GCF::GetPropertyMessage::~GetPropertyMessage()
{
    delete d;
}

void GCF::GetPropertyMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::GetPropertyMessage::objectId() const
{
    return d->objectId;
}

void GCF::GetPropertyMessage::setPropertyName(const QString& name)
{
    if( isIncoming() )
        return;

    d->propertyName = name;
}

QString GCF::GetPropertyMessage::propertyName() const
{
    return d->propertyName;
}

void GCF::GetPropertyMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->propertyName;
}

void GCF::GetPropertyMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->propertyName;
}

void GCF::GetPropertyMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Property Name: " << d->propertyName;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::GetPropertyResMessageData
{
    GetPropertyResMessageData() : objectId(-1), success(false) { }

    qint32 objectId;
    QString propertyName;
    QVariant propertyValue;
    bool success;
};

GCF::GetPropertyResMessage::GetPropertyResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::GetPropertyResMessageData;

    setMessageType( GetPropertyRes );
}

GCF::GetPropertyResMessage::GetPropertyResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::GetPropertyResMessageData;

    setMessageType(GetPropertyRes);
}

GCF::GetPropertyResMessage::~GetPropertyResMessage()
{
    delete d;
}

void GCF::GetPropertyResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::GetPropertyResMessage::objectId() const
{
    return d->objectId;
}

void GCF::GetPropertyResMessage::setPropertyName(const QString& name)
{
    if( isIncoming() )
        return;

    d->propertyName = name;
}

QString GCF::GetPropertyResMessage::propertyName() const
{
    return d->propertyName;
}

void GCF::GetPropertyResMessage::setPropertyValue(const QVariant& value)
{
    if( isIncoming() )
        return;

    d->propertyValue = value;
}

QVariant GCF::GetPropertyResMessage::propertyValue() const
{
    return d->propertyValue;
}

void GCF::GetPropertyResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::GetPropertyResMessage::isSuccess() const
{
    return d->success;
}

void GCF::GetPropertyResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->propertyName;
    encodeVariant(ds, d->propertyValue);
    ds << d->success;
}

void GCF::GetPropertyResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->propertyName;
    extractVariant(ds, d->propertyValue);
    ds >> d->success;
}

void GCF::GetPropertyResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Property Name: " << d->propertyName
        << ", Value: " << d->propertyValue.toString();
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::InvokeMethodMessageData
{
    InvokeMethodMessageData() : objectId(-1) { }

    qint32 objectId;
    QString methodName;
    QVariantList args;
};

GCF::InvokeMethodMessage::InvokeMethodMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::InvokeMethodMessageData;

    setMessageType( InvokeMethod );
}

GCF::InvokeMethodMessage::InvokeMethodMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::InvokeMethodMessageData;

    setMessageType(InvokeMethod);
}

GCF::InvokeMethodMessage::~InvokeMethodMessage()
{
    delete d;
}

void GCF::InvokeMethodMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::InvokeMethodMessage::objectId() const
{
    return d->objectId;
}

void GCF::InvokeMethodMessage::setMethodName(const QString& name)
{
    if( isIncoming() )
        return;

    d->methodName = name;
}

QString GCF::InvokeMethodMessage::methodName() const
{
    return d->methodName;
}

void GCF::InvokeMethodMessage::setArgList(const QVariantList& args)
{
    if( isIncoming() )
        return;

    d->args = args;
}

QVariantList GCF::InvokeMethodMessage::argList() const
{
    return d->args;
}

void GCF::InvokeMethodMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->methodName;
    encodeVariantList(ds, d->args);
}

void GCF::InvokeMethodMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->methodName;
    extractVariantList(ds, d->args);
}

void GCF::InvokeMethodMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Method Name: " << d->methodName;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::InvokeMethodResMessageData
{
    InvokeMethodResMessageData() : objectId(-1), success(false) { }

    qint32 objectId;
    QString methodName;
    QVariant retValue;
    bool success;
};

GCF::InvokeMethodResMessage::InvokeMethodResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::InvokeMethodResMessageData;

    setMessageType(InvokeMethodRes);
}

GCF::InvokeMethodResMessage::InvokeMethodResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::InvokeMethodResMessageData;

    setMessageType(InvokeMethodRes);
}

GCF::InvokeMethodResMessage::~InvokeMethodResMessage()
{
    delete d;
}

void GCF::InvokeMethodResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::InvokeMethodResMessage::objectId() const
{
    return d->objectId;
}

void GCF::InvokeMethodResMessage::setMethodName(const QString& name)
{
    if( isIncoming() )
        return;

    d->methodName = name;
}

QString GCF::InvokeMethodResMessage::methodName() const
{
    return d->methodName;
}

void GCF::InvokeMethodResMessage::setRetValue(const QVariant& val)
{
    if( isIncoming() )
        return;

    d->retValue = val;
}

QVariant GCF::InvokeMethodResMessage::retValue() const
{
    return d->retValue;
}

void GCF::InvokeMethodResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::InvokeMethodResMessage::isSuccess() const
{
    return d->success;
}

void GCF::InvokeMethodResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->methodName;
    encodeVariant(ds, d->retValue);
    ds << d->success;
}

void GCF::InvokeMethodResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->methodName;
    extractVariant(ds, d->retValue);
    ds >> d->success;
}

void GCF::InvokeMethodResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Method Name: " << d->methodName
        << ", Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestConnectionMessageData
{
    RequestConnectionMessageData() : objectId(-1) { }

    qint32 objectId;
    QString signal;
};

GCF::RequestConnectionMessage::RequestConnectionMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestConnectionMessageData;

    setMessageType( RequestConnection );
}

GCF::RequestConnectionMessage::RequestConnectionMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestConnectionMessageData;

    setMessageType(RequestConnection);
}

GCF::RequestConnectionMessage::~RequestConnectionMessage()
{
    delete d;
}

void GCF::RequestConnectionMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::RequestConnectionMessage::objectId() const
{
    return d->objectId;
}

void GCF::RequestConnectionMessage::setSignal(const QString& signal)
{
    if( isIncoming() )
        return;

    d->signal = signal;
}

QString GCF::RequestConnectionMessage::signal() const
{
    return d->signal;
}

void GCF::RequestConnectionMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->signal;
}

void GCF::RequestConnectionMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->signal;
}

void GCF::RequestConnectionMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Signal: " << d->signal;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::RequestConnectionResMessageData
{
    RequestConnectionResMessageData() : objectId(-1), success(false) { }

    qint32 objectId;
    QString signal;
    bool success;
};

GCF::RequestConnectionResMessage::RequestConnectionResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::RequestConnectionResMessageData;

    setMessageType( RequestConnectionRes );
}

GCF::RequestConnectionResMessage::RequestConnectionResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::RequestConnectionResMessageData;

    setMessageType(RequestConnectionRes);
}

GCF::RequestConnectionResMessage::~RequestConnectionResMessage()
{
    delete d;
}

void GCF::RequestConnectionResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::RequestConnectionResMessage::objectId() const
{
    return d->objectId;
}

void GCF::RequestConnectionResMessage::setSignal(const QString& signal)
{
    if( isIncoming() )
        return;

    d->signal = signal;
}

QString GCF::RequestConnectionResMessage::signal() const
{
    return d->signal;
}

void GCF::RequestConnectionResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::RequestConnectionResMessage::isSuccess() const
{
    return d->success;
}

void GCF::RequestConnectionResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->signal;
    ds << d->success;
}

void GCF::RequestConnectionResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->signal;
    ds >> d->success;
}

void GCF::RequestConnectionResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Signal: " << d->signal
        << ", Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::SignalDeliveryMessageData
{
    SignalDeliveryMessageData() : objectId(-1) { }

    qint32 objectId;
    QString signal;
    QVariantList args;
};

GCF::SignalDeliveryMessage::SignalDeliveryMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::SignalDeliveryMessageData;

    setMessageType( SignalDelivery );
}

GCF::SignalDeliveryMessage::SignalDeliveryMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::SignalDeliveryMessageData;

    setMessageType(SignalDelivery);
}

GCF::SignalDeliveryMessage::~SignalDeliveryMessage()
{
    delete d;
}

void GCF::SignalDeliveryMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::SignalDeliveryMessage::objectId() const
{
    return d->objectId;
}

void GCF::SignalDeliveryMessage::setSignal(const QString& signal)
{
    if( isIncoming() )
        return;

    d->signal = signal;
}

QString GCF::SignalDeliveryMessage::signal() const
{
    return d->signal;
}

void GCF::SignalDeliveryMessage::setArgList(const QVariantList& args)
{
    if( isIncoming() )
        return;

    d->args = args;
}

QVariantList GCF::SignalDeliveryMessage::argList() const
{
    return d->args;
}

void GCF::SignalDeliveryMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->signal;
    encodeVariantList(ds, d->args);
}

void GCF::SignalDeliveryMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->signal;
    extractVariantList(ds, d->args);
}

void GCF::SignalDeliveryMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Signal: " << d->signal;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseObjectMessageData
{
    ReleaseObjectMessageData() : objectId(-1) { }

    qint32 objectId;
};

GCF::ReleaseObjectMessage::ReleaseObjectMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseObjectMessageData;

    setMessageType( ReleaseObject );
}

GCF::ReleaseObjectMessage::ReleaseObjectMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseObjectMessageData;

    setMessageType(ReleaseObject);
}

GCF::ReleaseObjectMessage::~ReleaseObjectMessage()
{
    delete d;
}

void GCF::ReleaseObjectMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::ReleaseObjectMessage::objectId() const
{
    return d->objectId;
}

void GCF::ReleaseObjectMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
}

void GCF::ReleaseObjectMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
}

void GCF::ReleaseObjectMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseObjectResMessageData
{
    ReleaseObjectResMessageData() : objectId(-1), success(false) { }

    qint32 objectId;
    bool success;
};

GCF::ReleaseObjectResMessage::ReleaseObjectResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseObjectResMessageData;

    setMessageType( ReleaseObjectRes );
}

GCF::ReleaseObjectResMessage::ReleaseObjectResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseObjectResMessageData;

    setMessageType(ReleaseObjectRes);
}

GCF::ReleaseObjectResMessage::~ReleaseObjectResMessage()
{
    delete d;
}

void GCF::ReleaseObjectResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::ReleaseObjectResMessage::objectId() const
{
    return d->objectId;
}

void GCF::ReleaseObjectResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::ReleaseObjectResMessage::isSuccess() const
{
    return d->success;
}

void GCF::ReleaseObjectResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->success;
}

void GCF::ReleaseObjectResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->success;
}

void GCF::ReleaseObjectResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseAccessMessageData
{

};

GCF::ReleaseAccessMessage::ReleaseAccessMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseAccessMessageData;

    setMessageType( ReleaseAccess );
}

GCF::ReleaseAccessMessage::ReleaseAccessMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseAccessMessageData;

    setMessageType(ReleaseAccess);
}

GCF::ReleaseAccessMessage::~ReleaseAccessMessage()
{
    delete d;
}

void GCF::ReleaseAccessMessage::writeBody(QDataStream& ds) const
{
    // Do nothing
    Q_UNUSED(ds);
}

void GCF::ReleaseAccessMessage::readBody(QDataStream& ds)
{
    // Do nothing
    Q_UNUSED(ds);
}

void GCF::ReleaseAccessMessage::dumpMessageInfo(QTextStream& ts) const
{
    Q_UNUSED(ts);
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseAccessResMessageData
{
    ReleaseAccessResMessageData() : success(false)  { }

    bool success;
};

GCF::ReleaseAccessResMessage::ReleaseAccessResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseAccessResMessageData;

    setMessageType( ReleaseAccessRes );
}

GCF::ReleaseAccessResMessage::ReleaseAccessResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseAccessResMessageData;

    setMessageType(ReleaseAccessRes);
}

GCF::ReleaseAccessResMessage::~ReleaseAccessResMessage()
{
    delete d;
}

void GCF::ReleaseAccessResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::ReleaseAccessResMessage::isSuccess() const
{
    return d->success;
}

void GCF::ReleaseAccessResMessage::writeBody(QDataStream& ds) const
{
    ds << d->success;
}

void GCF::ReleaseAccessResMessage::readBody(QDataStream& ds)
{
    ds >> d->success;
}

void GCF::ReleaseAccessResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseConnectionMessageData
{
    ReleaseConnectionMessageData() : objectId(-1) { }

    qint32 objectId;
    QString signal;
};

GCF::ReleaseConnectionMessage::ReleaseConnectionMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseConnectionMessageData;

    setMessageType( ReleaseConnection );
}

GCF::ReleaseConnectionMessage::ReleaseConnectionMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseConnectionMessageData;

    setMessageType(ReleaseConnection);
}

GCF::ReleaseConnectionMessage::~ReleaseConnectionMessage()
{
    delete d;
}

void GCF::ReleaseConnectionMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::ReleaseConnectionMessage::objectId() const
{
    return d->objectId;
}

void GCF::ReleaseConnectionMessage::setSignal(const QString& signal)
{
    if( isIncoming() )
        return;

    d->signal = signal;
}

QString GCF::ReleaseConnectionMessage::signal() const
{
    return d->signal;
}

void GCF::ReleaseConnectionMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->signal;
}

void GCF::ReleaseConnectionMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->signal;
}

void GCF::ReleaseConnectionMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Signal: " << d->signal;
}

/////////////////////////////////////////////////////////////////////////////

struct GCF::ReleaseConnectionMessageResData
{
    ReleaseConnectionMessageResData() : objectId(-1), success(false) { }

    qint32 objectId;
    QString signal;
    bool success;
};

GCF::ReleaseConnectionResMessage::ReleaseConnectionResMessage(qint32 id) : GCF::Message(id)
{
    d = new GCF::ReleaseConnectionMessageResData;

    setMessageType( ReleaseConnectionRes );
}

GCF::ReleaseConnectionResMessage::ReleaseConnectionResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::ReleaseConnectionMessageResData;

    setMessageType(ReleaseConnectionRes);
}

GCF::ReleaseConnectionResMessage::~ReleaseConnectionResMessage()
{
    delete d;
}

void GCF::ReleaseConnectionResMessage::setObjectId(qint32 id)
{
    if( isIncoming() )
        return;

    d->objectId = id;
}

qint32 GCF::ReleaseConnectionResMessage::objectId() const
{
    return d->objectId;
}

void GCF::ReleaseConnectionResMessage::setSignal(const QString& signal)
{
    if( isIncoming() )
        return;

    d->signal = signal;
}

QString GCF::ReleaseConnectionResMessage::signal() const
{
    return d->signal;
}

void GCF::ReleaseConnectionResMessage::setSuccess(bool val)
{
    if( isIncoming() )
        return;

    d->success = val;
}

bool GCF::ReleaseConnectionResMessage::isSuccess() const
{
    return d->success;
}

void GCF::ReleaseConnectionResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectId;
    ds << d->signal;
    ds << d->success;
}

void GCF::ReleaseConnectionResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectId;
    ds >> d->signal;
    ds >> d->success;
}

void GCF::ReleaseConnectionResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectID: " << int(d->objectId) << ", Signal: " << d->signal
        << ", Success: " << d->success;
}

/////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct AsyncInvokeMethodMessageData
    {
        QString objectName;
        QString methodName;
        QVariantList argList;
    };
}

GCF::AsyncInvokeMethodMessage::AsyncInvokeMethodMessage(qint32 id)
: GCF::Message(id)
{
    d = new GCF::AsyncInvokeMethodMessageData;

    setMessageType( AsyncInvokeMethod );
}

GCF::AsyncInvokeMethodMessage::AsyncInvokeMethodMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::AsyncInvokeMethodMessageData;

    setMessageType(AsyncInvokeMethod);
}

GCF::AsyncInvokeMethodMessage::~AsyncInvokeMethodMessage ()
{
    delete d;
}

void GCF::AsyncInvokeMethodMessage::setObjectName(const QString& objectName)
{
    d->objectName = objectName;
}

QString GCF::AsyncInvokeMethodMessage::objectName() const
{
    return d->objectName;
}

void GCF::AsyncInvokeMethodMessage::setMethodName(const QString& methodName)
{
    d->methodName = methodName;
}

QString GCF::AsyncInvokeMethodMessage::methodName() const
{
    return d->methodName;
}

void GCF::AsyncInvokeMethodMessage::setArgList(const QVariantList& args)
{
    d->argList = args;
}

QVariantList GCF::AsyncInvokeMethodMessage::argList() const
{
    return d->argList;
}

void GCF::AsyncInvokeMethodMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectName;
    ds << d->methodName;
    encodeVariantList(ds, d->argList);
}

void GCF::AsyncInvokeMethodMessage::readBody(QDataStream& ds)
{
    ds >> d->objectName;
    ds >> d->methodName;
    extractVariantList(ds, d->argList);
}

void GCF::AsyncInvokeMethodMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectName: " << d->objectName << ", Method Name: " << d->methodName;
}

/////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct AsyncInvokeMethodResMessageData
    {
        AsyncInvokeMethodResMessageData() : success(false) { }

        QString objectName;
        QString methodName;
        bool success;
        QVariant result;
        QString errorMessage;
    };
}

GCF::AsyncInvokeMethodResMessage::AsyncInvokeMethodResMessage(qint32 id)
: GCF::Message(id)
{
    d = new GCF::AsyncInvokeMethodResMessageData;
    d->errorMessage = "Unknown Error";

    setMessageType( AsyncInvokeMethodRes );
}

GCF::AsyncInvokeMethodResMessage::AsyncInvokeMethodResMessage(GCF::Message* msg) : GCF::Message(msg)
{
    d = new GCF::AsyncInvokeMethodResMessageData;
    d->errorMessage = "Unknown Error";

    setMessageType(AsyncInvokeMethodRes);
}

GCF::AsyncInvokeMethodResMessage::~AsyncInvokeMethodResMessage ()
{
    delete d;
}

void GCF::AsyncInvokeMethodResMessage::setObjectName(const QString& objectName)
{
    d->objectName = objectName;
}

QString GCF::AsyncInvokeMethodResMessage::objectName() const
{
    return d->objectName;
}

void GCF::AsyncInvokeMethodResMessage::setMethodName(const QString& methodName)
{
    d->methodName = methodName;
}

QString GCF::AsyncInvokeMethodResMessage::methodName() const
{
    return d->methodName;
}

void GCF::AsyncInvokeMethodResMessage::setRetValue(const QVariant& val)
{
    d->result = val;
}

QVariant GCF::AsyncInvokeMethodResMessage::retValue() const
{
    return d->result;
}

void GCF::AsyncInvokeMethodResMessage::setSuccess(bool val)
{
    d->success = val;
}

bool GCF::AsyncInvokeMethodResMessage::isSuccess() const
{
    return d->success;
}

void GCF::AsyncInvokeMethodResMessage::setErrorMessage(const QString& errMsg)
{
    d->errorMessage = errMsg;
}

QString GCF::AsyncInvokeMethodResMessage::errorMessage() const
{
    return d->errorMessage;
}

void GCF::AsyncInvokeMethodResMessage::writeBody(QDataStream& ds) const
{
    ds << d->objectName;
    ds << d->methodName;
    ds << d->success;
    encodeVariant(ds, d->result);
    ds << d->errorMessage;
}

void GCF::AsyncInvokeMethodResMessage::readBody(QDataStream& ds)
{
    ds >> d->objectName;
    ds >> d->methodName;
    ds >> d->success;
    extractVariant(ds, d->result);
    ds >> d->errorMessage;
}

void GCF::AsyncInvokeMethodResMessage::dumpMessageInfo(QTextStream& ts) const
{
    ts << "ObjectName: " << d->objectName << ", Method Name: " << d->methodName
       << ", Success: " << d->success;
    if(!d->success)
        ts << " (" << d->errorMessage << ")";
}

/////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct AsyncInvokeMethodsMessageData
    {
        QList<GCF::Method> methodList;
    };
}

GCF::AsyncInvokeMethodsMessage::AsyncInvokeMethodsMessage(qint32 id)
    :GCF::Message(id)
{
    d = new GCF::AsyncInvokeMethodsMessageData;

    setMessageType( AsyncInvokeMethods );
}

GCF::AsyncInvokeMethodsMessage::AsyncInvokeMethodsMessage(GCF::Message* msg)
    :GCF::Message(msg)
{
    d = new GCF::AsyncInvokeMethodsMessageData;

    setMessageType( AsyncInvokeMethods );
}

GCF::AsyncInvokeMethodsMessage::~AsyncInvokeMethodsMessage ()
{
    delete d;
}

void GCF::AsyncInvokeMethodsMessage::setInvokeMethodList(const QList<GCF::Method>& methods)
{
    d->methodList = methods;
}

QList<GCF::Method> GCF::AsyncInvokeMethodsMessage::invokeMethodList() const
{
    return d->methodList;
}

void GCF::AsyncInvokeMethodsMessage::writeBody(QDataStream& ds) const
{
    ds << (quint32)d->methodList.count();

    Q_FOREACH(GCF::Method method, d->methodList)
    {
        ds << method.ObjectName;
        ds << method.MethodName;
        ds << (quint32)method.Args.count();
        Q_FOREACH(QVariant arg, method.Args)
            GCF::Message::encodeVariant(ds, arg);
    }
}

void GCF::AsyncInvokeMethodsMessage::readBody(QDataStream& ds)
{
    quint32 methodCount = 0;
    ds >> methodCount;

    d->methodList.clear();

    for(quint32 i=0; i<methodCount; i++)
    {
        GCF::Method method;
        ds >> method.ObjectName;
        ds >> method.MethodName;
        quint32 argCount = 0;
        ds >> argCount;
        for(quint32 a=0; a<argCount; a++)
        {
            QVariant arg;
            GCF::Message::extractVariant(ds, arg);
            method.Args.append(arg);
        }

        d->methodList.append(method);
    }
}

void GCF::AsyncInvokeMethodsMessage::dumpMessageInfo(QTextStream& ts) const
{
    QStringList methodInfoList;
    Q_FOREACH(GCF::Method method, d->methodList)
        methodInfoList << QString("%1::%2").arg(method.ObjectName).arg(method.MethodName);

    ts << "Invoking methods: " << methodInfoList.join(", ");
}

/////////////////////////////////////////////////////////////////////////////

namespace GCF
{
    struct AsyncInvokeMethodsResMessageData
    {
        QList<GCF::Method> methodList;
    };
}

GCF::AsyncInvokeMethodsResMessage::AsyncInvokeMethodsResMessage(qint32 id)
    :GCF::Message(id)
{
    d = new GCF::AsyncInvokeMethodsResMessageData;

    setMessageType( AsyncInvokeMethodsRes );
}

GCF::AsyncInvokeMethodsResMessage::AsyncInvokeMethodsResMessage(GCF::Message* msg)
    :GCF::Message(msg)
{
    d = new GCF::AsyncInvokeMethodsResMessageData;

    setMessageType( AsyncInvokeMethodsRes );
}

GCF::AsyncInvokeMethodsResMessage::~AsyncInvokeMethodsResMessage ()
{
    delete d;
}

void GCF::AsyncInvokeMethodsResMessage::setInvokedMethodList(const QList<GCF::Method>& MethodsRes)
{
    d->methodList = MethodsRes;
}

QList<GCF::Method> GCF::AsyncInvokeMethodsResMessage::invokedMethodList() const
{
    return d->methodList;
}

void GCF::AsyncInvokeMethodsResMessage::writeBody(QDataStream& ds) const
{
    ds << (quint32)d->methodList.count();

    Q_FOREACH(GCF::Method method, d->methodList)
    {
        ds << method.ObjectName;
        ds << method.MethodName;
        encodeVariant(ds, method.Result);
        ds << method.Success;
        ds << method.ErrorMessage;
    }
}

void GCF::AsyncInvokeMethodsResMessage::readBody(QDataStream& ds)
{
    quint32 methodCount = 0;
    ds >> methodCount;

    d->methodList.clear();

    for(quint32 i=0; i<methodCount; i++)
    {
        GCF::Method method;
        ds >> method.ObjectName;
        ds >> method.MethodName;
        extractVariant(ds, method.Result);
        ds >> method.Success;
        ds >> method.ErrorMessage;

        d->methodList.append(method);
    }
}

void GCF::AsyncInvokeMethodsResMessage::dumpMessageInfo(QTextStream& ts) const
{
    QStringList methodInfoList;
    Q_FOREACH(GCF::Method method, d->methodList)
        methodInfoList << QString("%1::%2=%3")
                          .arg(method.ObjectName)
                          .arg(method.MethodName)
                          .arg(method.Success);

    ts << "Invoked methods: " << methodInfoList.join(", ");
}
