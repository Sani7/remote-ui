#ifndef BETTER_RADIOBUTTON_H
#define BETTER_RADIOBUTTON_H

#include <QRadioButton>
#include <QVBoxLayout>

class Better_Radiobutton : public QWidget
{
    Q_OBJECT
  public:
    explicit Better_Radiobutton(QWidget *parent = nullptr);
    explicit Better_Radiobutton(std::vector<QString> names, QWidget *partent = nullptr);

    void add_radiobutton(QString name);
    QString isChecked() const;

  signals:
    void toggled(QString name);
    void checked(QString name);

  private:
    QVBoxLayout *m_v_layout;
    std::vector<QRadioButton *> m_radiobuttons;
    QString m_checked;
};

#endif // BETTER_RADIOBUTTON_H
