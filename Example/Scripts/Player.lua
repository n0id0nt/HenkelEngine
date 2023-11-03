local speed = 80
local groundAngle = 30
local gravity = 200 
local jumpSpeed = 60

function update(deltaTime)
    local verticalSpeed = GO:getPhysicsBody():getVelocity().y;
    --print("old speed ", verticalSpeed)
	local horrizontalSpeed = 0;

    local horrizontalDir = Input:getArrowDir().x
    --print("input dir ", horrizontalDir)
    local horrizontalSpeed = horrizontalDir * speed

    local isGrounded = GO:getPhysicsBody():checkGrounded(groundAngle)
    --print("grounded ", isGrounded)
    if not isGrounded then 
        verticalSpeed = verticalSpeed + gravity * deltaTime
    end

    if Input:isInputDown("Jump") and isGrounded then 
        print("Jump")
        verticalSpeed = verticalSpeed - jumpSpeed
    end

    if Input:isInputDown("Shoot") then 
        print("Shoot")
        local potion = Scene:createTemplatedObject("Template/Potion.tx")
    end

    GO:getPhysicsBody():setVelocity(vec2.new(horrizontalSpeed, verticalSpeed))
end