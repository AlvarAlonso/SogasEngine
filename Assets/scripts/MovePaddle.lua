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

MovePaddle = class(Script);

function MovePaddle:Start()
    return 0;
end

function MovePaddle:Update(dt)
    local y = 0;
    if(IsKeyPressed(SGS_KEY_W)) then
        y = 5 * dt;
    end
    if(IsKeyPressed(SGS_KEY_S)) then
        y = -5 * dt;
    end

    self.transform = GetTransform(self);
    self.transform.y = self.transform.y + y;
    SetTransform(self);

    return 0;
end

function MovePaddle:OnDestroy()
    return 0;
end