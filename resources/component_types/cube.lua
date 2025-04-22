cube = {
  -- Variables to track animation
  radius = 0.1,
  speed = 0.09, -- rotation amount per frame
  camMoveSpeed = 0.3,
  camAngleSpeed = 1,
  pos = Vector3(0, 0, 0),
  
  OnStart = function(self)
    Camera.SetPosition(0,0,0.5)
  end,

  OnUpdate = function(self)
    local xPos = Camera.GetPositionX()
    local yPos = Camera.GetPositionY()
    local zPos = Camera.GetPositionZ()

    -- Get current frame number
    local frame = Application.GetFrame()
    
    -- Calculate angle based on frame count
    local angle = frame * self.speed
    
    -- Calculate position on the circle
    local x = 0
    local z = 1
    local y = 1

    -- Forward/backward movement
    if Input.GetKey("w") then
      zPos = zPos - self.camMoveSpeed  -- Move forward (negative Z)
    end
    
    if Input.GetKey("s") then
      zPos = zPos + self.camMoveSpeed  -- Move backward (positive Z)
    end
    
    -- Left/right movement
    if Input.GetKey("a") then
      xPos = xPos - self.camMoveSpeed  -- Move left (negative X)
    end
    
    if Input.GetKey("d") then
      xPos = xPos + self.camMoveSpeed  -- Move right (positive X)
    end
    
    -- Up/down movement
    if Input.GetKey("space") then
      yPos = yPos + self.camMoveSpeed  -- Move up (positive Y)
    end
    
    if Input.GetKey("lctrl") or Input.GetKey("rctrl") then
      yPos = yPos - self.camMoveSpeed  -- Move down (negative Y)
    end

    if Input.GetKey("escape") then
      Application.Quit()
    end

    -- Get current angles
    local yaw = Camera.GetYaw()
    local pitch = Camera.GetPitch()
    
    -- Adjust angles based on input
    if Input.GetKey("right") then
        Camera.SetYaw(yaw + self.camAngleSpeed)
    end
    if Input.GetKey("left") then
        Camera.SetYaw(yaw - self.camAngleSpeed)
    end
    if Input.GetKey("up") then
        Camera.SetPitch(pitch + self.camAngleSpeed)
    end
    if Input.GetKey("down") then
        Camera.SetPitch(pitch - self.camAngleSpeed)
    end

    if Input.GetKey("i") then
      self.pos.y = self.pos.y + self.speed
    end
    if Input.GetKey("j") then
      self.pos.x = self.pos.x - self.speed
    end
    if Input.GetKey("k") then
      self.pos.y = self.pos.y - self.speed
    end
    if Input.GetKey("l") then
      self.pos.x = self.pos.x + self.speed
    end
    
    -- Update camera position
    Camera.SetPosition(xPos, yPos, zPos)

    Model.DrawCube(0.1, self.pos)
    Model.DrawSphere(self.radius, Vector3(0.8, 0.5, 0.0), 16, Vector3(z, x, y))
    local scale = Vector3(0.1, 0.1, 0.1)
    -- Original trees
    Model.DrawModel("tree", Vector3(1, -4.8, 0), scale)
    Model.DrawModel("tree", Vector3(0, -4.8, 2), scale)
    Model.DrawModel("tree", Vector3(2, -4.8, 1), scale)
    Model.DrawModel("tree", Vector3(4, -4.8, 2), scale)
    Model.DrawModel("tree", Vector3(-2, -4.8, -1), scale)
    Model.DrawModel("tree", Vector3(-3, -4.8, 3), scale)
    Model.DrawModel("tree", Vector3(-1, -4.8, 4), scale)
    Model.DrawModel("tree", Vector3(3, -4.8, -2), scale)
    Model.DrawModel("tree", Vector3(2, -4.8, -3), scale)
    Model.DrawModel("tree", Vector3(-4, -4.8, 0), scale)
    Model.DrawModel("tree", Vector3(0, -4.8, -3), scale)
    Model.DrawModel("tree", Vector3(-2, -4.8, -4), scale)
    Model.DrawModel("tree", Vector3(3, -4.8, 4), scale)
    Model.DrawModel("tree", Vector3(-3, -4.8, -3), Vector3(0.12, 0.12, 0.12))
    Model.DrawModel("tree", Vector3(4, -4.8, -1), Vector3(0.08, 0.08, 0.08))

    Model.DrawTexturedPlane("dirt.jpg", 10.0, 10.0, Vector3(0.5, 0.8, 0.5), Vector3(0, -5, 0))
  end
}