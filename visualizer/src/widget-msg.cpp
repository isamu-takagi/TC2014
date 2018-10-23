#include "widget-msg.hpp"
#include "tkglib/tkg-debug.hpp"

WidgetMSG::WidgetMSG() : QTextEdit()
{
    tkg::debug("new WidgetMSG\n");
//    setFrameStyle( QFrame::Panel | QFrame::Sunken );
//    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setReadOnly(true);
    setFontPointSize(10);
}

WidgetMSG::~WidgetMSG()
{
    tkg::debug("delete WidgetMSG\n");
}

void WidgetMSG::set_message(const std::string &str)
{
    setText(str.c_str());
}

void WidgetMSG::add_message(const std::string &str)
{
    setText(str.c_str() + toPlainText());
}

