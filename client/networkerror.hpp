#pragma once
#include <QDialog>

namespace Ui
{
class NetworkError;
}

class NetworkError : public QDialog
{
    Q_OBJECT

  public:
    explicit NetworkError(QWidget *parent = nullptr);
    void set_error(QString error);
    ~NetworkError();

  private:
    Ui::NetworkError *ui;
};
