#include "sdmmc.h"
#include "ui_sdmmc.h"

SDMMC::SDMMC(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SDMMC)
{
    ui->setupUi(this);
}

SDMMC::~SDMMC()
{
    delete ui;
}
