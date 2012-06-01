#include "htmlnotewriter.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include "datetime.h"
#include "xmlnotereader.h"
#include <QDir>


HtmlNoteWriter::HtmlNoteWriter(const QString &filePath)
{
   document_ = 0;
   filePath_ = filePath;
}

void HtmlNoteWriter::write()
{

    if(!document_)
    {
        qDebug("HtmlNoteWriter::write failed : no QTextDocument set");
        return;
    }


    document_->setMetaInformation(QTextDocument::DocumentTitle,title_);

    QString content = document_->toHtml();

    // remove { } braces
    QString uuidStr = uuid_.toString().remove(0,1);
    uuidStr.chop(1);

    QString metaUuid("<meta name=\"uuid\" content=\"" + uuidStr + "\"/>");

    QString metaLastChange("<meta name=\"last-change-date\" content=\"" +DateTime::toISO8601(lastChange_.isNull()? QDateTime::currentDateTime():lastChange_) + "\"/>");
    QString metaCreateDate("<meta name=\"create-date\" content=\"" +DateTime::toISO8601(createDate_.isNull()? QDateTime::currentDateTime():createDate_) + "\"/>");

    // insert meta elements
    int headIdx = content.indexOf("<head>");
    content.insert(headIdx + qstrlen("<head>"), metaCreateDate);
    content.insert(headIdx + qstrlen("<head>"), metaLastChange);
    content.insert(headIdx + qstrlen("<head>"), metaUuid);

    QFile file(filePath_);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug(qPrintable(QString("HtmlNoteWriter::write failed : could not open ") + filePath_));
           return;
    }

    QTextStream out(&file);
    out << content;
    file.close();
}

/*static*/ void HtmlNoteWriter::writeXml2Html(const QString &xmlFilePath, const QString &outputPath)
{
    if(xmlFilePath.isEmpty() || outputPath.isEmpty())
        return;

    QTextDocument document;
    XmlNoteReader reader(xmlFilePath,&document);

    QString folder;
    QString tag = reader.tag();

    QRegExp illegal("[" +QRegExp::escape("\\^/?<>:*|\"")+ "]|^(com\\d|lpt\\d|con|nul|prn)$");
    if(!tag.isEmpty())
    {
        //  takes 2nd colon, remove before
        //int colonIdx = tag.indexOf(":",tag.indexOf(":")+1); // 2nd index of :, e.g. "system:notebook:tagname"
        //folder = tag.right(tag.length()-colonIdx);
        tag.remove("system:notebook:");
        tag.remove("system:template");
        tag.remove(illegal);//remove illegal chars in filenames
        if(tag[0] == '.') // would be invisible if allowed
            tag.remove(0,1);
        folder = tag;
    }
    else
        folder = tr("default");

    QString title = reader.title();
    title.remove(illegal);
    if(title.isEmpty())
        title = tr("untitled note");

    QString filePath;
    QDir().mkpath(outputPath + "/" + folder);
    filePath =  outputPath + "/"+ folder + "/" + title;

    // TODO move this in extra static method
    int counter = 0;
    QString origPath = filePath;
    while(QFile::exists(filePath))
    {
        ++counter;
        filePath = origPath +  QString(" (%1)").arg(counter);
    }

    HtmlNoteWriter writer(filePath);
    writer.setDocument(&document);
    writer.setTitle(title);
    writer.setLastChange(reader.lastChange());
    writer.setLastMetadataChange(reader.lastMetadataChange());
    writer.setCreateDate(reader.createDate());
    writer.setUuid(reader.uuid());
    writer.write();
}