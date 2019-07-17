#ifndef SDMMC_H
#define SDMMC_H

#include <QWidget>

namespace Ui {
class SDMMC;
}

class SDMMC : public QWidget
{
    Q_OBJECT

public:
    explicit SDMMC(QWidget *parent = nullptr);
    ~SDMMC();

private:
    Ui::SDMMC *ui;
};

#endif // SDMMC_H
