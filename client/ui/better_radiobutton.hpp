#pragma once
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
    void toggled(QString name, bool checked);

  private:
    QVBoxLayout *m_v_layout;
    std::vector<QRadioButton *> m_radiobuttons;
    QString m_checked;
};
