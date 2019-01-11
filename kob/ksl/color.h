//
// Created by 李寅斌 on 2018/11/21.
//

#ifndef KOB_KSL_COLOR_H_
#define KOB_KSL_COLOR_H_

namespace ksl {
namespace color {

    static const std::string reset      = "\033[00m";
    static const std::string bold       = "\033[1m";
    static const std::string dark       = "\033[2m";
    static const std::string underline  = "\033[4m";
    static const std::string blink      = "\033[5m";
    static const std::string reverse    = "\033[7m";
    static const std::string concealed  = "\033[8m";

    // Foreground colors
    static const std::string grey       = "\033[30m";
    static const std::string red        = "\033[31m";
    static const std::string green      = "\033[32m";
    static const std::string yellow     = "\033[33m";
    static const std::string blue       = "\033[34m";
    static const std::string magenta    = "\033[35m";
    static const std::string cyan       = "\033[36m";
    static const std::string white      = "\033[37m";

    /// Background colors
    static const std::string on_grey    = "\033[40m";
    static const std::string on_red     = "\033[41m";
    static const std::string on_green   = "\033[42m";
    static const std::string on_yellow  = "\033[43m";
    static const std::string on_blue    = "\033[44m";
    static const std::string on_magenta = "\033[45m";
    static const std::string on_cyan    = "\033[46m";
    static const std::string on_white   = "\033[47m";

}
}
#endif //KOB_KSL_COLOR_H_
