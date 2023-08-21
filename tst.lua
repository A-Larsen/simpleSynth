local audio = require("audio")
audio.playOscillator("square", 80, 0.5)

while true do
    local s = io.read(1)
    print(s)
end
