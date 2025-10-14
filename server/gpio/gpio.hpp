#pragma once
#include <QObject>
#if defined(__linux__) && (defined(__aarch64__) || defined(__x86_64__))
#include <gpiod.hpp>
#else

#endif

class GPIO : public QObject
{
    Q_OBJECT
  public:
    enum Direction
    {
        Input,
        Output
    };
    explicit GPIO(QObject *parent = nullptr);
    ~GPIO();
    void configure_pin(Direction dir, uint8_t gpio);
    void set_value(bool val);
    bool value();

  private:
#if defined(__linux__) && (defined(__aarch64__) || defined(__x86_64__))
    std::unique_ptr<gpiod::line_request> m_request;
#endif
    unsigned int m_offset;
};
