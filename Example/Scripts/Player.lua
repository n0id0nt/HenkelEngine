--------------------------------------------------------------
--HELPER FUNCTIONS
--------------------------------------------------------------
function MoveTowards(currentValue, targetValue, speed)
    local direction = targetValue - currentValue
    local magnitude = math.abs(direction)

    if magnitude > speed then
        local normalizedDirection = direction / magnitude
        local newValue = currentValue + normalizedDirection * speed
        return newValue
    else
        return targetValue
    end
end

function Clamp(value, minValue, maxValue)
    return math.max(minValue, math.min(value, maxValue))
end

function copyTable(originalTable)
    local copy = {}
    for key, value in pairs(originalTable) do
        if type(value) == "table" then
            copy[key] = copyTable(value) -- recursively copy subtables
        else
            copy[key] = value
        end
    end
    return copy
end

--------------------------------------------------------------
--INPUT
--------------------------------------------------------------
local horrizontalInput = 0
local jumpInput = false
local jumpInputUsed = true
local lastJumpPress = 0

function setHorizontalInput(value)
    horizontalInput = value
end

function setJumpInput(value)
    if value then
        lastJumpPress = Time:getTime()
        if value ~= jumpInput then
            jumpInputUsed = false
        end
    end
    jumpInput = value
end

--------------------------------------------------------------
--MOVE
--------------------------------------------------------------
Script:property("accelerationTime", 0.3)
Script:property("deccelerationTime", 0.2)
Script:property("maxSpeed", 80)
local horizontalSpeed = 0
local curAcceleration = 0

function move()
    curAcceleration = horizontalSpeed / maxSpeed
    local stillDeccelerating = horizontalInput * curAcceleration < 0
    if horizontalInput ~= 0 and (not stillDeccelerating or accelerationTime < deccelerationTime) then
        --acceleration
        local deltaAcceleration = accelerationTime <= 0 and horizontalInput or (horizontalInput * Time:getDeltaTime() / accelerationTime)
        curAcceleration = curAcceleration + deltaAcceleration
        curAcceleration = Clamp(curAcceleration, -1, 1)
    else
        --decceleration
        local deltaAcceleration = deccelerationTime <= 0 and 1 or (Time:getDeltaTime() / deccelerationTime)
        curAcceleration = MoveTowards(curAcceleration, 0, deltaAcceleration)
    end
    horizontalSpeed = curAcceleration * maxSpeed
end

--------------------------------------------------------------
--JUMP
--------------------------------------------------------------
Script:property("jumpDist", 16 * 3)
Script:property("jumpHeight", 16 * 6)
Script:property("jumpArcHeight", 16)
Script:property("jumpArcDist", 16)
Script:property("jumpFallDist", 16 * 2)
Script:property("maxFallSpeed", 300)
Script:property("bufferTime", 100)
Script:property("coyoteTime", 100)
local isGrounded = false
local verticalSpeed = 0
local endJumpEarly = false
local timeLeftGrounded  = 0
local jumping = false
local coyoteUsable = false

function canUseCoyote()
    return coyoteUsable and not isGrounded and (timeLeftGrounded + coyoteTime > Time:getTime()) and lastJumpPress == Time:getTime()
end

function hasBufferJump()
    return isGrounded and (lastJumpPress + bufferTime > Time:getTime())
end

function jumpSpeed()
    return jumpDist ~= 0 and (2 * jumpHeight * maxSpeed) / jumpDist or 0
end

function jumpArcSpeed()
    return jumpArcDist ~= 0 and (2 * jumpArcHeight * maxSpeed) / jumpArcDist or 0
end

function jumpGravity()
    return jumpDist ~= 0 and (2 * jumpHeight * maxSpeed ^ 2) / jumpDist ^ 2 or 0
end

function jumpArcGravity()
    return jumpArcDist ~= 0 and (2 * jumpArcHeight * maxSpeed ^ 2) / jumpArcDist ^ 2 or 0
end

function fallGravity()
    return jumpFallDist ~= 0 and (2 * jumpHeight * maxSpeed ^ 2) / jumpFallDist * 2 or 0
end

-- these are for animation controls
local justJumped = false
local justLanded = false
local justInAir = false

function calculateGravity()
    local newIsGrounded = checkGrounded()
    justLanded = false
    justInAir = false
    if isGrounded ~= newIsGrounded then
        isGrounded = newIsGrounded
        if isGrounded then
            coyoteUsable = true
            justLanded = true
            jumping = false
        else
            timeLeftGrounded = Time:getTime()
            if not jumping then
                justInAir = true
            end
        end
    end
    if not isGrounded then
        local deltaSpeed = (verticalSpeed < 0 and (endJumpEarly and jumpArcGravity() or jumpGravity()) or fallGravity()) * Time:getDeltaTime()
        verticalSpeed = verticalSpeed + deltaSpeed

        -- clamp speed
        if verticalSpeed > maxFallSpeed then 
            verticalSpeed = maxFallSpeed
        end
    end
end

function jump()
    -- can jump if: grounded or within coyote threshold or sufficient jump buffer
    if not jumpInputUsed and (hasBufferJump() or canUseCoyote()) then 
        verticalSpeed = -jumpSpeed()
        jumpInputUsed = true
        endJumpEarly = false
        coyoteUsable = false
        justJumped = true
        jumping = true
    else
        justJumped = false
    end
    -- end jump input once at jump arc
    if verticalSpeed > -jumpArcSpeed() then
        endJumpEarly = true
    end
    -- test to end jump early
    if not isGrounded and not jumpInput and not endJumpEarly then
        endJumpEarly = true
        verticalSpeed = -jumpArcSpeed()
    end
end

--------------------------------------------------------------
--COLLISION
--------------------------------------------------------------
Script:property("groundCheckArea", 1)
Script:property("groundLayer", 1)
Script:property("groundAngle", 25)

function checkGrounded()
    return GO:getPhysicsBody():checkGrounded(groundAngle)
end

--------------------------------------------------------------
--DASH
--------------------------------------------------------------
Script:property("dashSpeed", 1)

local canDash = false
local isDashing = false

function dash()
    canDash = false
    horizontalSpeed = horrizontalInput * dashSpeed
    verticalSpeed = 0
end

--------------------------------------------------------------
--ANIMATION
--------------------------------------------------------------
local runAnimation = "Run"
local dashAnimation = "Dash"
local idleAnimation = "Idle"
local jumpAnimation = "Jump"
local upToFallAnimation = "UpToFall"
local fallAnimation = "Fall"

function controlAnimations()
    if isGrounded then
        if horizontalInput ~= 0 then
            tryPlayAnimation(runAnimation)
        else
            tryPlayAnimation(idleAnimation)
        end
    else
        if verticalSpeed < 0 then
            tryPlayAnimation(jumpAnimation)
        else
            tryPlayAnimation(fallAnimation)
        end
    end
end

function tryPlayAnimation(animation)
    local spriteAnimation = GO:getSpriteAnimation()
    if spriteAnimation:getCurrentAnimation() ~= animation then
        spriteAnimation:playAnimation(animation)
    end
end

--------------------------------------------------------------
--CAMERA
--------------------------------------------------------------
Script:property("cameraLookAhead", 0.3)
Script:property("cameraGroundedOffset", vec2.new(0, 0))
Script:property("cameraInAirOffset", vec2.new(0, 0.3))
Script:property("CameraGroundedDeadZone", vec2.new(0, 0))
Script:property("CameraInAirDeadZone", vec2.new(0, 0.3))

function updateCamera()
    local offset = vec2.copy(isGrounded and cameraGroundedOffset or cameraInAirOffset)
    local deadZone = vec2.copy(isGrounded and CameraGroundedDeadZone or CameraInAirDeadZone)
    if horizontalInput > 0 then
        offset.x = offset.x - cameraLookAhead
    elseif horizontalInput < 0 then
        offset.x = offset.x + cameraLookAhead
    end
    local camera = GO:getCamera()
    camera:setDeadZone(deadZone)
    camera:setOffset(offset)
end

--------------------------------------------------------------
--SCRIPT EVENTS
--------------------------------------------------------------
Script.update = function()

    setHorizontalInput(Input:getInputAxis("Horizontal"))
    if horizontalInput ~= 0 then
        GO:getSprite().flipped = horizontalInput < 0
    end
    setJumpInput(Input:isInputDown("Jump"))

    local velocity = GO:getPhysicsBody():getVelocity();
    horizontalSpeed = velocity.x
    verticalSpeed = velocity.y

    if dashing then
        dash()
    else
        move()
        calculateGravity()
        jump()
    end

    if Input:isInputDown("Shoot") then 
        print("Shoot")
        --local potion = Scene:createTemplatedObject("Template/Potion.tx")
    end

    GO:getPhysicsBody():setVelocity(vec2.new(horizontalSpeed, verticalSpeed))

    controlAnimations()
    updateCamera()
end
