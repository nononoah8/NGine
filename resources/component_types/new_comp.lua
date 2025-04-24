new_comp = {
  pos = Vector3(0, 0, 0),
  size = 0.1,

  OnStart = function(self)
    Debug.Log("Create this compnoent!!!")
  end,

  OnUpdate = function(self)
    Model.DrawCube(self.size, self.pos)
  end
}