-- Base Script Class for all the entity scripts in the game
Script = class(ENTITY_SCRIPT, nil);

scriptClassCounter = 0;

function Script:Start()
    return 0;
end

function Script:Update()
    scriptClassCounter = scriptClassCounter + 1;
    return 0;
end

function Script:OnDestroy()
    return 0;
end


-- Entity script classes

TestScript = class(Script, { x = 0});

function TestScript:Start()
    return 0;
end

function TestScript:Update()
    x = x + 1;
    return 0;
end

function TestScript:OnDestroy()
    return 0;
end

-- this should be created in C++
--testScript = Script:Create(nil);