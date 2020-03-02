
local make_rpc = function()
	
	local rpc = {}

	rpc.__index = rpc

	rpc.func_entries = {}

	function rpc:update()
		if (Network.IsConnected()) then
			messages = { Network.Recv() }

			for k, v in pairs(messages) do

				msg = v
				params = { Network.DeMarshall(msg) }
				func_name = params[1]
				func = self.func_entries[func_name]
				if (func ~= nil) then
					table.remove(params, 1)
					callable = func[1]
					this = func[2]
					if (this ~= nil) then
						callable(this, table.unpack(params))
					else
						callable(table.unpack(params))
					end
				end
			end
		end
	end

	function rpc:bind(name, func, this)
		if (func ~= nil and name ~= nil) then
			self.func_entries[name] = { func, this }
		end
	end

	function rpc:call(name, ...)
		if (Network.IsConnected()) then
			if (name ~= nil) then
				msg = Network.Marshall(name, ...)
				Network.Send(msg)
			end
		end
	end

	return rpc
end

rpc = make_rpc()
