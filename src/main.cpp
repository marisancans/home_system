#include "App.h"
#include "json.h"
#include "alias.h"
#include "logger.h"
#include "events.h"

bool with_gui = WITH_GUI ? true : false;
#ifdef WITH_GUI
    #include "imgui/gui.h"
    #include "widgets/log.h"
    #include "widgets/image.h"
#endif

using namespace dw;
using namespace nlohmann;

struct PerSocketData {
    uint64_t session_id = 0;
};

// Signals
dw::Signal<str> SIGNAL_log;



void start_sock_server(){
    // Websocket server
    using namespace uWS;
    App app;

    // Settings
    App::WebSocketBehavior b;
    b.compression = uWS::DISABLED;

    /* Here you can use req just like as for Http */
    b.open = [](auto *ws, auto *req) {

        // Set data to user
        auto u_data_ptr = ws->getUserData();
        PerSocketData* user_data = (PerSocketData*) u_data_ptr;

        ws->send("Connected", OpCode::TEXT);
        logger.info("Websocket client connected");
    };

    // Handle commands
    b.message = [&](auto *ws, std::string_view message, OpCode opCode) {
        auto u_data_ptr = ws->getUserData();
        PerSocketData* user_data = (PerSocketData*) u_data_ptr;

        auto j = json::parse(message);

        // Check command
        if(j.contains("command")){
            auto command = j.at("command").get<str>();

            // unfinished
            if(command == "subscribe"){
                if(j.contains("channel")){
                    auto channel = j.at("channel").get<str>();
                    ws->subscribe(channel);
                } else {
                    logger.warning("Websocket channel name not found: {}, skipping...");
                }
            }

            // Call event here with data
            if(command == "event"){
                auto data = j.at("data");

                auto msg = data.dump();
                logger.info(msg);

            }
            ws->send(message, opCode);
        }
    };
    b.close = [](auto *ws, int code, std::string_view message) {
        ws->send("Disconnected from server", OpCode::TEXT);
        logger.info("Websocket client disconnected: ", message);
    };

    // Create websocket
    app.ws<PerSocketData>("/*", std::move(b));

    // Run event loop
    app.listen(9001, [](auto *listenSocket) {
        if (listenSocket) {
            logger.info("Websockets listening for connections...");
        }
    }).run();

    logger.crash("Websockets fell through...");
};




int main(){
    // Logger
    logger.signal_p = &SIGNAL_log;
    logger.file_path = "/home/maris/Downloads/log.txt";

    LogWidget log_w;
    ImageWidget img_w;
    img_w.img = cv::imread("/home/maris/Downloads/asd.jpeg");


   // Pass drawing functions to gui
    std::vector<std::function<void()>> drawing_procs = {
       [&]() { log_w.draw(); },
//       [&]() { img_w.draw(); }
    };


    std::thread gui_thread([&](){ start_gui(drawing_procs); });


    dw::EventListener el;
    std::thread el_thread([&](){ el.eventLoop(); });



    // seperate thread for websocket server
    std::thread ws_thread([&](){
        start_sock_server();
    });


    // Plants

    connect(SIGNAL_log, &log_w, &LogWidget::SLOT_log, &el);



    while(true){};

};
