local WallSlideState = {}

function WallSlideState:new(stateMachine, movement, animations, playAnimation)
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    self.stateMachine = stateMachine
    self.movement = movement
    self.animations = animations
    self.playAnimation = playAnimation
    return obj
end

function WallSlideState:enter()
    GO:getSpriteAnimation():playAnimation(self.animations.wallSlide)
    self.movement.isWallSliding = true
end

function WallSlideState:exit()
    self.movement.isWallSliding = false
end

function WallSlideState:update()
    self.movement:calculateGravity()

    if self.movement.isGrounded or GO:getPhysicsBody():checkGrounded(180, groundAngle) then
        self.stateMachine:changeState(self.stateMachine.states.moveState)
    elseif Input:isInputDown("Jump") then
        self.stateMachine:changeState(self.stateMachine.states.dashState)
    end
end

return WallSlideState