#include "App.h"
#include "json.h"

bool with_gui = WITH_GUI ? true : false;
#ifdef WITH_GUI
    #include "imgui/gui.h"
    #include "widgets/log.h"
    #include "widgets/image.h"
#endif



struct PerSocketData {

};

void gui_setup(bool threaded = true){
    LogWidget log_w;
    ImageWidget img_w;
    img_w.img = cv::imread("/home/maris/Downloads/asd.jpeg");


   // Pass drawing functions to gui
    std::vector<std::function<void()>> drawing_procs = {
       [&]() { log_w.draw(); },
       [&]() { img_w.draw(); }
    };

    // seperate thread for GUI
    if(threaded)
        std::thread gui_thread([&](){ start_gui(drawing_procs); });
    else
        start_gui(drawing_procs);
}


int main(){
    // GUI
    if(with_gui)
        gui_setup();

    // Websocket server
	auto app = uWS::App();

    app.ws<PerSocketData>("/*", {
        /* Settings */
        .compression = uWS::SHARED_COMPRESSOR,
        .maxPayloadLength = 16 * 1024 * 1024,
        .idleTimeout = 10,
        .maxBackpressure = 1 * 1024 * 1204,

        /* Handlers */
        .open = [](auto *ws, auto *req) {
            /* Here you can use req just like as for Http */
            ws->send("Connected", uWS::OpCode::TEXT);
            int x=0;
        },

        // Client sends message
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
            auto x = ws->getUserData();
            ws->send(message, opCode);
        },

        .drain = [](auto *ws) {
            /* Check getBufferedAmount here */
        },
        .ping = [](auto *ws) {

        },
        .pong = [](auto *ws) {

        },
        .close = [](auto *ws, int code, std::string_view message) {
            ws->send("Disconnected from server", uWS::OpCode::TEXT);
        }
    });
    
    app.listen(9001, [](auto *listenSocket) {
        if (listenSocket) {
            std::cout << "Listening for connections..." << std::endl;
        }
    }).run();

    std::cout << "Shoot! We failed to listen and the App fell through, exiting now!" << std::endl;

}
