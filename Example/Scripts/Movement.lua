local Helper_Functions = require("HelperFunctions")

local Movement = {}

function Movement:new()
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    --input
    obj.horizontalInput = 0
    obj.jumpInput = false
    obj.jumpInputUsed = true
    obj.lastJumpPress = 0
    --move
    obj.horizontalSpeed = 0
    obj.curAcceleration = 0
    obj.facing = 1
    --jump
    obj.isGrounded = false
    obj.verticalSpeed = 0
    obj.endJumpEarly = false
    obj.timeLeftGrounded  = 0
    obj.jumping = false
    obj.coyoteUsable = false
    -- these are for animation controls
    obj.justJumped = false
    obj.justLanded = false
    obj.justInAir = false
    --dash
    obj.canDash = false
    obj.isDashing = false
    return obj
end

--------------------------------------------------------------
--INPUT
--------------------------------------------------------------
function Movement:setHorizontalInput(value)
    self.horizontalInput = value
    if value ~= 0 then
        self.facing = value
    end
end

function Movement:setJumpInput(value)
    if value then
        self.lastJumpPress = Time:getTime()
        if value ~= self.jumpInput then
            self.jumpInputUsed = false
        end
    end
    self.jumpInput = value
end

--------------------------------------------------------------
--MOVE
--------------------------------------------------------------
Script:property("accelerationTime", 0.3)
Script:property("deccelerationTime", 0.2)
Script:property("maxSpeed", 80)

function Movement:move()
    self.curAcceleration = self.horizontalSpeed / maxSpeed
    local stillDeccelerating = self.horizontalInput * self.curAcceleration < 0
    if self.horizontalInput ~= 0 and (not stillDeccelerating or accelerationTime < deccelerationTime) then
        --acceleration
        local deltaAcceleration = accelerationTime <= 0 and self.horizontalInput or (self.horizontalInput * Time:getDeltaTime() / accelerationTime)
        self.curAcceleration = self.curAcceleration + deltaAcceleration
        self.curAcceleration = Helper_Functions.Clamp(self.curAcceleration, -1, 1)
    else
        --decceleration
        local deltaAcceleration = deccelerationTime <= 0 and 1 or (Time:getDeltaTime() / deccelerationTime)
        self.curAcceleration = Helper_Functions.MoveTowards(self.curAcceleration, 0, deltaAcceleration)
    end
    self.horizontalSpeed = self.curAcceleration * maxSpeed
end

--------------------------------------------------------------
--WALLSLIDE
--------------------------------------------------------------
Script:property("wallSideMaxFallSpeed", 10)

Movement.isWallSliding = false

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

function Movement:canUseCoyote()
    return self.coyoteUsable and not self.isGrounded and (self.timeLeftGrounded + coyoteTime > Time:getTime()) and self.lastJumpPress == Time:getTime()
end

function Movement:hasBufferJump()
    return self.isGrounded and (self.lastJumpPress + bufferTime > Time:getTime())
end

function Movement:jumpSpeed()
    return jumpDist ~= 0 and (2 * jumpHeight * maxSpeed) / jumpDist or 0
end

function Movement:jumpArcSpeed()
    return jumpArcDist ~= 0 and (2 * jumpArcHeight * maxSpeed) / jumpArcDist or 0
end

function Movement:jumpGravity()
    return jumpDist ~= 0 and (2 * jumpHeight * maxSpeed ^ 2) / jumpDist ^ 2 or 0
end

function Movement:jumpArcGravity()
    return jumpArcDist ~= 0 and (2 * jumpArcHeight * maxSpeed ^ 2) / jumpArcDist ^ 2 or 0
end

function Movement:fallGravity()
    return jumpFallDist ~= 0 and (2 * jumpHeight * maxSpeed ^ 2) / jumpFallDist * 2 or 0
end

function Movement:calculateGravity()
    local newIsGrounded = self:checkGrounded()
    self.justLanded = false
    self.justInAir = false
    if self.isGrounded ~= newIsGrounded then
        self.isGrounded = newIsGrounded
        if self.isGrounded then
            self.coyoteUsable = true
            self.justLanded = true
            self.jumping = false
        else
            self.timeLeftGrounded = Time:getTime()
            if not self.jumping then
                self.justInAir = true
            end
        end
    end
    if not self.isGrounded then 
        local deltaSpeed = (self.verticalSpeed < 0 and (self.endJumpEarly and self:jumpArcGravity() or self:jumpGravity()) or self:fallGravity()) * Time:getDeltaTime()
        self.verticalSpeed = self.verticalSpeed + deltaSpeed

        -- clamp speed
        if self.isWallSliding and self.verticalSpeed > wallSideMaxFallSpeed then
            self.verticalSpeed = wallSideMaxFallSpeed
        elseif self.verticalSpeed > maxFallSpeed then 
            self.verticalSpeed = maxFallSpeed
        end
    end
end

function Movement:jump()
    -- can jump if: grounded or within coyote threshold or sufficient jump buffer
    if not self.jumpInputUsed and (self:hasBufferJump() or self:canUseCoyote()) then 
        self.verticalSpeed = -self:jumpSpeed()
        self.jumpInputUsed = true
        self.endJumpEarly = false
        self.coyoteUsable = false
        self.justJumped = true
        self.jumping = true
    else
        self.justJumped = false
    end
    -- end jump input once at jump arc
    if self.verticalSpeed > -self:jumpArcSpeed() then
        self.endJumpEarly = true
    end
    -- test to end jump early
    if not self.isGrounded and not self.jumpInput and not self.endJumpEarly then
        self.endJumpEarly = true
        self.verticalSpeed = -self:jumpArcSpeed()
    end
end

--------------------------------------------------------------
--COLLISION
--------------------------------------------------------------
Script:property("groundCheckArea", 1)
Script:property("groundLayer", 1)
Script:property("groundAngle", 25)

function Movement:checkGrounded()
    return GO:getPhysicsBody():checkGrounded(groundAngle)
end

--------------------------------------------------------------
--DASH
--------------------------------------------------------------
Script:property("dashSpeed", 500)

function Movement:dash()
    self.canDash = false
    self.horizontalSpeed = self.facing * dashSpeed
    self.verticalSpeed = 0
end

--------------------------------------------------------------
--HELPER
--------------------------------------------------------------
function Movement:setSpeed(x, y)
    self.horizontalSpeed = x
    self.verticalSpeed = y
end

function Movement:getSpeed()
    return self.horizontalSpeed, self.verticalSpeed
end

return Movement