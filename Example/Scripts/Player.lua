function update(deltaTime)
    print("dt = ", deltaTime)

    if Input:isInputDown("Jump") then
        print(GO:Test())
    else
        print("NO")
    end
end