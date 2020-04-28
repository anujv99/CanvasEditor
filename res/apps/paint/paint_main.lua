
System.DoFile("res/apps/paint/rpc_module.lua")

local make_paint = function()
	
	local paint = {}

	paint.__index = paint

	function paint:init()
		self.previous_mouse_pos = Vec2.New(Input.RawMousePos.x, Config.Height - Input.RawMousePos.y)
		self.current_mouse_pos = self.previous_mouse_pos
		self.brush_size = 1.5
		self.brush_color = Vec4.New(1.0, 1.0, 0.0, 1.0)

		rpc:bind("draw_line", self.rpc_draw_line, self)		-- To draw line
		rpc:bind("clear", self.rpc_clear, self)				-- To clear screen
	end

	function paint:rpc_draw_line(start_x, start_y, end_x, end_y, brush_size, brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		PaintRenderer.DrawLine(
			Vec2.New(start_x, start_y),
			Vec2.New(end_x, end_y), 
			brush_size,
			Vec4.New(brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		)
	end
	
	function paint:rpc_clear()
		PaintRenderer.Clear()
	end

	function paint:update(dt)
		self.previous_mouse_pos = self.current_mouse_pos
		self.current_mouse_pos = Vec2.New(Input.RawMousePos.x, Config.Height - Input.RawMousePos.y)
		rpc:update()
	end

	function paint:render()
		if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
			PaintRenderer.DrawLine(self.current_mouse_pos, self.previous_mouse_pos, self.brush_size, self.brush_color)
			rpc:call("draw_line", self.current_mouse_pos.x, self.current_mouse_pos.y,
				self.previous_mouse_pos.x, self.previous_mouse_pos.y, self.brush_size, self.brush_color.x, self.brush_color.y, self.brush_color.z, self.brush_color.w)
		end

		if (Input.IsKeyDown(Input.KEY_C)) then
			PaintRenderer.Clear()
			rpc:call("clear")
		end

		PaintRenderer.Render()

		ImmGFX.DrawCircleWire(self.current_mouse_pos, self.brush_size)
	end

	function paint:gui()
		ImGui.Begin("Props")
		self.brush_size = ImGui.SliderFloat("Brush Size", self.brush_size, 1.0, 100.0)
		ImGui.SliderRGBA("Brush Color", self.brush_color)
		ImGui.End()
	end

	return paint
end

g_app_main = make_paint()
g_app_main:init()