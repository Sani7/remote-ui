#include "networkerror.h"
#include "ui_networkerror.h"

NetworkError::NetworkError(QWidget* parent) : QDialog(parent), ui(new Ui::NetworkError) {
    ui->setupUi(this);
    connect(ui->exit, &QPushButton::clicked, this, &QCoreApplication::quit, Qt::QueuedConnection);
    connect(ui->close, &QPushButton::clicked, this, [=, this]{this->close();});

    this->setWindowTitle("Error");
}

void NetworkError::set_error(QString error)
{
    ui->Error->setText(error);
}

NetworkError::~NetworkError() {
    delete ui;
}
