#ifndef NETWORKERROR_H
#define NETWORKERROR_H

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

#endif // NETWORKERROR_H
