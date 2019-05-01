local inspect = require "inspect"

ddcl.callback(function(info, ...)
    local t = ...
    ddcl.log(inspect(t))
    print("cb:", ...)
end)

local t = {
    --99, "hello world!",
    --name = "diandian",
    sub = { 1, 2, 3},
}
ddcl.send(ddcl.self(), t)
