#pragma once

#include "ui_base.hpp"
#include <QMap>
#include <QObject>
#include <QString>
#include <functional>

class UIFactory
{
  public:
    using Creator = std::function<UI_base *(Web_socket_wrapper *web_socket, QWidget *)>;

    static UIFactory &instance()
    {
        static UIFactory f;
        return f;
    }

    bool registerType(const QString &key, Creator creator)
    {
        if (m_creators.contains(key))
            return false;
        m_creators.insert(key, creator);
        return true;
    }

    UI_base *create(const QString &key, Web_socket_wrapper *w, QWidget *parent = nullptr) const
    {
        auto it = m_creators.constFind(key);
        if (it == m_creators.cend())
            return nullptr;
        return it.value()(w, parent);
    }

    QStringList keys() const
    {
        return m_creators.keys();
    }

  private:
    QMap<QString, Creator> m_creators;
};

#define REGISTER_UI(TYPE)                                                                                              \
    namespace                                                                                                          \
    {                                                                                                                  \
    struct Register##TYPE                                                                                              \
    {                                                                                                                  \
        Register##TYPE()                                                                                               \
        {                                                                                                              \
            UIFactory::instance().registerType(TYPE::name(),                                                           \
                                               [](Web_socket_wrapper *w, QWidget *p) { return new TYPE(w, p); });      \
        }                                                                                                              \
    };                                                                                                                 \
    static Register##TYPE register_##TYPE;                                                                             \
    }
