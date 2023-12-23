local Helper_Functions = require("HelperFunctions")
local Movement = require("Movement")
local State_Machine = require("StateMachine")
local Move_State = require("MoveState")
local Dash_State = require("DashState")
local Wall_Slide_State = require("WallSlideState")

local movement = Movement:new()
--------------------------------------------------------------
--ANIMATION
--------------------------------------------------------------
local animations = {
    run = "Run",
    dash = "Dash",
    idle = "Idle",
    jump = "Jump",
    upToFall = "UpToFall",
    fall = "Fall",
    wallSlide = "WallSlide",
}

function tryPlayAnimation(animation)
    local spriteAnimation = GO:getSpriteAnimation()
    if spriteAnimation:getCurrentAnimation() ~= animation then
        spriteAnimation:playAnimation(animation)
    end
end

--------------------------------------------------------------
--STATES
--------------------------------------------------------------
local stateMachine = State_Machine:new()

local states = {
    moveState = Move_State:new(stateMachine, movement, animations, tryPlayAnimation),
    dashState = Dash_State:new(stateMachine, movement, animations, tryPlayAnimation),
    wallSlideState = Wall_Slide_State:new(stateMachine, movement, animations, tryPlayAnimation),
}

stateMachine.states = states
stateMachine:changeState(states.moveState)

--------------------------------------------------------------
--INPUT
--------------------------------------------------------------
local horizontalInput = 0
local jumpInput = false

local function getControllerInput()
    horizontalInput = Input:getInputAxis("Horizontal")
    jumpInput = Input:isInputDown("Jump")
end


--------------------------------------------------------------
--CAMERA
--------------------------------------------------------------
Script:property("cameraLookAhead", 0.3)
Script:property("cameraGroundedOffset", vec2.new(0, -0.3))
Script:property("cameraInAirOffset", vec2.new(0, -0.1))
Script:property("CameraGroundedDeadZone", vec2.new(0, 0))
Script:property("CameraInAirDeadZone", vec2.new(0, 0.4))

function updateCamera()
    local offset = vec2.copy(movement.isGrounded and cameraGroundedOffset or cameraInAirOffset)
    local deadZone = vec2.copy(movement.isGrounded and CameraGroundedDeadZone or CameraInAirDeadZone)
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
    getControllerInput()
    movement:setHorizontalInput(horizontalInput)
    if horizontalInput ~= 0 then
        GO:getSprite().flipped = horizontalInput < 0
    end
    movement:setJumpInput(jumpInput)

    local velocity = GO:getPhysicsBody():getVelocity()
    movement:setSpeed(velocity.x, velocity.y)

    stateMachine:update()

    local horizontalSpeed, verticalSpeed = movement:getSpeed()
    GO:getPhysicsBody():setVelocity(vec2.new(horizontalSpeed, verticalSpeed))

    updateCamera()
end
