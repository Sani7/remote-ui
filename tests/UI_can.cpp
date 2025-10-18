#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_can.hpp"

TEST(UI_can, add_clear_message)
{
    UI_can can(Color::Default, Color::Default);
    QByteArray array;
    array.fill((char)0xAA, 8);
    can.add_received_message(QCanBusFrame(123, array));
    array.fill((char)0xBB, 8);
    can.add_send_message(QCanBusFrame(456, array));
    json j = can.to_json(0);
    EXPECT_EQ(j.dump(),
              "{\"bg_color\":\"#FF839192\",\"enabled\":true,\"fg_color\":\"#FF839192\",\"id\":0,\"rcvd_msgs\":[{"
              "\"dlc\":8,\"id\":123,\"payload\":[170,170,170,170,170,170,170,170]}],\"send_msgs\":[{\"dlc\":8,\"id\":"
              "456,\"payload\":[187,187,187,187,187,187,187,187]}],\"type\":\"ui_can\",\"visible\":true}");

    can.clear_send_buffer();
    j = can.to_json(0);
    EXPECT_EQ(j.dump(), "{\"bg_color\":\"#FF839192\",\"enabled\":true,\"fg_color\":\"#FF839192\",\"id\":0,\"rcvd_"
                        "msgs\":[{\"dlc\":8,\"id\":123,\"payload\":[170,170,170,170,170,170,170,170]}],\"send_msgs\":[]"
                        ",\"type\":\"ui_can\",\"visible\":true}");

    can.clear_receive_buffer();
    j = can.to_json(0);
    EXPECT_EQ(j.dump(), "{\"bg_color\":\"#FF839192\",\"enabled\":true,\"fg_color\":\"#FF839192\",\"id\":0,\"rcvd_"
                        "msgs\":[],\"send_msgs\":[],\"type\":\"ui_can\",\"visible\":true}");

    array.fill((char)0xAA, 8);
    can.add_received_message(QCanBusFrame(123, array));
    array.fill((char)0xBB, 8);
    can.add_send_message(QCanBusFrame(456, array));
    j = can.to_json(0);
    EXPECT_EQ(j.dump(),
              "{\"bg_color\":\"#FF839192\",\"enabled\":true,\"fg_color\":\"#FF839192\",\"id\":0,\"rcvd_msgs\":[{"
              "\"dlc\":8,\"id\":123,\"payload\":[170,170,170,170,170,170,170,170]}],\"send_msgs\":[{\"dlc\":8,\"id\":"
              "456,\"payload\":[187,187,187,187,187,187,187,187]}],\"type\":\"ui_can\",\"visible\":true}");

    can.clear();
    j = can.to_json(0);
    EXPECT_EQ(j.dump(), "{\"bg_color\":\"#FF839192\",\"enabled\":true,\"fg_color\":\"#FF839192\",\"id\":0,\"rcvd_"
                        "msgs\":[],\"send_msgs\":[],\"type\":\"ui_can\",\"visible\":true}");
}

TEST(UI_can, signal)
{
    UI_can can(Color::Default, Color::Default);
    QSignalSpy spy_ui(&can, &UI_can::ui_changed);
    QSignalSpy spy_send(&can, &UI_can::send);
    QByteArray array;
    array.fill((char)0xAA, 8);
    can.add_received_message(QCanBusFrame(123, array));
    array.fill((char)0xBB, 8);
    can.add_send_message(QCanBusFrame(456, array));
    can.can_send(123, 3, {0xAA, 0xBB, 0xAA});
    EXPECT_EQ(spy_ui.count(), 2);
    EXPECT_EQ(spy_send.count(), 1);
}
