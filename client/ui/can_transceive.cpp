#include "can_transceive.hpp"
#include "ui_can_transceive.h"

Can_Transceive::Can_Transceive(QWidget *parent) : QWidget(parent), ui(new Ui::Can_Transceive)
{
    ui->setupUi(this);

    can_send_hexspinboxes = {
        ui->s_can_send_data0, ui->s_can_send_data1, ui->s_can_send_data2, ui->s_can_send_data3,
        ui->s_can_send_data4, ui->s_can_send_data5, ui->s_can_send_data6, ui->s_can_send_data7,
    };

    enable_can_send_data();
    connect(ui->c_can_send_dlc, &QComboBox::currentIndexChanged, this, [=, this] { enable_can_send_data(); });
    connect(ui->b_can_send_send, &QPushButton::clicked, this, [=, this] {
        emit send_can_message(
            ui->s_can_send_sid->value(), ui->c_can_send_dlc->currentIndex(),
            std::array<uint8_t, 8>(
                {(uint8_t)(can_send_hexspinboxes[0]->value()), (uint8_t)(can_send_hexspinboxes[1]->value()),
                 (uint8_t)(can_send_hexspinboxes[2]->value()), (uint8_t)(can_send_hexspinboxes[3]->value()),
                 (uint8_t)(can_send_hexspinboxes[4]->value()), (uint8_t)(can_send_hexspinboxes[5]->value()),
                 (uint8_t)(can_send_hexspinboxes[6]->value()), (uint8_t)(can_send_hexspinboxes[7]->value())}));
    });
    connect(ui->b_clear, &QPushButton::clicked, this, [=, this] { emit can_clear(); });
}

Can_Transceive::~Can_Transceive()
{
    delete ui;
}

void Can_Transceive::enable_can_send_data()
{
    uint8_t selected = atoi(ui->c_can_send_dlc->currentText().toLocal8Bit());
    for (size_t i = 0; i < can_send_hexspinboxes.size(); i++)
    {
        can_send_hexspinboxes[i]->setEnabled(i < selected);
        if (i >= selected)
        {
            can_send_hexspinboxes[i]->setValue(0);
        }
    }
}

void Can_Transceive::clear()
{
    ui->recvd_list->setRowCount(0);
    ui->send_list->setRowCount(0);
}

void Can_Transceive::add_send_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    size_t rc = ui->send_list->rowCount();
    ui->send_list->setRowCount(rc + 1);

    ui->send_list->setItem(rc, 0, new QTableWidgetItem(QString().setNum(id, 16)));
    ui->send_list->setItem(rc, 1, new QTableWidgetItem(QString("%0").arg(dlc)));
    for (size_t i = 0; i < dlc; i++)
    {
        ui->send_list->setItem(rc, 2 + i, new QTableWidgetItem(QString().setNum(payload[i], 16)));
    }
    ui->send_list->resizeColumnsToContents();
}

void Can_Transceive::add_receive_item(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    size_t rc = ui->recvd_list->rowCount();
    ui->recvd_list->setRowCount(rc + 1);

    ui->recvd_list->setItem(rc, 0, new QTableWidgetItem(QString().setNum(id, 16)));
    ui->recvd_list->setItem(rc, 1, new QTableWidgetItem(QString("%0").arg(dlc)));
    for (size_t i = 0; i < dlc; i++)
    {
        ui->recvd_list->setItem(rc, 2 + i, new QTableWidgetItem(QString().setNum(payload[i], 16)));
    }
    ui->recvd_list->resizeColumnsToContents();
}
