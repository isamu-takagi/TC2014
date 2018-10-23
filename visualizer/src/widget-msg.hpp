#ifndef WIDGET_MSG_HPP
#define WIDGET_MSG_HPP

#include <QTextEdit>

class WidgetMSG : public QTextEdit
{
    Q_OBJECT

    public:

         WidgetMSG();
        ~WidgetMSG();

    public slots:

         void set_message(const std::string &str);
         void add_message(const std::string &str);
};

#endif // WIDGET_MSG_HPP
