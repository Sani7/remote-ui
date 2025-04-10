#ifndef GPIO_HPP
#define GPIO_HPP

#include <QObject>
#ifdef __aarch64__
#include <gpiod.h>
#else
struct gpiod_line_request{};
struct gpiod_request_config{};
struct gpiod_line_settings{};
struct gpiod_line_config{};
struct gpiod_chip{};
enum gpiod_line_value {
	GPIOD_LINE_VALUE_ERROR = -1,
	/**< Returned to indicate an error when reading the value. */
	GPIOD_LINE_VALUE_INACTIVE = 0,
	/**< Line is logically inactive. */
	GPIOD_LINE_VALUE_ACTIVE = 1,
	/**< Line is logically active. */
};
enum gpiod_line_direction {
	GPIOD_LINE_DIRECTION_AS_IS = 1,
	/**< Request the line(s), but don't change direction. */
	GPIOD_LINE_DIRECTION_INPUT,
	/**< Direction is input - for reading the value of an externally driven
	 *   GPIO line. */
	GPIOD_LINE_DIRECTION_OUTPUT,
	/**< Direction is output - for driving the GPIO line. */
};
#endif

class GPIO : public QObject {
    Q_OBJECT
  public:
    enum Direction {
        Input,
        Output
    };
    explicit GPIO(Direction dir, uint8_t gpio, bool state = false, QObject* parent = nullptr);
    ~GPIO();
    void set_value(bool val);
    bool value();

  private:
    struct gpiod_line_request *
    request_output_line(const char *chip_path, unsigned int offset,
                        enum gpiod_line_value value, const char *consumer);
    struct gpiod_line_request *request_input_line(const char *chip_path,
                                                        unsigned int offset,
                                                        const char *consumer);

    struct gpiod_line_request *m_request;
    unsigned int m_offset;
};

#endif // GPIO_HPP
