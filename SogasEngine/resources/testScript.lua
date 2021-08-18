-- Base Script Class for all the entity scripts in the game
Script = class(ENTITY_SCRIPT, nil);

function Script:Start()
    return 0;
end

function Script:Update()
    return 0;
end

function Script:OnDestroy()
    return 0;
end

-- Entity script classes

TestScript = class(Script, { x = 5, y = 3, z = 1, name = "jijiji", number = 0.4 });

function TestScript:Start()
    return 0;
end

function TestScript:Update()
    if(IsKeyPressed(65)) then
        self.x = self.x + 1;
    end
    return 0;
end

function TestScript:OnDestroy()
    return 0;
end

-- this should be created in C++
--testScript = Script:Create(nil);