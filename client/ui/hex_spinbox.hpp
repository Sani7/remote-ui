#pragma once
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QSpinBox>

#ifdef INTERNAL
#define EXPORT Q_DECL_EXPORT
#else
#define EXPORT Q_DECL_IMPORT
#endif

/**
 * @brief Class representing a hexadecimal spin box
 * @note Since QSpinBox uses int as the storage type, the effective editing range
 *       is +/- 0x7FFF FFFF, so it can't handle a full unsigned int.
 *       QDoubleSpinBox would be a more suitable base class if a wider range is needed.
 */
class EXPORT HexSpinBox : public QSpinBox
{
    Q_OBJECT

  public:
    /**
     * @brief Construct a new Hex Spin Box object
     *
     * @param parent The parent QWidget
     */
    explicit HexSpinBox(QWidget *parent = nullptr);
    /**
     * @brief Construct a new Hex Spin Box object
     *
     * @param showPrefix Whether to show the "0x" prefix
     * @param format The printf-style format string to use for displaying the value
     * @param parent The parent QWidget
     */
    HexSpinBox(bool showPrefix = false, const QString &format = QStringLiteral("%X"), QWidget *parent = nullptr);

  public slots:
    /**
     * @brief Set the Show Prefix object
     *
     * @param show True to show the "0x" prefix, false to hide it
     */
    void setShowPrefix(bool show);

    /**
     * @brief Set the Format object
     *
     * @param text The printf-style format string to use for displaying the value
     */
    void setFormat(const QString &text);

  protected:
    /**
     * @brief Validate the input text
     *
     * @param text The input text
     * @param pos The position of the cursor
     * @return QValidator::State The validation state
     */
    QValidator::State validate(QString &text, int &pos) const override;
    /**
     * @brief Convert text to value
     *
     * @param text The input text
     * @return int The corresponding value
     */
    int valueFromText(const QString &text) const override;
    /**
     * @brief Convert value to text
     *
     * @param value The input value
     * @return QString The corresponding text
     */
    QString textFromValue(int value) const override;

  private:
    QRegularExpressionValidator *validator;
    QString format;
};
