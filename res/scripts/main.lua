
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
	uniform vec2 PrevMousePos;
	uniform vec2 MouseState;

	float DistanceToLine(vec2 v, vec2 w, vec2 p) {
		float l2 = pow(length(v - w), 2);
		if (l2 == 0.0f) { return length(p - v); }

		float t = max(0, min(1, dot(p - v, w - v) / l2));
		vec2 projection = v + t * (w - v);
		return length(p - projection);
	}

	void main() {
		float dist = DistanceToLine(MousePos, PrevMousePos, gl_FragCoord.xy);
		if (dist <= MouseState.y && MouseState.x > 0.0f) {
			FragColor = vec4(1, 1, 0, 1);
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

fbo = Framebuffer.Create(Vec2.New(1280, 720))

game = {
	update = function()
		state = Vec2.New(0, 4)
		prevMousePos = Vec2.New(Input.RawMousePos.x, 720 - Input.RawMousePos.y)
		currentMousePos = prevMousePos

		while(true) do
			
			prevMousePos = currentMousePos
			currentMousePos = Vec2.New(Input.RawMousePos.x, 720 - Input.RawMousePos.y)

			if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
				state.x = 1
			else
				state.x = -1
			end

			if (state.x > 0) then

				fbo:Bind()

				sp:Bind()
				sp:SetUniformVec2("MousePos", currentMousePos)
				sp:SetUniformVec2("PrevMousePos", prevMousePos)
				sp:SetUniformVec2("MouseState", state)
				va:Bind()
				va:Draw(6)

				fbo:UnBind()
				fbo:Resolve()
			end

			if (Input.IsKeyDown(Input.KEY_C)) then
				fbo:Bind()
				fbo:Clear()
				fbo:UnBind()
				fbo:Resolve()
			end

			fbo:DrawToScreen()

			ImGui.Begin("Lua ImGui")
			ImGui.SliderVec2("Position", pos, Vec2.New(0.0), Vec2.New(2000.0))
			ImGui.SliderVec2("Size", size, Vec2.New(0.0), Vec2.New(500.0))
			ImGui.SliderRGB("Color", col)
			state.y = ImGui.SliderFloat("BrushSize", state.y, 0.0, 50.0)
			ImGui.End()

			Renderer.BeginScene()
			Renderer.DrawSprite(pos, size, Vec4.New(col.x, col.y, col.z, 1.0))
			Renderer.EndScene()

			ImmGFX.DrawCircleWire(Input.MousePos, state.y)

			coroutine.yield()
		end
	end
}

main_thread = coroutine.create(game.update)