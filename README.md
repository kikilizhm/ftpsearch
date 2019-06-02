ftpsearch  ![ci](https://travis-ci.com/kikilizhm/ftpsearch.svg?branch=master "CI")  
=========

打算使用c语言写一个小型的ftp服务器搜索引擎爬虫，然后供web方式搜索

构建：
1. make Cftps 生成Cftps主程序，使用方法 ./Cftps add 192.168.1.103:23
2. web server 端，cgi程序使用make a.cgi 生成，web服务器为boa，在tools/boaxxx/src/目录下的boa执行即可。



ftp命令字参考 http://www.cnblogs.com/zhili/archive/2012/10/18/FTPServer.html 
ftp实现示例 https://blog.csdn.net/kikilizhm/article/details/8192596
