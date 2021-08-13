Script = class(ENTITY_SCRIPT, nil);

frame = 1;

function Script:Start()
    x = "started";
    return 0;
end

function Script:Update()
    frame = frame + 1;
    return 0;
end

function Script:OnDestroy()
    y = "destroyed";
    return 0;
end

-- this should be created in C++
--testScript = Script:Create(nil);