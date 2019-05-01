
os.execute("pwd")

ddcl.callback(function(...)
    print(...)
end)

local p, sz = ddcl.malloc(10)
ddcl.send(ddcl.self(), p, sz)
