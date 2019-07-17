#ifndef USB_H
#define USB_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>


namespace Ui {
class Usb;
}

class Usb : public QWidget
{
    Q_OBJECT

public:
    explicit Usb(QWidget *parent = nullptr);
    ~Usb();
    void initDirectory(QTreeWidgetItem* curRoot, QString path);

private slots:
    void whenItemDoubleClicked(QTreeWidgetItem* item, int colum);

private:
    Ui::Usb *ui;
    QTreeWidget *tree_root;
//    QTreeWidgetItem *tree_widget_item;
    QString root_path;
    void init();
};

#endif // USB_H
