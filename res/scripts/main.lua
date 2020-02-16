
-- Do not delete this file

pos = Vec2.New(200.0)
size = Vec2.New(50.0)
col = Vec3.New(1.0)

vShader = [[
	#version 450 core

	layout (location = 0) in vec2 aPos;

	void main() {
		gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
	}
]]

fShader = [[
	#version 450 core

	out vec4 FragColor;

	uniform vec2 MousePos;

	void main() {
		vec2 FragPos = gl_FragCoord.xy;
		if (length(FragPos - MousePos) <= 10) {
			FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		} else {
			discard;
		}

		
	}
]]

sp = Shader.Create(vShader, fShader)
vb = VertexBuffer.Create({
	-1, 1,
	1, 1,
	1, -1,

	-1, 1,
	1, -1,
	-1, -1,
})

va = VertexArray.Create()

bl = BufferLayout.Create()

bl:BeginEntries()
bl:AddEntry(DataType.Float2, 0, false)
bl:EndEntries()

vb:SetBufferLayout(bl)

va:AddVertexBuffer(vb)

game = {
	update = function()
		while(true) do

			sp:Bind()
			sp:SetUniformVec2("MousePos", Input.MousePos)
			va:Bind()
			va:Draw(6)

			ImGui.Begin("Lua ImGui")
			ImGui.SliderVec2("Position", pos, Vec2.New(0.0), Vec2.New(2000.0))
			ImGui.SliderVec2("Size", size, Vec2.New(0.0), Vec2.New(500.0))
			ImGui.SliderRGB("Color", col)
			ImGui.End()

			Renderer.BeginScene()
			Renderer.DrawSprite(pos, size, Vec4.New(col.x, col.y, col.z, 1.0))
			Renderer.EndScene()

			ImmGFX.DrawCircleWire(Input.MousePos, 4)

			coroutine.yield()
		end
	end
}

main_thread = coroutine.create(game.update)