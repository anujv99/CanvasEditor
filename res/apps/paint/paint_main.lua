
local make_paint = function()
	
	local paint = {}

	paint.__index = paint

	function paint:init()
		local vertex_shader_code = [[
			#version 450 core

			layout (location = 0) in vec2 aPos;

			void main() {
				gl_Position = vec4(aPos.xy, 0.0f, 1.0f);
			}
		]]

		local fragment_shader_code = [[
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

		self.shader = Shader.Create(vertex_shader_code, fragment_shader_code)

		local vertex_buffer = VertexBuffer.Create({
			-1.0,  1.0,
			 1.0,  1.0,
			 1.0, -1.0,
			-1.0,  1.0,
			 1.0, -1.0,
			-1.0, -1.0,
		})

		local buffer_layout = BufferLayout.Create()
		buffer_layout:BeginEntries()
		buffer_layout:AddEntry(DataType.Float2, 0, false)
		buffer_layout:EndEntries()

		vertex_buffer:SetBufferLayout(buffer_layout)

		self.vertex_array = VertexArray.Create()
		self.vertex_array:AddVertexBuffer(vertex_buffer)

		self.framebuffer = Framebuffer.Create(Vec2.New(1280.0, 720.0))

		self.previous_mouse_pos = Vec2.New(Input.RawMousePos.x, 720.0 - Input.RawMousePos.y)
		self.current_mouse_pos = self.previous_mouse_pos
		self.mouse_state = Vec2.New(0, 4)
	end

	function paint:update(dt)
		self.previous_mouse_pos = self.current_mouse_pos
		self.current_mouse_pos = Vec2.New(Input.RawMousePos.x, 720.0 - Input.RawMousePos.y)

		if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
			self.mouse_state.x =  1.0
		else
			self.mouse_state.x = -1.0
		end
	end

	function paint:render()
		if (self.mouse_state.x > 0.0) then
			self.framebuffer:Bind()
			self.shader:Bind()
			self.vertex_array:Bind()

			self.shader:SetUniformVec2("MousePos", self.current_mouse_pos)
			self.shader:SetUniformVec2("PrevMousePos", self.previous_mouse_pos)
			self.shader:SetUniformVec2("MouseState", self.mouse_state)

			self.vertex_array:Draw(6)

			self.framebuffer:UnBind()
			self.framebuffer:Resolve()
		end

		if (Input.IsKeyDown(Input.KEY_C)) then
			self.framebuffer:Bind()
			self.framebuffer:Clear()
			self.framebuffer:UnBind()
			self.framebuffer:Resolve()
		end

		self.framebuffer:DrawToScreen()

		ImmGFX.DrawCircleWire(self.current_mouse_pos, self.mouse_state.y)
	end

	function paint:gui()
		ImGui.Begin("Props")
		self.mouse_state.y = ImGui.SliderFloat("Brush Size", self.mouse_state.y, 1.0, 100.0)
		ImGui.End()
	end

	return paint
end

g_app_main = make_paint()
g_app_main:init()