--local player = require("player")

local stateMachine

local gameObjectScripts = {
    Player = function(player)
        print(player:getName())
    end
}

Script.update = function()
    -- run a state machine to control different components of the game through game states. 
    -- itterate through the scripts of each game object in the game ie player each enemy througg the tagging systen that may of may not be done
    -- here ill write a tet script to draw the player 

    for tag, script in pairs(gameObjectScripts) do
        entities = World:getEntitiesWithTag(tag)
        for _, entity in ipairs(entities) do 
            script(entity)
        end
    end
end