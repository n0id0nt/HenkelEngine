local Helper_Functions = require("HelperFunctions")
local Movement = require("Movement")
local State_Machine = require("StateMachine")
local Move_State = require("MoveState")
local Dash_State = require("DashState")
local Wall_Slide_State = require("WallSlideState")
local Wall_Jump_State = require("WallJumpState")

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
--INPUT
--------------------------------------------------------------
local input = {
    horizontalInput = 0,
    jumpInput = false,
}

local function getControllerInput()
    input.horizontalInput = Input:getInputAxis("Horizontal")
    input.jumpInput = Input:isInputDown("Jump")
end

--------------------------------------------------------------
--STATES
--------------------------------------------------------------
local stateMachine = State_Machine:new()

local states = {
    moveState = Move_State:new(stateMachine, movement, animations, tryPlayAnimation, input),
    dashState = Dash_State:new(stateMachine, movement, animations, tryPlayAnimation, input),
    wallSlideState = Wall_Slide_State:new(stateMachine, movement, animations, tryPlayAnimation, input),
    wallJumpState = Wall_Jump_State:new(stateMachine, movement, animations, tryPlayAnimation, input),
}

stateMachine.states = states
stateMachine:changeState(states.moveState)

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
    if input.horizontalInput > 0 then
        offset.x = offset.x - cameraLookAhead
    elseif input.horizontalInput < 0 then
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

    local velocity = GO:getPhysicsBody():getVelocity()
    movement:setSpeed(velocity.x, velocity.y)

    stateMachine:update()

    local horizontalSpeed, verticalSpeed = movement:getSpeed()
    GO:getPhysicsBody():setVelocity(vec2.new(horizontalSpeed, verticalSpeed))

    updateCamera()
end

Script.onCollisionEnter = function()
    print("Collision Begin")
end

Script.onCollisionExit = function()
    print("Collision End")
end
