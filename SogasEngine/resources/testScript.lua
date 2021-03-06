-- Base Script Class for all the entity scripts in the game
Script = class(ENTITY_SCRIPT, nil);

function Script:Start()
    return 0;
end

function Script:Update(dt)
    return 0;
end

function Script:OnDestroy()
    return 0;
end

-- Entity script classes

TestScript = class(Script, { x = 0, y = 3, z = 1, name = "jijiji", number = 0.4, transform = {} });

function TestScript:Start()
    return 0;
end

function TestScript:Update(dt)
    local x = 0;
    if(IsKeyPressed(SGS_KEY_A)) then
        x = 5 * dt;
    end
    if(IsKeyPressed(SGS_KEY_D)) then
        x = -5 * dt;
    end

    self.transform = GetTransform(self);
    self.transform.x = self.transform.x + x;
    SetTransform(self);

    return 0;
end

function TestScript:OnDestroy()
    return 0;
end

-- this should be created in C++
--testScript = Script:Create(nil);