#pragma once
#include <QRadioButton>
#include <QVBoxLayout>

#ifdef INTERNAL
#define EXPORT Q_DECL_EXPORT
#else
#define EXPORT Q_DECL_IMPORT
#endif

/**
 * @brief Class representing a better radiobutton group
 *
 */
class EXPORT Better_Radiobutton : public QWidget
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a new Better_Radiobutton object
     *
     * @param parent A pointer to the parent widget
     */
    explicit Better_Radiobutton(QWidget *parent = nullptr);
    /**
     * @brief Construct a new Better_Radiobutton object
     *
     * @param names A list of names for the radiobuttons
     * @param parent A pointer to the parent widget
     */
    explicit Better_Radiobutton(std::vector<QString> names, QWidget *parent = nullptr);

    /**
     * @brief Add a radiobutton to the group
     *
     * @param name The name of the radiobutton
     */
    void add_radiobutton(QString name);

    /**
     * @brief Get the name of the currently checked radiobutton
     *
     * @return QString The name of the checked radiobutton
     */
    QString isChecked() const;

  signals:
    /**
     * @brief Emitted when a radiobutton is toggled
     *
     * @param name The name of the radiobutton
     * @param checked Whether the radiobutton is checked
     */
    void toggled(QString name, bool checked);

  private:
    QVBoxLayout *m_v_layout;
    std::vector<QRadioButton *> m_radiobuttons;
    QString m_checked;
};
