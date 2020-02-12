
-- Do not delete this file

pos = Vec2.New(200.0)
size = Vec2.New(50.0)
col = Vec3.New(1.0)

game = {
	update = function()
		while(true) do
			ImGui.Begin("Lua ImGui")
			ImGui.SliderVec2("Position", pos, Vec2.New(0.0), Vec2.New(2000.0))
			ImGui.SliderVec2("Size", size, Vec2.New(0.0), Vec2.New(500.0))
			ImGui.SliderRGB("Color", col)
			ImGui.End()

			Renderer.BeginScene()
			Renderer.DrawSprite(pos, size, Vec4.New(col.x, col.y, col.z, 1.0))
			Renderer.EndScene()
			

			ImmGFX.DrawCircleWire(Vec2.New(500.0), 4)

			coroutine.yield()
		end
	end
}

main_thread = coroutine.create(game.update)