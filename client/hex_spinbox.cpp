#include "hex_spinbox.h"

HexSpinBox::HexSpinBox(QWidget *parent,
           bool showPrefix,
           const QString &format) :
      QSpinBox(parent),
      format(format)
{
    // Validates hex strings up to 8 chars with or w/out leading "0x" prefix.
    // For arbitrary prefix/suffix, the regex could be built dynamically
    //  in validate(), or override setPrefix()/setSuffix() methods.
    const QRegularExpression rx("(?:0[xX])?[0-9A-Fa-f]{1,8}");
    validator = new QRegularExpressionValidator(rx, this);
    setShowPrefix(showPrefix);
}

void HexSpinBox::setShowPrefix(bool show)
{
    if (show)
        setPrefix(QStringLiteral("0x"));
    else
        setPrefix(QString());
}

void HexSpinBox::setFormat(const QString &text)
{
    format = text;
    this->lineEdit()->setText(textFromValue(value()));
}

QValidator::State HexSpinBox::validate(QString &text, int &pos) const
{
    return validator->validate(text, pos);
}

int HexSpinBox::valueFromText(const QString &text) const
{
    return text.toInt(0, 16);
}

QString HexSpinBox::textFromValue(int value) const
{
    return QString().asprintf(qPrintable(format), value);
}
