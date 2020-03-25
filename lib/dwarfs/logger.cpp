#include "logger.h"
#include "events.h"
#include "fs.h"

namespace dw {
    void Logger::exec_handlers(str fmt){
        // not the most effective file writing
        if(!file_path.empty()){
                if(!txt_file_p)
                    txt_file_p = std::make_unique<fs::TxtFile>(file_path);
                mtx.lock();
                txt_file_p->append(fmt);
                mtx.unlock();
        };

        // Signal
        if(signal_p){
            mtx.lock();
            signal_p->emit(fmt);
            mtx.unlock();
        }

        // console
        std::cout << fmt << std::endl;
    }


    Logger logger;
}
