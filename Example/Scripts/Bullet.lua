local speed = 80
local groundAngle = 30
local gravity = 200 

Script.update = function(deltaTime)
    local verticalSpeed = GO:getPhysicsBody():getVelocity().y;
    --print("old speed ", verticalSpeed)
	local horrizontalSpeed = 0;

    local horrizontalDir = 1
    --print("input dir ", horrizontalDir)
    local horrizontalSpeed = horrizontalDir * speed

    local isGrounded = GO:getPhysicsBody():checkGrounded(groundAngle)
    --print("grounded ", isGrounded)
    if not isGrounded then 
        verticalSpeed = verticalSpeed + gravity * deltaTime
    else
        speed = 0
    end

    GO:getPhysicsBody():setVelocity(vec2.new(horrizontalSpeed, verticalSpeed))
end