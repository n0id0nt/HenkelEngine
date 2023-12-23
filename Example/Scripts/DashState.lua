local DashState = {}

Script:property("dashTime", 500)

function DashState:new(stateMachine, movement, animations, playAnimation)
    local obj = {}
    setmetatable(obj, self)
    self.__index = self
    self.stateMachine = stateMachine
    self.movement = movement
    self.animations = animations
    self.playAnimation = playAnimation
    return obj
end

function DashState:enter()
    GO:getSpriteAnimation():playAnimation(self.animations.dash)
    self.startTime = Time:getTime()
    GO:getCamera():addShakeTrauma(0.2)
end

function DashState:exit()

end

function DashState:update()
    self.movement:dash()
    if self.startTime + dashTime < Time:getTime() then
        self.stateMachine:changeState(self.stateMachine.states.moveState)
    end
end

return DashState