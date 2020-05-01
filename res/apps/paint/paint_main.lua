
System.DoFile("res/apps/paint/rpc_module.lua")

local make_paint = function()
	
	local paint = {}

	paint.__index = paint

	function paint:init()
		self.previous_mouse_pos = Vec2.New(Input.RawMousePos.x, Window.GetSize().y - Input.RawMousePos.y)
		self.current_mouse_pos = self.previous_mouse_pos

		self.clicked_mouse_pos = Vec2.New(0.0)	-- Position where the mouse is clicked

		self.brush_size = 2.0
		self.brush_color = Vec4.New(0.0, 0.0, 0.0, 1.0)

		self.current_tool = "BRUSH"

		rpc:bind("draw_line", self.rpc_draw_line, self)			-- To draw line
		rpc:bind("draw_circle", self.rpc_draw_circle, self)		-- To draw circle
		rpc:bind("draw_rect", self.rpc_draw_rect, self)			-- To draw rectangle
		rpc:bind("clear", self.rpc_clear, self)					-- To clear screen
	end

	function paint:rpc_draw_line(start_x, start_y, end_x, end_y, brush_size, brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		PaintRenderer.DrawLine(
			Vec2.New(start_x, start_y),
			Vec2.New(end_x, end_y), 
			brush_size,
			Vec4.New(brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		)
	end
	
	function paint:rpc_draw_circle(center_x, center_y, radius, brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		PaintRenderer.DrawCircle(
			Vec2.New(center_x, center_y),
			radius,
			Vec4.New(brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		)
	end

	function paint:rpc_draw_rect(pos1_x, pos1_y, pos2_x, pos2_y, brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		PaintRenderer.DrawRect(
			Vec2.New(pos1_x, pos1_y),
			Vec2.New(pos2_x, pos2_y),
			Vec4.New(brush_color_x, brush_color_y, brush_color_z, brush_color_w)
		)
	end

	function paint:rpc_clear()
		PaintRenderer.Clear()
	end

	function paint:update(dt)
		self.previous_mouse_pos = self.current_mouse_pos
		self.current_mouse_pos = Vec2.New(Input.RawMousePos.x, Window.GetSize().y - Input.RawMousePos.y)

		if (Input.IsMouseKeyPressed(Input.MOUSE_BUTTON_LEFT)) then
			self.clicked_mouse_pos = self.current_mouse_pos
		end

		rpc:update()
	end

	function paint:render()
		if (self.current_tool == "BRUSH") then
			if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
				PaintRenderer.DrawLine(self.current_mouse_pos, self.previous_mouse_pos, self.brush_size, self.brush_color)
				rpc:call("draw_line", self.current_mouse_pos.x, self.current_mouse_pos.y,
					self.previous_mouse_pos.x, self.previous_mouse_pos.y, self.brush_size, self.brush_color.x, self.brush_color.y, self.brush_color.z, self.brush_color.w)
			end

			ImmGFX.Color(Vec4.New(0.0, 0.0, 0.0, 1.0))
			ImmGFX.DrawCircleWire(self.current_mouse_pos, self.brush_size)
		elseif (self.current_tool == "CIRCLE") then
			if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
				-- Don't yet draw the circle on the screen. Just show a preview of the circle
				local radius = self.current_mouse_pos - self.clicked_mouse_pos
				radius = math.sqrt((radius.x * radius.x) + (radius.y * radius.y))

				ImmGFX.Color(self.brush_color)
				ImmGFX.DrawCircle(self.clicked_mouse_pos, radius)
			elseif (Input.IsMouseKeyReleased(Input.MOUSE_BUTTON_LEFT)) then
				-- When the user release the mouse button then draw the circle
				local radius = self.current_mouse_pos - self.clicked_mouse_pos
				radius = math.sqrt((radius.x * radius.x) + (radius.y * radius.y))

				PaintRenderer.DrawCircle(self.clicked_mouse_pos, radius, self.brush_color)
				rpc:call("draw_circle", self.clicked_mouse_pos.x, self.clicked_mouse_pos.y,
					radius, self.brush_color.x, self.brush_color.y, self.brush_color.z, self.brush_color.w
				)
			end
		elseif (self.current_tool == "RECT") then
			if (Input.IsMouseKeyDown(Input.MOUSE_BUTTON_LEFT)) then
				ImmGFX.Color(self.brush_color)
				ImmGFX.DrawRect(self.clicked_mouse_pos + (self.current_mouse_pos - self.clicked_mouse_pos) / Vec2.New(2.0), self.current_mouse_pos - self.clicked_mouse_pos)
			elseif (Input.IsMouseKeyReleased(Input.MOUSE_BUTTON_LEFT)) then
				PaintRenderer.DrawRect(self.clicked_mouse_pos, self.current_mouse_pos, self.brush_color)
				rpc:call("draw_rect", self.clicked_mouse_pos.x, self.clicked_mouse_pos.y,
					self.current_mouse_pos.x, self.current_mouse_pos.y,
					self.brush_color.x, self.brush_color.y, self.brush_color.z, self.brush_color.w
				)
			end
		end

		if (Input.IsKeyDown(Input.KEY_C)) then
			PaintRenderer.Clear()
			rpc:call("clear")
		end

		PaintRenderer.Render()
	end

	function paint:gui()
		ImGui.Begin("Props")

		if (ImGui.RadioButton("Brush", self.current_tool == "BRUSH")) then
			self.current_tool = "BRUSH"
		end

		if (ImGui.RadioButton("Circle", self.current_tool == "CIRCLE")) then
			self.current_tool = "CIRCLE"
		end

		if (ImGui.RadioButton("Rectangle", self.current_tool == "RECT")) then
			self.current_tool = "RECT"
		end

		self.brush_size = ImGui.SliderFloat("Brush Size", self.brush_size, 1.0, 100.0)
		ImGui.SliderRGBA("Brush Color", self.brush_color)
		ImGui.End()
	end

	return paint
end

g_app_main = make_paint()
g_app_main:init()