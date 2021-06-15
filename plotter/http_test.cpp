#include <sys/socket.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <util.hpp>
#include "plotterUtil.cpp"
//#include "HttpReq.h"

//time_t StrTime2unix(const std::string& ts)
//{
//    struct tm tm {
//    };
//    memset(&tm, 0, sizeof(tm));
//
//    sscanf(
//        ts.c_str(),
//        "%d-%d-%d %d:%d:%d",
//        &tm.tm_year,
//        &tm.tm_mon,
//        &tm.tm_mday,
//        &tm.tm_hour,
//        &tm.tm_min,
//        &tm.tm_sec);
//
//    tm.tm_year -= 1900;
//    tm.tm_mon--;
//
//    return mktime(&tm);
//}
//
//std::string GetEnv(const char* envName)
//{
//    char* podName = new char[0];
//    // podName = getenv("HOME");
//    if (getenv(envName) != nullptr) {
//        podName = getenv(envName);
//    }
//    return podName;
//}
//
//std::string GenTimeNow()
//{
//    time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//    std::stringstream ss;
//    ss << std::put_time(std::localtime(&t), "%F %X");
//    std::cout << ss.str() << std::endl;
//    std::string ssTime = ss.str();
//    time_t timeUnix = StrTime2unix(ssTime);
//
//    std::stringstream strUnix;
//    strUnix << timeUnix;
//    std::string ts = strUnix.str();
//    return ts;
//}

int main()
{
    ReportHttp("1","");
    ReportHttp("2","");
    ReportHttp("3","");
    ReportHttp("4","");
    ReportHttp("5","");
//    Timer p1;
    // HTTP 请求告诉服务～
//    sleep(3);
    //    std::double_t duringTime = 0;
    //    if (p1.GetElapsed() != 0) {
    //        duringTime = p1.GetElapsed();
    //    }
    //    std::cout << "time ============" << duringTime << "s" << std::endl;
    //    std::ostringstream strDurTime;
    //    strDurTime << duringTime;
//    std::string a = "http://10.1.64.143:8001/ReportChart?pod=" + pod + "&node=" + node+"&p=1";
//    HttpRequest* Http;
//    char http_return[4096] = {0};
//    char http_msg[4096] = {0};
//    std::string pod = GetEnv("JOB_POD_NAME");
//    std::string node = GetEnv("JOB_NODE_NAME");
//    std::string a = "http://127.0.0.1:8001/ReportChart?pod=" + pod + "&node=" + node+"&p=1";
//    std::strcpy(http_msg, a.data());
//    if (Http->HttpGet(http_msg, http_return)) {
//        std::cout << "get" << http_return << std::endl;
//    }
//
//
//    std::string a2 = "http://127.0.0.1:8001/ReportChart?pod=" + pod + "&node=" + node+"&p=2";
//    std::strcpy(http_msg, a2.data());
//    if (Http->HttpGet(http_msg, http_return)) {
//        std::cout << "get" << http_return << std::endl;
//    }
//
//    std::string a3 = "http://127.0.0.1:8001/ReportChart?pod=" + pod + "&node=" + node+"&p=3";
//    std::strcpy(http_msg, a3.data());
//    if (Http->HttpGet(http_msg, http_return)) {
//        std::cout << "get" << http_return << std::endl;
//    }
//
//    std::string a4 = "http://127.0.0.1:8001/ReportChart?pod=" + pod + "&node=" + node+"&p=4";
//    std::strcpy(http_msg, a4.data());
//    if (Http->HttpGet(http_msg, http_return)) {
//        std::cout << "get" << http_return << std::endl;
//    }
    return 0;
}
