#########################################################################
# File Name: run.sh
# Author: wrf
# mail: wrf6758@qq.com
# Created Time: 2022年09月18日 星期日 08时21分01秒
#########################################################################
#!/bin/bash
gcc client.c -o client -lpthread
gcc server.c -o server -lpthread

