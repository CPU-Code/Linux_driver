# Linux_driver

## 介绍
Linux驱动开发

## 文件说明

inlcude:

    Linux驱动开发的头文件

1chrdevbase：

    字符驱动开发的模板

2Led：

    LED驱动开发

3newchrled ：

    新LED字符驱动开发

## 使用说明

输入如下命令编译出驱动模块文件：

```shell
make
```
编译成功以后就会生成一个名为“xxx.ko”的驱动模块文件

输入如下命令编译测试 xxxAPP.c 这个测试程序
```shell
arm-linux-gnueabihf-gcc xxxAPP.c -o xxxApp
```
编译成功以后就会生成 xxxApp 这个应用程序

输入如下命令查看xxxAPP 这个程序的文件信息
```shell
file xxxApp
```

编译出来的 xxx.ko 和 xxxApp 这两个文件拷贝到 指定 目录中
```shell
sudo cp xxx.ko xxxApp /home/xxx -f
```

重启开发板，进入到指定的目录
```shell
cd /home/xxx
```

第一次加载驱动的时候需要运行此命令
```shell
depmod
```
驱动加载成功以后创建“/dev/xxx”设备节点

使用 modprobe 加载 xxx.ko
```shell
modprobe xxx.ko
````

输入“ lsmod ”命令即可查看当前系统中存在的模块
```shell
lsmod
````

创建设备节点文件
```shell
mknod /dev/xxx 字符设备 主设备号 次设备号
```

设备操作测试
```shell
./xxxApp /dev/xxx 参数
```

卸载驱动模块
```shell
rmmod xxx.ko
```
