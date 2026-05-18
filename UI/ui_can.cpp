#include "ui_can.hpp"

UI_can::UI_can(Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_can, parent)
{
    m_ui_item.can().mutColor().setFgColor(fg_color.color());
    m_ui_item.can().mutColor().setBgColor(bg_color.color());
}

UI_can::UI_can(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_can, parent)
{
}

void UI_can::add_send_message(QCanBusFrame msg)
{
    CAN_Frame_m frame;(msg.frameId(), msg.payload().size(), msg.payload());
    frame.setSid(msg.frameId());
    frame.setDlc(msg.payload().size());
    frame.setPayload(msg.payload());
    auto send_frames = m_ui_item.can().sendFrames();
    send_frames.append(frame);
    m_ui_item.can().setSendFrames(send_frames);
    emit ui_changed();
}

void UI_can::add_received_message(QCanBusFrame msg)
{
    CAN_Frame_m frame;(msg.frameId(), msg.payload().size(), msg.payload());
    frame.setSid(msg.frameId());
    frame.setDlc(msg.payload().size());
    frame.setPayload(msg.payload());
    auto recv_frames = m_ui_item.can().recvFrames();
    recv_frames.append(frame);
    m_ui_item.can().setRecvFrames(recv_frames);
    emit ui_changed();
}

void UI_can::clear_send_buffer()
{
    m_ui_item.can().setSendFrames(QList<CAN_Frame_m>());
    emit ui_changed();
}

void UI_can::clear_receive_buffer()
{
    m_ui_item.can().setSendFrames(QList<CAN_Frame_m>());
    emit ui_changed();
}

void UI_can::clear()
{
    clear_send_buffer();
    clear_receive_buffer();
}

void UI_can::can_send(uint32_t id, uint64_t dlc, QByteArray payload)
{
    QCanBusFrame frame;
    Q_UNUSED(dlc);
    frame.setFrameId(id);
    frame.setPayload(payload);

    emit send(frame);
}
