

Script.update = function()
    if Input:isKeyJustReleased("e") then
        local collisionBody = GO:getTileMapCollisionBodyComponent()
        local newValue = not collisionBody:getIsSensor()
        print("platform switch", newValue)
        collisionBody:setIsSensor(newValue)
    end
end

Script.onCollisionEnter = function(contact)
    
end