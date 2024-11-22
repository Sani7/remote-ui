#ifndef LED_H
#define LED_H

#include "QPushButton"

class Led : public QPushButton {
    Q_OBJECT
  public:
    explicit Led(QWidget *parent = nullptr);
    explicit Led(const QString &text, QWidget *parent = nullptr);
    Led(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ~Led();
  private:
    bool is_read_only(QWidget* widget);
    void set_read_only(QWidget* widget, bool read_only);
};

#endif // LED_H
