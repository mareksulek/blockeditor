local NORMAL_TEXT = {
    item = true,
    object = "text",
    grid = false,
    parsable = false,
    text = {
        isColor = false
    }
}

local CHANGED_TEXT = {
    item = true,
    object = "text",
    grid = false,
    parsable = false,
    text = {
        isColor = false
    }
}

local INVALID_TEXT = {
    item = true,
    object = "text",
    grid = false,
    parsable = false,
    text = {
        isColor = false
    }
}

local function COLOR_TEXT(color)
    return {
        item = true,
        object = "text",
        grid = false,
        parsable = false,
        text = {
            isColor = true,
            color = color
        }
    }
end


local function COLOR_BOLD_TEXT(color)
    return {
        item = true,
        object = "text",
        grid = false,
        parsable = false,
        text = {
            isColor = true,
            isBold = true,
            color = color
        }
    }
end

local VERT_BLOCK = {
    grid = true,
    parsable = true,
    object = "vertical"
}

local AUX_VERT = {
    grid = true,
    parsable = false,
    object = "vertical"
}

local AUX_LINE = {
    grid = true,
    parsable = false,
    object = "horizontal"
}

local LINE = {
    grid = true,
    parsable = false,
    object = "horizontal"
}

local LINE_PARSABLE = {
    grid = true,
    parsable = true,
    object = "horizontal"
}

local FUNCTIONDEF = {
    grid = true,
    parsable = false,
    object = "vertical"
}

local CYCLE = {
    item = true,
    parsable = false,
    object = "vertical"
}

local FUNCTION_STAT = {
    grid = true,
    parsable = false,
    object = "horizontal"
}

return {
        root = VERT_BLOCK,
        block = VERT_BLOCK,
        do_stat = VERT_BLOCK,
        if_stat = VERT_BLOCK,
        while_stat = VERT_BLOCK,
        repeat_stat = VERT_BLOCK,
        for_stat = VERT_BLOCK,
        aux_vert = AUX_VERT,
        aux_line = AUX_LINE,


	header = LINE,
	par_list = LINE,
	dot_comma = LINE,
        assing_stat = LINE_PARSABLE, --parsable
        functioncall_stat = LINE_PARSABLE,
        label_stat = LINE_PARSABLE,
        break_stat = LINE_PARSABLE,
        goto_stat = LINE_PARSABLE,
        return_stat = LINE_PARSABLE,
        exp = LINE,

        keyword = COLOR_BOLD_TEXT({ 0, 0, 1 }),
        operator = COLOR_BOLD_TEXT({ 1, 0, 1 }),
        comment = COLOR_TEXT({ 0.4, 0, 0 }),

	Name = NORMAL_TEXT,
        Number = COLOR_BOLD_TEXT({ 0, 0.8, 0 }),
        String = COLOR_TEXT({ 1, 0, 0 }),
        spaced_text = NORMAL_TEXT,
        changed_text = CHANGED_TEXT,

        function_stat = FUNCTION_STAT,
        functiondef = FUNCTIONDEF,
        cycle = CYCLE
}
