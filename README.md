ftpsearch  ![ci](https://travis-ci.com/kikilizhm/ftpsearch.svg?branch=master "CI")  
=========

c语言写一个小型的ftp服务器搜索引擎爬虫，然后供web方式搜索

爬虫：C语言socket+sqlite3本地数据库

web server：boa + C语言cgi + sqlite3数据库

构建：
1. make Cftps 生成Cftps主程序
   1）显示当前爬取的ftp服务器  ./Cftps ls
   2) 删除某个服务器信息   ./Cftps del x （x为ls命令显示的序号）
   3）增加服务器信息       ./Cftps add "192.168.1.103:21:user:name:ftp info desc" 
   4）爬取服务器信息       ./Cftps search
   
2. web server 端，cgi程序使用make a.cgi 生成，web服务器为boa，在tools/boaxxx/src/目录下的boa执行即可。
web 服务器主目录为web目录下，通过简单select命令查询结果
