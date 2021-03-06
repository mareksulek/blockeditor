local NORMAL_TEXT = {
	item = true,
	object = "text",
	text = {
		size = 11,
		color = 0,
		bold = false,
	}
}

local function COLOR_TEXT(color)
	return {
		item = true,
		object = "text",
		text = {
			size = 11,
			color = color,
			bold = false,
		}
	}
end

local function COLOR_BOLD_TEXT(color)
	return {
		item = true,
		object = "text",
		text = {
			size = 11,
			color = color,
			bold = false,
		}
	}
end

local function COLOR_BLOCK(color) 
	return {
		grid = true,
		object = "vertical",
		background = {
			padding = 10,
			radius = 10,
			color = color,
		}
	}
end

local LINE = {
	grid = true,
	object = "horizontal",
}

return {
        root = COLOR_BLOCK({ 0.5, 0.5, 1.0, 0.1 }),
	block = COLOR_BLOCK({ 0.5, 0.5, 1.0, 0.1 }),
	do_stat = COLOR_BLOCK({ 0.5, 0.5, 1.0, 0.1 }),
	if_stat = COLOR_BLOCK({ 0.5, 0.5, 1.0, 0.1 }),
	while_stat = COLOR_BLOCK({1, 0.3, 0.3, 0.2}),
	repeat_stat = COLOR_BLOCK({1, 0.3, 0.3, 0.2}),
	for_stat = COLOR_BLOCK({1, 0.3, 0.3, 0.2}),

	function_stat = {
		grid = true,
		object = "horizontal",
		background = {
			padding = 5,
			color = {0.3, 1, 0.3, 0.2},
		},
		border = {
			size = 5,
		}
	},

	functiondef = {
		grid = true,
		object = "vertical",
	},
	
	header = LINE,
	par_list = LINE,
	dot_comma = LINE,
	assing_stat = LINE,
	functioncall_stat = LINE,
	label_stat = LINE,
	break_stat = LINE,
	goto_stat = LINE,
	return_stat = LINE,
	exp = LINE,

	keyword = COLOR_BOLD_TEXT({ 0, 0, 1 }),
	operator = COLOR_BOLD_TEXT({ 1, 0, 1 }),
	comment = COLOR_TEXT(0.5),

	Name = NORMAL_TEXT,
	Number = COLOR_BOLD_TEXT({ 0, 0.8, 0 }),
	String = COLOR_TEXT({ 1, 0, 0 }),

	cycle = {
		item = true,
		object = "image",
		image = "cycle.png",
	}
}
