
local listen = ddcl.listen_socket("0.0.0.0", 8001, 1000)
print("listen:", listen)

ddcl.co_fork(function()
    while true do
        local fd, cmd = ddcl.accept_socket(listen)
        print(fd, cmd)
    end
end)
