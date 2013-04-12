#include "xsdread.h"

XSDRead::XSDRead(QWidget *parent) :
    QWidget(parent)
{
    treeWidget = new QTreeWidget;
    QStringList labels;
    labels << "" << "ID" << "Name" << "Value" << "Pic" << "Axis";
    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    treeWidget->header()->setMovable(true);
    treeWidget->setHeaderLabels(labels);
    treeWidget->setColumnCount(6);

    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget(treeWidget);
    this->setLayout(layout);

    //Read XSD
    QString fileName =
            QFileDialog::getOpenFileName(this, tr("Open XSD Files"),
                                         QDir::currentPath(),
                                         tr("XSD Files (*.xsd)"));
    if (fileName.isEmpty()) return;
    file.setFileName(fileName);
    if (!readFile(fileName)) return;

    QXmlSchema schema;
    schema.load(&file);
    if (!schema.isValid())
    {
        QMessageBox::information(window(), tr("XSDRead")
                                 ,tr("This is not a valid XSD File!"));
    }

    file.close();

    //Read XML
    fileName =
            QFileDialog::getOpenFileName(this, tr("Open XML Files"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) return;
    file.setFileName(fileName);
    if (!readFile(fileName)) return;

    //DOM Read-In
    QString errorStr;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, true, &errorStr, &errorLine,
                                &errorColumn)) {
        QMessageBox::information(window(), tr("XSDRead"),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return;
    }

    //Reset the file pointer
    file.reset();

    //XSD Validator
    QXmlSchemaValidator validator(schema);
    if (!validator.validate(&file))
    {
        QMessageBox::information(window(),tr("XSDRead"),
                                 tr("This is not a valid XML File!"));
        return;
    }

    // Building the tree
    QDomElement node = doc.documentElement();
    nodeData* temp = NULL;
    Treeroot = new nodeData;
    Treeroot->name = node.tagName();
    QDomElement child = node.firstChildElement();
    while (!child.isNull())
    {
        temp = new nodeData;
        Treeroot->child.append(temp);
        parseElement(temp,child);
        child = child.nextSiblingElement();
    }

    //Show the tree in TreeView
    treeWidget->clear();
    QTreeWidgetItem* rootNode = new QTreeWidgetItem;
    rootNode->setText(2,Treeroot->name);
    for (int i = 0; i < Treeroot->child.count(); i++)
    {
        showTree(Treeroot->child[i], rootNode);
    }
    treeWidget->addTopLevelItem(rootNode);
    treeWidget->expandAll();
    this->resize(600,400);
}

void XSDRead::showTree(nodeData *root,QTreeWidgetItem *parentItem)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
    item->setText(1,QString("%1").arg(root->ID));
    item->setText(2,root->name);
    item->setText(4,root->picPath);
    QString AXIS = QString("%1,%2").arg(root->posX).arg(root->posY);
    item->setText(5,AXIS);
    if (root->child.count() != 0)
    {
        int tempValue = 0;
        for (int i = 0; i < root->child.count(); i++)
        {
            showTree(root->child[i], item);
            tempValue += root->child[i]->value;
        }
        root->value = tempValue;
    }
    item->setText(3,QString("%1").arg(root->value));
}

bool XSDRead::readFile(QString fileName)
{
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("XSDRead"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
        return false;
    }
    return true;
}

void XSDRead::parseElement(nodeData* root,const QDomElement &element)
{
    nodeData* temp = NULL;
    root->name = element.tagName();
    root->ID = element.attribute("id").toInt();
    root->picPath = element.attribute("pic");
    QDomElement x = element.firstChildElement().firstChildElement();
    QDomElement y = x.nextSiblingElement();
    root->posX = x.text().toInt();
    root->posY = y.text().toInt();
    QDomElement child = element.firstChildElement().nextSiblingElement();
    if (child.tagName() == "VALUE")
    {
        root->value = child.text().toInt();
    }
    else
    {
        while (!child.isNull())
        {
            temp = new nodeData;
            root->child.append(temp);
            parseElement(temp,child);
            child = child.nextSiblingElement();
        }
        int sum = 0;
        for (int i = 0; i < root->child.count(); i++)
            sum += root->child[i]->value;
        root->value =(int) (sum / root->child.count());
    }
}

XSDRead::~XSDRead()
{
}
