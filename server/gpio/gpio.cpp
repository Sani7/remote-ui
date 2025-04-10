#include "gpio.hpp"
#include "spdlog/spdlog.h"
#include <errno.h>
#if __aarch64__
#else
struct gpiod_chip *gpiod_chip_open(const char *path){return NULL;}
struct gpiod_line_settings *gpiod_line_settings_new(void){}
int gpiod_line_settings_set_direction(struct gpiod_line_settings *settings,
  enum gpiod_line_direction direction){return 0;}
int gpiod_line_settings_set_output_value(struct gpiod_line_settings *settings,
    enum gpiod_line_value value){return 0;}
struct gpiod_line_config *gpiod_line_config_new(void){return NULL;}
int gpiod_line_config_add_line_settings(struct gpiod_line_config *config,
  const unsigned int *offsets,
  size_t num_offsets,
  struct gpiod_line_settings *settings){return 0;}
struct gpiod_request_config *gpiod_request_config_new(void){}
void gpiod_request_config_set_consumer(struct gpiod_request_config *config,
  const char *consumer){}
struct gpiod_line_request *
  gpiod_chip_request_lines(struct gpiod_chip *chip,
         struct gpiod_request_config *req_cfg,
         struct gpiod_line_config *line_cfg){return NULL;}
int gpiod_line_request_set_value(struct gpiod_line_request *request,
            unsigned int offset,
            enum gpiod_line_value value){}
enum gpiod_line_value
gpiod_line_request_get_value(struct gpiod_line_request *request,
                             unsigned int offset) {}
void gpiod_line_request_release(struct gpiod_line_request *request) {}
void gpiod_request_config_free(struct gpiod_request_config *config){}
void gpiod_line_config_free(struct gpiod_line_config *config){}
void gpiod_line_settings_free(struct gpiod_line_settings *settings){}
void gpiod_chip_close(struct gpiod_chip *chip){}
#endif

GPIO::GPIO(Direction dir, uint8_t gpio, bool state, QObject* parent)
    : QObject(parent), m_offset(gpio)
{
    if (dir == Direction::Output)
    {
        m_request = request_output_line("/dev/gpiochip0", gpio, (enum gpiod_line_value)state, "unisim_cpp");
        if (!m_request) {
            SPDLOG_ERROR("failed to request line: {}",
                strerror(errno));
        }
        return;
    }
    if (dir == Direction::Input)
    {
        m_request = request_input_line("/dev/gpiochip0", gpio, "unisim_cpp");
        if (!m_request) {
            fprintf(stderr, "failed to request line: %s\n",
                    strerror(errno));
        }
    }
}

GPIO::~GPIO()
{
    gpiod_line_request_release(m_request);
}

void GPIO::set_value(bool value)
{
#if __aarch64__
    if (!m_request)
        return;
    gpiod_line_request_set_value(m_request, m_offset, (enum gpiod_line_value)value);
#endif
}

bool GPIO::value()
{
#if __aarch64__
    if (!m_request)
        return;
    return (bool)gpiod_line_request_get_value(m_request, m_offset);
#else
    return 0;
#endif
}

struct gpiod_line_request *
GPIO::request_output_line(const char *chip_path, unsigned int offset,
                    enum gpiod_line_value value, const char *consumer)
{
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_chip *chip;
    int ret;

    chip = gpiod_chip_open(chip_path);
    if (!chip)
        return NULL;

    settings = gpiod_line_settings_new();
    if (!settings)
        goto close_chip;

    gpiod_line_settings_set_direction(settings,
                                      GPIOD_LINE_DIRECTION_OUTPUT);
    gpiod_line_settings_set_output_value(settings, value);

    line_cfg = gpiod_line_config_new();
    if (!line_cfg)
        goto free_settings;

    ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1,
                                              settings);
    if (ret)
        goto free_line_config;

    if (consumer) {
        req_cfg = gpiod_request_config_new();
        if (!req_cfg)
            goto free_line_config;

        gpiod_request_config_set_consumer(req_cfg, consumer);
    }

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);

free_line_config:
    gpiod_line_config_free(line_cfg);

free_settings:
    gpiod_line_settings_free(settings);

close_chip:
    gpiod_chip_close(chip);

    return request;
}

struct gpiod_line_request *GPIO::request_input_line(const char *chip_path,
                                                     unsigned int offset,
                                                     const char *consumer)
{
    struct gpiod_request_config *req_cfg = NULL;
    struct gpiod_line_request *request = NULL;
    struct gpiod_line_settings *settings;
    struct gpiod_line_config *line_cfg;
    struct gpiod_chip *chip;
    int ret;

    chip = gpiod_chip_open(chip_path);
    if (!chip)
        return NULL;

    settings = gpiod_line_settings_new();
    if (!settings)
        goto close_chip;

    gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_INPUT);

    line_cfg = gpiod_line_config_new();
    if (!line_cfg)
        goto free_settings;

    ret = gpiod_line_config_add_line_settings(line_cfg, &offset, 1,
                                              settings);
    if (ret)
        goto free_line_config;

    if (consumer) {
        req_cfg = gpiod_request_config_new();
        if (!req_cfg)
            goto free_line_config;

        gpiod_request_config_set_consumer(req_cfg, consumer);
    }

    request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);

    gpiod_request_config_free(req_cfg);

free_line_config:
    gpiod_line_config_free(line_cfg);

free_settings:
    gpiod_line_settings_free(settings);

close_chip:
    gpiod_chip_close(chip);

    return request;
}
