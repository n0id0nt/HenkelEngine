Script.update = function()
    local layout = GO:getUILayout()
    zombie = layout:findArea("Zombie")
    local newX = zombie:getX() + 1 * Time:getDeltaTime()
    --print(newX)
    zombie:setX(newX)
    GO:getRenderer():invalidateBatch()
end