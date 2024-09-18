#include "pch.h"

using boost::asio::ip::tcp;

int main()
{
    
    try
    {
        // Boost.Asio I/O 서비스 객체 생성
        boost::asio::io_service io_service;

        // 서버의 엔드포인트 생성
        tcp::resolver resolver(io_service);
        tcp::resolver::query query("127.0.0.1", "7777");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        // 소켓 생성 및 서버에 연결
        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        if (socket.is_open())
            std::cout << "Server Connected!!" << std::endl;

        while (true) {
            //메시지 수신
        }

        //// 서버로 보낼 메시지
        //const std::string msg = "Hello from client!";

        //// 메시지를 서버로 전송
        //boost::asio::write(socket, boost::asio::buffer(msg));

        //// 서버로부터 응답 받기 위한 버퍼 생성
        //char reply[1024];
        //size_t reply_length = boost::asio::read(socket, boost::asio::buffer(reply, msg.size()));

        //// 서버로부터 받은 응답 출력
        //std::cout << "Reply is: ";
        //std::cout.write(reply, reply_length);
        //std::cout << "\n";
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}