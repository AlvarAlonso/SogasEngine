TestScript = class(ENTITY_SCRIPT, nil);

frame = 1;

function TestScript:Start()
    x = "started";
    return 0;
end

function TestScript:Update()
    frame = frame + 1;
    return 0;
end

function TestScript:OnDestroy()
    y = "destroyed";
    return 0;
end

testScript = TestScript:Create(nil);