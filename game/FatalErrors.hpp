#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//   ID ошибок, которые всегда приводят к крашу игры.
//   Эти ID отображаются в сообщениях исключений - std::exception#what().
//
///////////////////////////////////////////////////////////////////////////////////////////////////


#define FAILED_TO_BIND_UDP_SOCKET_TO_PORT        "AWD-C0001-FATAL"
#define DRAWABLE_ID_NOT_UNIQUE                   "AWD-C0002-FATAL"

