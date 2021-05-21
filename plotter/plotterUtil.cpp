//
// Created by Nate Yang on 2021/5/21.
//
#include <sys/socket.h>

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <util.hpp>

#include "HttpReq.h"

time_t StrTime2unix(const std::string& ts)
{
    struct tm tm {
    };
    memset(&tm, 0, sizeof(tm));

    sscanf(
        ts.c_str(),
        "%d-%d-%d %d:%d:%d",
        &tm.tm_year,
        &tm.tm_mon,
        &tm.tm_mday,
        &tm.tm_hour,
        &tm.tm_min,
        &tm.tm_sec);

    tm.tm_year -= 1900;
    tm.tm_mon--;

    return mktime(&tm);
}

std::string GetEnv(const char* envName)
{
    char* podName = new char[0];
    // podName = getenv("HOME");
    if (getenv(envName) != nullptr) {
        podName = getenv(envName);
    }
    return podName;
}

std::string GenTimeNow()
{
    time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%F %X");
    std::cout << ss.str() << std::endl;
    std::string ssTime = ss.str();
    time_t timeUnix = StrTime2unix(ssTime);

    std::stringstream strUnix;
    strUnix << timeUnix;
    std::string ts = strUnix.str();
    return ts;
}

//std::double_t GetElapsed(const std::string &name) const
//{
//    auto end = std::chrono::steady_clock::now();
//    auto wall_clock_ms =
//        std::chrono::duration_cast<std::chrono::milliseconds>(end - this->wall_clock_time_start_)
//            .count();
//    double cpu_time_ms =
//        1000.0 * (static_cast<double>(clock()) - this->cpu_time_start_) / CLOCKS_PER_SEC;
//    double cpu_ratio = static_cast<int>(10000 * (cpu_time_ms / wall_clock_ms)) / 100.0;
//
//    return (wall_clock_ms / 1000.0);
//    std::cout << name << " " << (wall_clock_ms / 1000.0) << " seconds. CPU (" << cpu_ratio << "%) "
//              << Timer::GetNow();
//}

void ReportHttp(const std::string pNum)
{
    // Timer p1;
    // HTTP 请求告诉服务～
    // sleep(3);
    //    std::double_t duringTime = 0;
    //    if (p1.GetElapsed() != 0) {
    //        duringTime = p1.GetElapsed();
    //    }
    //    std::cout << "time ============" << duringTime << "s" << std::endl;
    //    std::ostringstream strDurTime;
    //    strDurTime << duringTime;
    //    //HTTP 请求告诉服务～

    //注释时间函数，由服务端的时间为准，服务端自行获取时间记录
    HttpRequest* Http = nullptr;
    char http_return[4096] = {0};
    char http_msg[4096] = {0};
    // get env
    std::string pod = GetEnv("JOB_POD_NAME");
    std::string node = GetEnv("JOB_NODE_NAME");
//    std::string a = "http://10.1.64.143:8008/ReportChart?pod=" + pod + "&node=" + node+"&p="+pNum;
    std::string a = "http://127.0.0.1:8001/ReportChart?pod=" + pod + "&node=" + node+"&p="+pNum;
    std::strcpy(http_msg, a.data());
    if (Http->HttpGet(http_msg, http_return)) {
        std::cout << "get" << http_return << std::endl;
    }
    //    //HTTP 请求告诉服务～
}
