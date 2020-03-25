#pragma once
#include <stdexcept>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <exception>
#include "alias.h"

namespace fs {
    namespace std_fs = std::filesystem;
    using namespace dw;

    enum ENTRY_TYPE {
        FILE = 0,
        DIR
    };

    enum IO_MODE {
        APPEND = 0,
        TRUNCATE
    };

    std_fs::path fs_path(str path){
        return std_fs::path(path);
    }


    str parent_path(str path){
        auto p = fs_path(path);
        auto pp = p.parent_path();
        return pp.string();
    }


    bool exists(str path, ENTRY_TYPE et){
        auto p = fs_path(path);
        return std_fs::exists(p);
    }

    bool mk_dir(str path){
        auto p = fs_path(path);
        return std_fs::create_directories(p);
    }

    bool touch(str path){
        auto p = fs_path(path);
        auto pp = p.parent_path();

        if(exists(p))
            return false;

        if(!exists(pp))
            mk_dir(pp);

        std::ofstream ofs(path);
        ofs.close();

        return exists(p);
    }


    struct TxtFile {
        str p;

        TxtFile(str path) : p(path){ touch(p); }

        void append(str txt, bool truncate = false, bool new_line = true)
        {
            using namespace std;

            if(new_line)
                txt += '\n';

            if(truncate){
                fstream f(p, f.out | f.app | f.trunc);
                f << txt;
                f.flush();
            } else {
                fstream f(p, f.out | f.app);
                f << txt;
                f.flush();
            }
        }
    };


}
