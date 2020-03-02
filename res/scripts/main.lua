

app_path = "res/apps/paint/paint_main.lua"

--[[ Do not modify this file beyond this point]]--

local create_main_thread = function()
	
	local app = {}

	app.__index = app

	app.current_program = nil

	function app:init()
		System.DoFile(app_path)
		if (g_app_main == nil) then
			print("Invalid script file ", app_path)
			return;
		end
		if (g_app_main.update == nil) then
			print("Invalid script file ", app_path)
			return;
		end
		if (g_app_main.render == nil) then
			print("Invalid script file ", app_path)
			return;
		end
		self.current_program = g_app_main
	end

	function app:update(dt) -- deltaTime
		if (self.current_program ~= nil) then
			self.current_program:update(dt)
		end
	end

	function app:render()
		if (self.current_program ~= nil) then
			self.current_program:render()
		end
	end

	function app:gui()
		if (self.current_program ~= nil) then
			self.current_program:gui()
		end
	end

	return app
end

g_main_thread = create_main_thread()
g_main_thread:init()