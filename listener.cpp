#include "listener.hpp"

listener::listener()
    : m_id(1)
    , m_address_info(nullptr)
    , m_port("80")
    , m_connection_queue_size(30)
    , m_read_size(8 * 1024)
    , m_socket(-1)
{
}

listener::~listener()
{
}

void listener::start()
{
    std::cout << "[Listener] Starting" << std::endl;
    
    set_address_info();
    create_socket();

    set_socket_reusable();
    set_socket_no_delay();
    set_socket_bind();
    set_socket_non_blocking();
    set_socket_listen();

    create_epoll();

    std::cout << "[Listener] Started" << std::endl;
    
    while(true)
    {
        handle_events();
    }
}

void listener::request_callback(std::function<void(const int, const std::string&, const std::string&)> cb)
{
    m_cb = cb;
}

void listener::handle_events()
{
    const int event_count = epoll_wait(m_epoll, &m_epoll_events[0], m_connection_queue_size, -1);
    for(int i = 0; i < event_count; ++i)
    {
        handle_event(m_epoll_events[i]);
    }
}

void listener::read_data(const int sck, std::string& out_data)
{
    char buffer[m_read_size];
    memset(buffer, 0, m_read_size);

    read(sck, buffer, m_read_size - 1);
    
    out_data = buffer;
}

void listener::handle_event(const epoll_event& event)
{
    if((event.events & EPOLLERR) ||
       (event.events & EPOLLHUP) ||
       (!(event.events & EPOLLIN)))
    {
        close(event.data.fd);

        return;
    }

    if(m_socket == event.data.fd)
    {
        while(true)
        {
            sockaddr_in cli_addr;
            socklen_t clilen = sizeof(cli_addr);

            const int newsockfd = accept(m_socket, (sockaddr*)&cli_addr, &clilen);
            if(newsockfd == -1)
            {
                break;
            }
            
            set_epoll_interface(newsockfd);
            m_socket_address[newsockfd] = cli_addr.sin_addr;
        }
        
        return;
    }
    
    const int newsockfd = event.data.fd;
    
    std::string data = "";
    read_data(newsockfd, data);
    
    const std::string ip_address = inet_ntoa(m_socket_address[newsockfd]);
    
    m_cb(newsockfd, data, ip_address);
}

void listener::set_address_info()
{
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, m_port.c_str(), &hints, &m_address_info);
}

void listener::create_socket()
{
    m_socket = socket(m_address_info->ai_family,
                      m_address_info->ai_socktype,
                      m_address_info->ai_protocol);
}

void listener::set_socket_reusable()
{
    const int yes = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
}

void listener::set_socket_no_delay()
{
    const int yes = 1;
    setsockopt(m_socket, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(int));
}

void listener::set_socket_bind()
{
    bind(m_socket, m_address_info->ai_addr, m_address_info->ai_addrlen);
}

void listener::set_socket_non_blocking()
{
    int flags = fcntl(m_socket, F_GETFL, 0);
    flags |= O_NONBLOCK;

    fcntl(m_socket, F_SETFL, flags);
}

void listener::set_socket_listen()
{
    listen(m_socket, m_connection_queue_size);
}

void listener::create_epoll()
{
    m_epoll = epoll_create1(0);

    set_epoll_interface(m_socket);

    m_epoll_events.resize(m_connection_queue_size);
}

void listener::set_epoll_interface(int socket)
{
    epoll_event event;
    event.data.fd = socket;
    event.events = EPOLLIN | EPOLLET;

    epoll_ctl(m_epoll, EPOLL_CTL_ADD, socket, &event);
}
