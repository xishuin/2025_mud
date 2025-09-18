#include "Logger.h"
using namespace LOG::utility;
#include <time.h>
#include <stdexcept>
#include <iostream>
#include <stdarg.h>
#include <memory.h>
const char* Logger::s_level[LEVEL_COUNT]={
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"
};

Logger * Logger::m_log=nullptr;

Logger::Logger():m_level(DEBUG),m_len(0),m_max(0)
{
    m_worker=std::thread(&Logger::work,this);//启动后台程序
}
Logger::~Logger(){
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stop=true;
    }
    m_cond.notify_all();
    m_worker.join();//等待后台程序完成输入
    close(); 
}
Logger* Logger::instance(){
    if(m_log==nullptr)
    {
        m_log=new Logger();
    }
    return m_log;
}

void Logger::open(const string& filename){
    m_filename=filename;
    m_fout.open(filename,ios::app);
    if(m_fout.fail()){
        throw std::logic_error("open file failed "+filename);
    }
    m_fout.seekp(0,ios::end);
    m_len=m_fout.tellp();
}


void Logger::close(){
    m_fout.close();
}

void Logger::log(Level level,const char* file,int line,const char* format,...)
{
    if(m_level>level){
        return; 
    }
    if(m_fout.fail()){
        throw std::logic_error("open file failed "+m_filename);
    }
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];//内存的对齐
    memset(timestamp,0,sizeof(timestamp));
    strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",ptm);

    const char* fmt="%s %s %s:%d ";
    int size=snprintf(NULL,0,fmt,timestamp,s_level[level],file,line);
    char* buffer =new char[size+1];
    if(size>0){
        snprintf(buffer,size+1,fmt,timestamp,s_level[level],file,line);
        buffer[size]=0;
        m_len+=size;
    }
    va_list arg_ptr;
    va_start(arg_ptr,format);
    size=vsnprintf(NULL,0,format,arg_ptr);
    va_end(arg_ptr);
    char * content =new char[size+1];
    if(size>0){
        va_start(arg_ptr,format);
        vsnprintf(content,size+1,format,arg_ptr);
        va_end(arg_ptr);
        m_len+=size;
    }
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(string(buffer)+string(content));
        m_cond.notify_one();
    }
    delete[] buffer;
    delete[] content;
}

void Logger::rotate(){
    close();
    time_t ticks = time(NULL);
    struct tm* ptm = localtime(&ticks);
    char timestamp[32];//内存的对齐
    memset(timestamp,0,sizeof(timestamp));
    strftime(timestamp,sizeof(timestamp),"%Y-%m-%d %H:%M:%S",ptm);
    string filename=m_filename+timestamp;
    if(rename(m_filename.c_str(),filename.c_str())!=0){
        throw std::logic_error("rename log file failed");
    }
    open(m_filename);
}

void Logger::work(){
    while(true){
        string content;
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock,[this]{return !m_queue.empty()|| !m_stop;});
            if(m_queue.empty() && m_stop){break;}
            content=m_queue.front();
            m_queue.pop();
        }
        m_fout << content << "\n";
        m_fout.flush();
    }
}