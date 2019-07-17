#include "usb.h"
#include "ui_usb.h"
#include <QHBoxLayout>
#include <QRect>

const QRect SCREEN_SIZE;


Usb::Usb(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Usb)
{
    ui->setupUi(this);
    init();
}

Usb::~Usb()
{
    delete ui;
}


void Usb::init(){

    // udisk root path
//    tree_root = new QTreeWidget();
//    tree_root->setColumnCount(3);
//    QStringList list;
//    list.append("文件名"); list.append("文件大小"); list.append("修改日期");
//    tree_root->setHeaderLabels(list);
////    ui->verticalLayout->addWidget(tree_root);
//    QHBoxLayout* hbox = new QHBoxLayout();
//    hbox->addWidget(tree_root);
////    widget->setLayout(hbox);

//    QTreeWidgetItem* item = new QTreeWidgetItem();
//    tree_root->addTopLevelItem(item);
//    item->setText(0, root_path);

//    item->setIcon(0, QIcon("../source/dir_usb.ico"));
//    initDirectory(item, root_path);

//    this->setGeometry((SCREEN_SIZE.x() + SCREEN_SIZE.width()) / 4 ,
//                      (SCREEN_SIZE.y() + SCREEN_SIZE.height()) / 4,
//                      SCREEN_SIZE.width() / 2, SCREEN_SIZE.height() / 2);

//    connect(tree_root, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
//            this,      SLOT(whenItemDoubleClicked(QTreeWidgetItem*, int)));




}


void Usb::whenItemDoubleClicked(QTreeWidgetItem* item, int colum){

}
