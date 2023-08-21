local audio = require("audio")
audio.playOscillator("sine", 400, 0.5)

while true do
    local s = io.read(1)
    print(s)
end
