#ifndef XSDREAD_H
#define XSDREAD_H

#include <QtGui>
#include <QtXmlPatterns>
#include <QDomDocument>

class XSDRead : public QWidget
{
    Q_OBJECT
    
public:
    explicit XSDRead(QWidget *parent = 0);
    ~XSDRead();
    
private:
    QFile file, fileXML;
    QDomDocument doc;

    class nodeData
    {
    public:
        int posX,posY,ID,value;
        QString picPath,name;
        QList<nodeData*> child;
    };
    nodeData* Treeroot;
    QTreeWidget* treeWidget;

    void parseElement(nodeData*,const QDomElement&);
    bool readFile(QString);
    void showTree(nodeData*, QTreeWidgetItem *parentItem);

};

#endif // XSDREAD_H
