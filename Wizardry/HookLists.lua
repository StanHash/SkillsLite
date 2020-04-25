#!/usr/bin/env lua

------------
-- CONFIG --
------------

SCRIPT_BIT_PATTERN = "^//%+"
SCRIPT_BIT_SKIPAMT = 3

EXECUTE_AFTER_FUNCS = {}
EXECUTE_BEFORE_FUNCS = {}

---------------------------
-- FUNCTION DECLARATIONS --
---------------------------

function generate_script(infiles, outfile)
	-- open the output file
	local out = io.open(outfile, "w")

	-- iterate through the input files
	for _, infile in ipairs(infiles) do
		-- iterate through the lines from the input file
		for line in io.lines(infile) do
			if line:find(SCRIPT_BIT_PATTERN) ~= nil then
				-- write script bit if line matches pattern
				out:write(line:sub(SCRIPT_BIT_SKIPAMT + 1) .. "\n")
			end
		end
	end

	-- close output file
	out:close()
end

function execute_after(func)
	EXECUTE_AFTER_FUNCS[#EXECUTE_AFTER_FUNCS+1] = func
end

function execute_before(func)
	EXECUTE_BEFORE_FUNCS[#EXECUTE_BEFORE_FUNCS+1] = func
end

-----------------
-- SCRIPT MAIN --
-----------------

-- args is list of files to check for
local files = { ... }

-- tmp file for writing the script in
local tmpFilename = os.tmpname()

-- generate the temp script
generate_script(files, tmpFilename)


-- execute the temp script
dofile(tmpFilename)

-- execute "before" funcs
for _, func in ipairs(EXECUTE_BEFORE_FUNCS) do
	func()
end

-- execute "after" funcs
for _, func in ipairs(EXECUTE_AFTER_FUNCS) do
	func()
end

-- remove temp script
os.remove(tmpFilename)
