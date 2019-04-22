
    print("for", i)
    ddcl.co_fork(function()
        ddcl.log("co_fork")
        local fd, cmd = ddcl.connect_socket("127.0.0.1", 8001)
        ddcl.log("connect:", fd, cmd)
        while true do
            ddcl.log("read", ddcl.read_socket(fd, 0))
        end
        --[[
        ]]
    end)
    --ddcl.co_sleep(5)
for i = 1, 1 do
end
