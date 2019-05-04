require("class")

TestScript = class()

function TestScript:init(gameobject)

    self.gameobject = gameobject
    --local name = go:get_name()
end

function TestScript:tick(dt)

    local go = self.gameobject
    local pos = go:get_pos()
    --Simple test, comment line below, and press R to reload the script,
    --The gameObject that it its attaached to will stop updating, bird in this case
    pos.z = pos.z - 25
    go:set_pos(pos)
    --print(name .. " " .. go.x .. "," .. go.y)
end


function register_script()
    return TestScript
end