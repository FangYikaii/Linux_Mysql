#!/bin/bash

echo "Hello World!"


#从键盘读输入，取变量：前面加$
echo "What is your name?"
read PERSON
echo “Hello,$PERSON”

#输出
a="This is $ example"
echo "A is"
echo $a

num=2
echo "This is the ${num}nd"

#  1.$#:传入脚本的命令行参数
#  2.$*:所有命令行的参数值，在各个参数值留有空格
#  3.$0:命令本身
#  4.$1:第一个命令行参数
#  5.$2:第二个命令行参数

echo "Number of vars:"$#
echo "value of vars:"$*
echo "value of var1:"$0
echo "value of var2:"$1
echo "value of var3:"$2
