# NOTE:run this script before doing anything else! that includes opening neovim!
#
# make sure to run neovim in developer powershell
#

$env:LUA_DEV = "C:\Program Files (x86)\lua\5.4.2"
$env:LUA_INC = "$env:LUA_DEV\include"

# The developer console will search for this environment variable to know where
# to look for include files
$env:INCLUDE = "$env:LUA_INC;$env:INCLUDE"

# this searches your lua exe directory
$env:LUA_PATH=".\\?.lua;!\\lua\\?.lua;!\\lua\\?\\init.lua;!\\?.lua;!\\?\\init.lua"

# this searches your local directory
# this is a relative path, not an absolute path
$env:LUA_PATH="$env:LUA_PATH;.\\\lua\\?.lua;.\\\lua\\?\\init.lua;.\\\?.lua;.\\\?\\init.lua"
