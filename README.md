# ddcl
Dark Dragon Engine core library,
A lightweight multi thread concurrent network framework.


## 消息队列和线程
核心设计参考云风的 skynet https://github.com/cloudwu/skynet,
框架提供统一的跨平台 api 对线程和网络的访问，框架自行管理一些线程：
工作线程，网络线程，计时器线程，监控线程。也可以自己创建自定义的线程。

线程之间采用消息队列方式通信,单个消息队列称作一个"服务",
服务有两种工作方式:
1, 框架管理的工作线程自行运行,多个工作服务组成一个服务队列,
工作线程轮流从服务队列里面取出一个运行一条或多条消息,运行结束后将服务添加到队列末尾.
2, 自定义线程服务封装,通过创建非"工作服务"可以让自定义线程和其他线程/工作服务通信.


## 定时器
框架提供毫秒级的定时器，服务可以向定时器注册一个超时响应，
超时后服务可以收到一条 "DDCL_CMD_TIMEOUT" 命令的消息。


## 网络管理
框架接管所有网络连接,并通过消息传递操作和通知.
单独的网络线程管理所有连接,并自建id给上层隐藏底层 fd,
底层支持 LINUX 的 epoll, BSD 的kevent, Windows 支持 select 的模拟。
所有网络线程的消息都会发送到相应的操作服务,统一的消息命令为 "DDCL_CMD_SOCKET".


## lua 绑定
框架采用 lua 作为脚本语言,利用 lua 的 coroutine 机制将异步调用手动挂起,
收到回复后继续运行.可以让一个异步调用看起来像是一个函数调用.

## BUILD

### WINDOWS
```
cmake -DBUILD_SHARED_LIBS=1
MSBuild ddcl.sln
```

### LINUX
```
cmake -DLINUX=1 -DBUILD_SHARED_LIBS=1
make
```

### MACOS
```
cmake -DAPPLE=1 -DBUILD_SHARED_LIBS=1
make
```



## EXAMPLE
``` lua
local conf = {
    worker = 1, --工作线程数量
    socket = 1, -- 网络管理线程数量
    timer_ms = 1, -- 计时器线程休眠毫秒数
}
ddcl.init(conf)

-- true 表示当前是主线程服务，退出服务器后不会自动 close lua 虚拟机
-- function 会在新的 coroutine 里面运行
ddcl.start_non_worker(true , function(data, sz)
    ddcl.log("start main service:", ddcl.self())
    -- 注册一个消息接收回调
    ddcl.callback(function(data, sz, ptype, cmd, session, self, from)
        ddcl.log("some msg to callback",
            data, sz, ptype, cmd, session, self, from)
        if session > 0 then
            ddcl.resp("resp success")
        end
    end)

    ddcl.send(ddcl.self(), "send self a msg")
    ddcl.call(ddcl.self(), "call self a msg")

    ddcl.fork(function()
        ddcl.log("run in new coroutine")
    end)
    ddcl.timeout(1000, function()
        ddcl.log("run in after 1000ms timeout")
    end)

    ddcl.co_sleep(1000)
    ddcl.log("run in after 1000ms sleep")

    local listen = ddcl.listen_socket("0.0.0.0", 10000, 1000)
    assert(listen > 0, "error listen")
    local close = false
    while not close do
        local rsp, sz = ddcl.accept_socket(listen)
        local fd, cmd = ddcl.parse_socket_rsp(rsp, sz)
        if cmd == ddcl.DDCL_SOCKET_ERROR then
            break
        end
        ddcl.fork(function()
            ddcl.send_socket(fd, "hello client\n")
            while true do
                local rsp, sz = ddcl.read_socket(fd, 0)
                local fd, cmd, data = ddcl.parse_socket_rsp(rsp, sz)
                if cmd == ddcl.DDCL_SOCKET_ERROR then
                    break
                end
                if string.sub(data, 1, 5) == "close" then
                    ddcl.log("close socket:", fd)
                    ddcl.close_socket(fd)
                    break
                elseif string.sub(data, 1, 5) == "final" then
                    ddcl.log("final socket:", fd)
                    ddcl.close_socket(fd)
                    ddcl.close_socket(listen)
                    close = true
                    break
                else
                    ddcl.log("recv socket:", fd, data)
                    ddcl.send_socket(fd, "rsp pong\n")
                end
            end

        end)
    end
    ddcl.exit()
end)
ddcl.final()
```
#### RUN
lddcl main.lua
