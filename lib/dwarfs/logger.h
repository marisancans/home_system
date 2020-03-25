#pragma once
#include "alias.h"
#include "alg.h"

#include <mutex>
#include <cstdio>
#include <iostream>
#include <fstream>

// forward declared
namespace fs {
struct TxtFile;
}


namespace dw {

// Forward declared
template <typename ...Args>
struct Signal;

struct Logger {
    struct{
        int log_level_length = 4;
    } formatter;

    str file_path;
    uptr<fs::TxtFile> txt_file_p;
    std::mutex mtx;
    map<str, int> log_levels { {"debug", 1 }, {"info", 2}, {"warning", 3}, {"crash", 4}};

    void add_log_level(str log_level, int priotity){
        log_levels[log_level] = priotity;
    }

    Signal<str>* signal_p; // if provided, event will be fired upon logging

    str prefix_level(str fmt, str log_level){
        if(!exists(log_level, log_levels))
            throw std::runtime_error("Log level doesn't exist!");

        log_level[0] = std::toupper(log_level[0]);

        auto trim_to = min(int(log_level.length()), formatter.log_level_length);
        log_level = log_level.erase(trim_to, std::string::npos);

        return "[" + log_level + "] " + fmt;
    }

    template <typename ...Args>
    str format_t(str fmt, str log_level, Args... args){
        size_t size = snprintf( nullptr, 0, fmt.c_str(), args ... ) + 1; // Extra space for '\0'
        if( size <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
        std::unique_ptr<char[]> buf( new char[ size ] );
        snprintf( buf.get(), size, fmt.c_str(), args ... );
        auto formatted = std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside

        formatted = prefix_level(formatted, log_level);
        return formatted;
    }

    void exec_handlers(str fmt);

    void format_t(str fmt, str log_level){
        fmt = prefix_level(fmt, log_level);
        exec_handlers(fmt);
    }

    template <typename ...Args>
    void log(str fmt, str log_level, Args... args){
        format_t(fmt, log_level, args...);
    }

    template <typename ...Args>
    void debug(str fmt, Args... args){
        format_t(fmt, "debug", args...);
    }

    template <typename ...Args>
    void info(str fmt, Args... args){
        format_t(fmt, "info", args...);
    }

    template <typename ...Args>
    void warning(str fmt, Args... args){
        format_t(fmt, "warning", args...);
    }

    template <typename ...Args>
    void crash(str fmt, Args... args, int exit_code = 0){
        format_t(fmt, "crash", args...);
        std::exit(exit_code);
    }

    // without args
    void log(str fmt, str log_level) { format_t(fmt, log_level); }
    void debug(str fmt){ format_t(fmt, "debug"); }
    void info(str fmt){ format_t(fmt, "info"); }
    void warning(str fmt){ format_t(fmt, "warning"); }
    void crash(str fmt){ format_t(fmt, "crash"); }
};

extern Logger logger;
}
