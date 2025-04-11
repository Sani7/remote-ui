#ifndef _HEXSPINBOX_H_
#define _HEXSPINBOX_H_

#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSpinBox>

// NOTE: Since QSpinBox uses int as the storage type, the effective editing range
//   is +/- 0x7FFF FFFF, so it can't handle a full unsigned int.
// QDoubleSpinBox would be a more suitable base class if a wider range is needed.
class HexSpinBox : public QSpinBox
{
    Q_OBJECT

  public:
    HexSpinBox(QWidget *parent = nullptr, bool showPrefix = false, const QString &format = QStringLiteral("%X"));

  public slots:
    void setShowPrefix(bool show);
    void setFormat(const QString &text);

  protected:
    QValidator::State validate(QString &text, int &pos) const override;
    int valueFromText(const QString &text) const override;
    QString textFromValue(int value) const override;

  private:
    QRegularExpressionValidator *validator;
    QString format;
};

#endif // _HEXSPINBOX_H_
