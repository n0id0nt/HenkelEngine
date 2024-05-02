local eventResponses = 
{
    damage = function(message, sender)
        print("Message Received ---DAMAGE---")
        print(message)
        local layout = GO:getUILayout()
        zombie = layout:findArea("zombie")
        local newX = zombie:getX() + 100
        --print(newX)
        zombie:setX(newX)
        -- this forces the ui to update the render buffer need to think of a better way to do this as this should not need to be manually done
        GO:getRenderer():invalidateBatch()
    end,
}


Script.update = function()
    --local layout = GO:getUILayout()
    --zombie = layout:findArea("zombie")
    --local newX = zombie:getX() + 1 * Time:getDeltaTime()
    ----print(newX)
    --zombie:setX(newX)
    ---- this forces the ui to update the render buffer need to think of a better way to do this as this should not need to be manually done
    --GO:getRenderer():invalidateBatch()
end

Script.onMessage = function(messageId, message, sender)
    local responce = eventResponses[messageId]
    if responce then
        responce(message, sender)
    end
end