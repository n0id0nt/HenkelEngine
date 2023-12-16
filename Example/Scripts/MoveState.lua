local MoveState = {}

function MoveState:new(stateMachine, movement, animations, playAnimation)
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    self.stateMachine = stateMachine
    self.movement = movement
    self.animations = animations
    self.playAnimation = playAnimation
    return obj
end

function MoveState:controlAnimations()
    if self.movement.isGrounded then
        if self.movement.horizontalInput ~= 0 then
            self.playAnimation(self.animations.run)
        else
            self.playAnimation(self.animations.idle)
        end
    else
        if self.movement.verticalSpeed < 0 then
            self.playAnimation(self.animations.jump)
        else
            self.playAnimation(self.animations.fall)
        end
    end
end

function MoveState:enter()

end

function MoveState:exit()

end

function MoveState:update()
    self.movement:move()
    self.movement:calculateGravity()
    self.movement:jump()

    self:controlAnimations()

    if Input:isInputDown("Dash") then
        self.stateMachine:changeState(self.stateMachine.states.dashState)
    end
end

return MoveState