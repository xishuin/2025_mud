#pragma once
#include <string>
#include <fstream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
namespace LOG{
namespace utility{
#define debug(format,...) \
    Logger::instance()->log(Logger::DEBUG,__FILE__,__LINE__,format,##__VA_ARGS__)
#define info(format,...) \
    Logger::instance()->log(Logger::INFO,__FILE__,__LINE__,format,##__VA_ARGS__)
#define warn(format,...) \
    Logger::instance()->log(Logger::WARN,__FILE__,__LINE__,format,##__VA_ARGS__)
#define error(format,...) \
    Logger::instance()->log(Logger::ERROR,__FILE__,__LINE__,format,##__VA_ARGS__)
#define fatal(format,...) \
    Logger::instance()->log(Logger::FATAL,__FILE__,__LINE__,format,##__VA_ARGS__)

class Logger{

public:
    enum Level{
        DEBUG=0,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
    };
    static Logger* instance();
    void open(const string& filename);
    void close();
    void log(Level level,const char* file,int line,const char* format,...);
    void level(int level)
    {
        m_level=static_cast<Level>(level);
    }
    void max(int num)
    {
        m_max=num;
    }
private:
    Logger();
    void rotate();
    ~Logger();
private:
    //通用属性
    string m_filename;
    ofstream m_fout;
    Level m_level;
    int m_len;
    int m_max;
    static const char* s_level[LEVEL_COUNT];
    static Logger* m_log;
private:
    // 新增异步相关成员
    std::queue<std::string> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::thread m_worker;
    bool m_stop = false;
    void work(); // 后台线程工作函数
};
}
}